/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    pline.c    $NHDT-Date: 1549327495 2019/02/05 00:44:55 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.73 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

#define NEED_VARARGS /* Uses ... */ /* comment line for pre-compiled headers */
#include "hack.h"

#define BIGBUFSZ (5 * BUFSZ) /* big enough to format a 4*BUFSZ string (from
                              * config file parsing) with modest decoration;
                              * result will then be truncated to BUFSZ-1 */

                              /* `prefix' must be a string literal, not a pointer */
#define YouPrefix(pointer, prefix, text) \
    Strcpy((pointer = You_buf(strlen(text) + sizeof prefix)), prefix)

#define YouMessage(pointer, prefix, text) \
    strcat((YouPrefix(pointer, prefix, text), pointer), text)


STATIC_VAR int pline_attr = 0;
STATIC_VAR int pline_color = NO_COLOR;
STATIC_VAR unsigned pline_flags = 0;
STATIC_VAR int pline_prefix_attr = 0;
STATIC_VAR int pline_prefix_color = NO_COLOR;
STATIC_VAR const char* pline_prefix_text = 0;
STATIC_VAR int pline_separator_attr = 0;
STATIC_VAR int pline_separator_color = NO_COLOR;
STATIC_VAR const char* pline_separator_text = 0;
STATIC_VAR int* pline_multiattrs = 0;
STATIC_VAR int* pline_multicolors = 0;

STATIC_VAR char prevmsg[BUFSZ];

STATIC_DCL void FDECL(putmesg, (const char *));
STATIC_DCL void FDECL(putmesg_ex2, (const char*, const char*, const char*));
STATIC_DCL char *FDECL(You_buf, (size_t));
#if defined(MSGHANDLER) && (defined(POSIX_TYPES) || defined(__GNUC__))
STATIC_DCL void FDECL(execplinehandler, (const char *));
#endif

#if defined (DUMPLOG) || defined (DUMPHTML)
/* also used in end.c */
unsigned saved_pline_index = 0; /* slot in saved_plines[] to use next */
char *saved_plines[DUMPLOG_MSG_COUNT] = { (char *) 0 };
char* saved_pline_attrs[DUMPLOG_MSG_COUNT] = { (char*)0 };
char* saved_pline_colors[DUMPLOG_MSG_COUNT] = { (char*)0 };

/* keep the most recent DUMPLOG_MSG_COUNT messages */
void
dumplogmsg(line, attrs, colors, attr, color)
const char *line, *attrs, *colors;
int attr, color;
{
    /*
     * TODO:
     *  This essentially duplicates message history, which is
     *  currently implemented in an interface-specific manner.
     *  The core should take responsibility for that and have
     *  this share it.
     */
    unsigned indx = saved_pline_index; /* next slot to use */
    char *oldest = saved_plines[indx]; /* current content of that slot */
    char* oldest_attrs = saved_pline_attrs[indx]; /* current attrs of that slot */
    char* oldest_colors = saved_pline_colors[indx]; /* current attrs of that slot */

    size_t len = strlen(line);
    if (oldest && strlen(oldest) >= len) {
        /* this buffer will gradually shrink until the 'else' is needed;
           there's no pressing need to track allocation size instead */
        Strcpy(oldest, line);
        if (attrs)
        {
            memcpy(oldest_attrs, attrs, len + 1);
            memcpy(oldest_colors, colors, len + 1);
        }
        else
        {
            memset(oldest_attrs, attr, len);
            memset(oldest_colors, color, len);
            oldest_attrs[len] = 0;
            oldest_colors[len] = 0;
        }
    } else {
        if (oldest)
        {
            free((genericptr_t)oldest);
            free((genericptr_t)oldest_attrs);
            free((genericptr_t)oldest_colors);
        }
        saved_plines[indx] = dupstr(line);
        if(attrs)
            saved_pline_attrs[indx] = cpystr(line, attrs);
        else
            saved_pline_attrs[indx] = setstr(line, (char)attr);
        if (colors)
            saved_pline_colors[indx] = cpystr(line, colors);
        else
            saved_pline_colors[indx] = setstr(line, (char)color);
    }
    saved_pline_index = (indx + 1) % DUMPLOG_MSG_COUNT;
}

/* called during save (unlike the interface-specific message history,
   this data isn't saved and restored); end-of-game releases saved_pline[]
   while writing its contents to the final dump log */
void
dumplogfreemessages()
{
    unsigned indx;

    for (indx = 0; indx < DUMPLOG_MSG_COUNT; ++indx)
    {
        if (saved_plines[indx])
            free((genericptr_t)saved_plines[indx]), saved_plines[indx] = 0;
        if (saved_pline_attrs[indx])
            free((genericptr_t)saved_pline_attrs[indx]), saved_pline_attrs[indx] = 0;
        if (saved_pline_colors[indx])
            free((genericptr_t)saved_pline_colors[indx]), saved_pline_colors[indx] = 0;
    }
    saved_pline_index = 0;
}
#endif

/* keeps windowprocs usage out of pline() */
STATIC_OVL void
putmesg(line)
const char *line;
{
    if (!line)
        return;

    int attr = ATR_NONE, color = NO_COLOR;

    if ((pline_flags & URGENT_MESSAGE) != 0
        && (windowprocs.wincap2 & WC2_URGENT_MESG) != 0)
        attr |= ATR_URGENT;
    if ((pline_flags & SUPPRESS_HISTORY) != 0
        && (windowprocs.wincap2 & WC2_SUPPRESS_HIST) != 0)
        attr |= ATR_NOHISTORY;
    if ((pline_flags & STAY_ON_LINE) != 0)
        attr |= ATR_STAY_ON_LINE;

    attr |= pline_attr;
    color = pline_color;

    putstr_ex(WIN_MESSAGE, line, attr, color, 0);
}

STATIC_OVL void
putmesg_ex2(line, attrs, colors)
const char* line, *attrs, *colors;
{
    if (!line)
        return;

    int attr = ATR_NONE, color = NO_COLOR;

    if ((pline_flags & URGENT_MESSAGE) != 0
        && (windowprocs.wincap2 & WC2_URGENT_MESG) != 0)
        attr |= ATR_URGENT;
    if ((pline_flags & SUPPRESS_HISTORY) != 0
        && (windowprocs.wincap2 & WC2_SUPPRESS_HIST) != 0)
        attr |= ATR_NOHISTORY;
    if ((pline_flags & STAY_ON_LINE) != 0)
        attr |= ATR_STAY_ON_LINE;

    putstr_ex2(WIN_MESSAGE, line, attrs, colors, attr, color, 0);
}

/* Note that these declarations rely on knowledge of the internals
 * of the variable argument handling stuff in "tradstdc.h"
 */

#if defined(USE_STDARG) || defined(USE_VARARGS)
STATIC_DCL void FDECL(vpline, (const char *, va_list));

/*VARARGS1*/
void
pline
VA_DECL(const char *, line)
{
    VA_START(line);
    VA_INIT(line, char *);
    vpline(line, VA_ARGS);
    VA_END();
}

# ifdef USE_STDARG
STATIC_OVL void
vpline(const char *line, va_list the_args)
# else
STATIC_OVL void
vpline(line, the_args)
const char *line;
va_list the_args;
# endif

#else /* USE_STDARG | USE_VARARG */

# define vpline pline

/*VARARGS1*/
void
pline
VA_DECL(const char *, line)
#endif /* USE_STDARG | USE_VARARG */
{       /* start of vpline() or of nested block in USE_OLDARG's pline() */
    static int in_pline = 0;
    char pbuf[BIGBUFSZ]; /* will get chopped down to BUFSZ-1 if longer */
    int ln;
    int msgtyp;
    boolean no_repeat;
    char multi_line[BIGBUFSZ], combined_line[BIGBUFSZ], attrs[BIGBUFSZ], colors[BIGBUFSZ];
    multi_line[0] = 0;
    combined_line[0] = 0;
    attrs[0] = 0;
    colors[0] = 0;
    /* Do NOT use VA_START and VA_END in here... see above */

    if (!line || !*line)
        return;
#ifdef HANGUPHANDLING
    if (program_state.done_hup)
        return;
#endif
    if (program_state.wizkit_wishing)
        return;

    const char* used_line = line;
    boolean domulti = FALSE;
    if (index(used_line, '%'))
    {
        if (pline_multiattrs && pline_multicolors)
        {
            domulti = TRUE;
            const char* echars = "cdieEfgGosuxXpn%";
            const char* sp = used_line;
            const char* p, * ep;
            char cbuf[BIGBUFSZ];
            char sbuf[BIGBUFSZ];
            int pos = 0;
            int idx = -1;
            do
            {
                p = index(sp, '%');
                if (!p)
                {
                    p = ep = eos((char*)sp);
                }
                else
                {
                    ep = p + 1;
                    idx++;
                }

                char typechar = '\0';
                char* cptr;
                while (*ep)
                {
                    cptr = index(echars, *ep);
                    if (!cptr)
                        ep++;
                    else
                    {
                        typechar = *cptr;
                        break;
                    }
                }
                int len1 = (int)(p - sp);
                int len2 = (int)(ep - p + 1);
                if (len1 < 0)
                    len1 = 0;
                if (len2 < 0)
                    len2 = 0;

                if (len1 > 0)
                {
                    strncpy(cbuf, sp, len1);
                    cbuf[len1] = 0;
                    Strcat(multi_line, cbuf);
                    memset(&attrs[pos], pline_attr, (size_t)len1);
                    memset(&colors[pos], pline_color, (size_t)len1);
                    pos += len1;
                }

                if (len2 > 0)
                {
                    strncpy(sbuf, p, len2);
                    sbuf[len2] = 0;
                    switch (typechar)
                    {
                    case 'c':
                        Sprintf(cbuf, sbuf, va_arg(the_args, CHAR_P));
                        break;
                    case 'f':
                        Sprintf(cbuf, sbuf, va_arg(the_args, double));
                        break;
                    default:
                    case 'i':
                    case 'd':
                        if (ep > p && *(ep - 1) == 'h')
                            Sprintf(cbuf, sbuf, va_arg(the_args, SHORT_P));
                        else if (ep > p && *(ep - 1) == 'l')
                            Sprintf(cbuf, sbuf, va_arg(the_args, long));
                        else
                            Sprintf(cbuf, sbuf, va_arg(the_args, int));
                        break;
                    case 's':
                        Sprintf(cbuf, sbuf, va_arg(the_args, const char*));
                        break;
                    case 'p':
                        Sprintf(cbuf, sbuf, va_arg(the_args, void*));
                        break;
                    case 'x':
                    case 'X':
                    case 'u':
                        if(ep > p && *(ep - 1) == 'h')
                            Sprintf(cbuf, sbuf, va_arg(the_args, UNSIGNED_SHORT_P));
                        else if (ep > p && *(ep - 1) == 'l')
                            Sprintf(cbuf, sbuf, va_arg(the_args, unsigned long));
                        else
                            Sprintf(cbuf, sbuf, va_arg(the_args, unsigned int));
                        break;
                    case '%':
                        strncpy(cbuf, sbuf, len2 - 1);
                        cbuf[len2 - 1] = 0;
                        break;
                        break;
                    }
                    size_t clen = strlen(cbuf);
                    Strcat(multi_line, cbuf);
                    memset(&attrs[pos], idx >= 0 && pline_multiattrs[idx] != ATR_NONE ? pline_multiattrs[idx] : pline_attr, clen);
                    memset(&colors[pos], idx >= 0 && pline_multicolors[idx] != NO_COLOR ? pline_multicolors[idx] : pline_color, clen);
                    pos += clen;
                }

                sp = ep + 1;
            } while (p && *p);

            multi_line[pos] = 0;
            attrs[pos] = 0;
            colors[pos] = 0;
            used_line = multi_line;
        }
        else
        {
            Vsprintf(pbuf, used_line, VA_ARGS);
            used_line = pbuf;
        }
    }

    const char* original_line = used_line;
    Sprintf(combined_line, "%s%s%s", pline_prefix_text ? pline_prefix_text : "", pline_separator_text ? pline_separator_text : "", original_line);
    boolean truncated = FALSE;
    if ((ln = (int)strlen(combined_line)) > BIGBUFSZ - 1) 
    {
        //if (original_line != line)                          /* no '%' was present */
        (void)strncpy(pbuf, combined_line, BIGBUFSZ - 1); /* caveat: unterminated */
        /* truncate, preserving the final 3 characters:
           "___ extremely long text" -> "___ extremely l...ext"
           (this may be suboptimal if overflow is less than 3) */
        (void)strncpy(pbuf + BIGBUFSZ - 1 - 6, "...", 3);
        /* avoid strncpy; buffers could overlap if excess is small */
        pbuf[BIGBUFSZ - 1 - 3] = combined_line[ln - 3];
        pbuf[BIGBUFSZ - 1 - 2] = combined_line[ln - 2];
        pbuf[BIGBUFSZ - 1 - 1] = combined_line[ln - 1];
        pbuf[BIGBUFSZ - 1] = '\0';
        truncated = TRUE;
        used_line = pbuf;
    }
    else
        used_line = combined_line;

    /* use raw_print() if we're called too early (or perhaps too late
       during shutdown) or if we're being called recursively (probably
       via debugpline() in the interface code) */
    if (in_pline++ || !iflags.window_inited) {
        /* [we should probably be using raw_printf("\n%s", line) here] */
        raw_print(used_line);
        iflags.last_msg = PLNMSG_UNKNOWN;
        goto pline_done;
    }

    msgtyp = MSGTYP_NORMAL;
    no_repeat = (pline_flags & PLINE_NOREPEAT) ? TRUE : FALSE;
    if ((pline_flags & OVERRIDE_MSGTYPE) == 0) {
        msgtyp = msgtype_type(used_line, no_repeat);
        if ((pline_flags & URGENT_MESSAGE) == 0
            && (msgtyp == MSGTYP_NOSHOW
                || (msgtyp == MSGTYP_NOREP && !strcmp(used_line, prevmsg))))
            /* FIXME: we need a way to tell our caller that this message
             * was suppressed so that caller doesn't set iflags.last_msg
             * for something that hasn't been shown, otherwise a subsequent
             * message which uses alternate wording based on that would be
             * doing so out of context and probably end up seeming silly.
             * (Not an issue for no-repeat but matters for no-show.)
             */
            goto pline_done;
    }

    if (!saving && !restoring && !reseting && !check_pointing && iflags.window_inited)
    {
        if (vision_full_recalc)
            vision_recalc(0);
    }
    if (u.ux && !program_state.in_bones && iflags.window_inited)
        flush_screen(!flags.show_cursor_on_u); // show_cursor_on_u actually indicates that there is a getpos going on, in which case the cursor should not be returned to the player

    boolean usemulti = pline_prefix_text || pline_separator_text || domulti;
    //if (domulti)
    //{
    //    putmesg_ex2(multi_line, attrs, colors);
    //}
    //else 
    if (usemulti)
    {
        size_t line_len = strlen(original_line);
        size_t prefix_len = pline_prefix_text ? strlen(pline_prefix_text) : 0;
        size_t separator_len = pline_separator_text ? strlen(pline_separator_text) : 0;
        if (truncated)
        {
            size_t used_len = strlen(used_line);
            int truncatedlen = (int)used_len - (int)prefix_len - (int)separator_len;
            line_len = truncatedlen >= 0 ? (size_t)truncatedlen : 0UL;
        }
        
        if (domulti && (prefix_len > 0 || separator_len > 0))
        {
            int offset = (int)prefix_len + (int)separator_len;
            int j;
            for (j = (int)line_len; j >= 0; j--) //also copy ending zero
            {
                attrs[j + offset] = attrs[j];
                colors[j + offset] = colors[j];
            }
            for (j = 0; j < offset; j++)
            {
                attrs[j] = 0;
                colors[j] = 0;
            }
        }

        char* attr_p = attrs;
        char* color_p = colors;
        if (prefix_len > 0)
        {
            memset(attr_p, pline_prefix_attr, prefix_len);
            attr_p += prefix_len;
            memset(color_p, pline_prefix_color, prefix_len);
            color_p += prefix_len;
        }
        if (separator_len > 0)
        {
            memset(attr_p, pline_separator_attr, separator_len);
            attr_p += separator_len;
            memset(color_p, pline_separator_color, separator_len);
            color_p += separator_len;
        }
        if (!domulti)
        {
            memset(attr_p, pline_attr, line_len);
            attr_p += line_len;
            *attr_p = 0;
            memset(color_p, pline_color, line_len);
            color_p += line_len;
            *color_p = 0;
        }
        putmesg_ex2(combined_line, attrs, colors);
    }
    else
        putmesg(used_line);

#if defined (DUMPLOG) || defined (DUMPHTML)
    /* We hook here early to have options-agnostic output.
     * Unfortunately, that means Norep() isn't honored (general issue) and
     * that short lines aren't combined into one longer one (tty behavior).
     */
    if ((pline_flags & SUPPRESS_HISTORY) == 0)
        dumplogmsg(used_line, usemulti ? attrs : 0, usemulti ? colors : 0, pline_attr, pline_color);
#endif

#if defined(MSGHANDLER) && (defined(POSIX_TYPES) || defined(__GNUC__))
    execplinehandler(used_line);
#endif

    /* this gets cleared after every pline message */
    iflags.last_msg = PLNMSG_UNKNOWN;
    (void) strncpy(prevmsg, used_line, BUFSZ), prevmsg[BUFSZ - 1] = '\0';
    if (msgtyp == MSGTYP_STOP && iflags.window_inited)
        display_nhwindow(WIN_MESSAGE, TRUE); /* --more-- */

 pline_done:
    --in_pline;
    return;

#if !(defined(USE_STDARG) || defined(USE_VARARGS))
    /* provide closing brace for the nested block
       which immediately follows USE_OLDARGS's VA_DECL() */
    VA_END();
#endif
}


/* pline() variant which can override MSGTYPE handling or suppress
   message history (tty interface uses pline() to issue prompts and
   they shouldn't be blockable via MSGTYPE=hide) */
/*VARARGS2*/
void custompline
VA_DECL2(unsigned, pflags, const char *, line)
{
    VA_START(line);
    VA_INIT(line, const char *);
    pline_flags = pflags;
    vpline(line, VA_ARGS);
    pline_flags = 0;
    VA_END();
    return;
}

void custompline_ex
VA_DECL4(int, attr, int, color, unsigned, pflags, const char*, line)
{
    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    pline_flags = pflags;
    vpline(line, VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    pline_flags = 0;
    VA_END();
    return;
}

void custompline_ex_prefix
VA_DECL10(int, prefix_attr, int, prefix_color, const char*, prefix_line, int, separator_attr, int, separator_color, const char*, separator_line, int, attr, int, color, unsigned, pflags, const char*, line)
{
    VA_START(line);
    VA_INIT(line, const char*);
    pline_prefix_attr = prefix_attr;
    pline_prefix_color = prefix_color;
    pline_prefix_text = prefix_line;
    pline_separator_attr = separator_attr;
    pline_separator_color = separator_color;
    pline_separator_text = separator_line;
    pline_attr = attr;
    pline_color = color;
    pline_flags = pflags;
    vpline(line, VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    pline_flags = 0;
    pline_prefix_attr = ATR_NONE;
    pline_prefix_color = NO_COLOR;
    pline_prefix_text = 0;
    pline_separator_attr = ATR_NONE;
    pline_separator_color = NO_COLOR;
    pline_separator_text = 0;
    VA_END();
    return;
}


void pline_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    vpline(line, VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
    return;
}

void pline_multi_ex
VA_DECL5(int, attr, int, color, int*, multiattrs, int*, multicolors, const char*, line)
{
    VA_START(line);
    VA_INIT(line, const char*);
    pline_multiattrs = multiattrs;
    pline_multicolors = multicolors;
    pline_attr = attr;
    pline_color = color;
    vpline(line, VA_ARGS);
    pline_multiattrs = 0;
    pline_multicolors = 0;
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
    return;
}


/*VARARGS1*/
void You_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    vpline(YouMessage(tmp, "You ", line), VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

void You_multi_ex
VA_DECL5(int, attr, int, color, int*, multiattrs, int*, multicolors, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    pline_multiattrs = multiattrs;
    pline_multicolors = multicolors;
    vpline(YouMessage(tmp, "You ", line), VA_ARGS);
    pline_multiattrs = 0;
    pline_multicolors = 0;
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void Your_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    vpline(YouMessage(tmp, "Your ", line), VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void Your_multi_ex
VA_DECL5(int, attr, int, color, int*, multiattrs, int*, multicolors, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    pline_multiattrs = multiattrs;
    pline_multicolors = multicolors;
    vpline(YouMessage(tmp, "Your ", line), VA_ARGS);
    pline_multiattrs = 0;
    pline_multicolors = 0;
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void You_feel_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    if (Unaware)
        YouPrefix(tmp, "You dream that you feel ", line);
    else
        YouPrefix(tmp, "You feel ", line);
    vpline(strcat(tmp, line), VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void You_cant_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    vpline(YouMessage(tmp, "You can't ", line), VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void pline_The_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    vpline(YouMessage(tmp, "The ", line), VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void There_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    vpline(YouMessage(tmp, "There ", line), VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void You_hear_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    if (Deaf || !flags.acoustics)
        return;
    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    if (Underwater)
        YouPrefix(tmp, "You barely hear ", line);
    else if (Unaware)
        YouPrefix(tmp, "You dream that you hear ", line);
    else
        YouPrefix(tmp, "You hear ", line);
    vpline(strcat(tmp, line), VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void You_see_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    if (Unaware)
        YouPrefix(tmp, "You dream that you see ", line);
    else if (Blind) /* caller should have caught this... */
        YouPrefix(tmp, "You sense ", line);
    else
        YouPrefix(tmp, "You see ", line);
    vpline(strcat(tmp, line), VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}

/*VARARGS1*/
void verbalize_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    char* tmp;

    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    tmp = You_buf(strlen(line) + sizeof "\"\"");
    Strcpy(tmp, "\"");
    Strcat(tmp, line);
    Strcat(tmp, "\"");
    vpline(tmp, VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    VA_END();
}



/*VARARGS1*/
void Norep
VA_DECL(const char *, line)
{
    VA_START(line);
    VA_INIT(line, const char *);
    pline_flags = PLINE_NOREPEAT;
    vpline(line, VA_ARGS);
    pline_flags = 0;
    VA_END();
    return;
}

void Norep_ex
VA_DECL3(int, attr, int, color, const char*, line)
{
    VA_START(line);
    VA_INIT(line, const char*);
    pline_attr = attr;
    pline_color = color;
    pline_flags = PLINE_NOREPEAT;
    vpline(line, VA_ARGS);
    pline_attr = ATR_NONE;
    pline_color = NO_COLOR;
    pline_flags = 0;
    VA_END();
    return;
}

/* work buffer for You(), &c and verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, ) */
STATIC_VAR char *you_buf = 0;
STATIC_VAR size_t you_buf_siz = 0;

STATIC_OVL char *
You_buf(siz)
size_t siz;
{
    if (siz > you_buf_siz) {
        if (you_buf)
            free((genericptr_t) you_buf);
        you_buf_siz = siz + 10;
        you_buf = (char *) alloc(you_buf_siz);
    }
    return you_buf;
}

void
free_youbuf()
{
    if (you_buf)
        free((genericptr_t) you_buf), you_buf = (char *) 0;
    you_buf_siz = 0;
}



/*VARARGS1*/
void You
VA_DECL(const char *, line)
{
    char *tmp;

    VA_START(line);
    VA_INIT(line, const char *);
    vpline(YouMessage(tmp, "You ", line), VA_ARGS);
    VA_END();
}

/*VARARGS1*/
void Your
VA_DECL(const char *, line)
{
    char *tmp;

    VA_START(line);
    VA_INIT(line, const char *);
    vpline(YouMessage(tmp, "Your ", line), VA_ARGS);
    VA_END();
}

/*VARARGS1*/
void You_feel
VA_DECL(const char *, line)
{
    char *tmp;

    VA_START(line);
    VA_INIT(line, const char *);
    if (Unaware)
        YouPrefix(tmp, "You dream that you feel ", line);
    else
        YouPrefix(tmp, "You feel ", line);
    vpline(strcat(tmp, line), VA_ARGS);
    VA_END();
}

/*VARARGS1*/
void You_cant
VA_DECL(const char *, line)
{
    char *tmp;

    VA_START(line);
    VA_INIT(line, const char *);
    vpline(YouMessage(tmp, "You can't ", line), VA_ARGS);
    VA_END();
}

/*VARARGS1*/
void pline_The
VA_DECL(const char *, line)
{
    char *tmp;

    VA_START(line);
    VA_INIT(line, const char *);
    vpline(YouMessage(tmp, "The ", line), VA_ARGS);
    VA_END();
}

/*VARARGS1*/
void There
VA_DECL(const char *, line)
{
    char *tmp;

    VA_START(line);
    VA_INIT(line, const char *);
    vpline(YouMessage(tmp, "There ", line), VA_ARGS);
    VA_END();
}

/*VARARGS1*/
void You_hear
VA_DECL(const char *, line)
{
    char *tmp;

    if (Deaf || !flags.acoustics)
        return;
    VA_START(line);
    VA_INIT(line, const char *);
    if (Underwater)
        YouPrefix(tmp, "You barely hear ", line);
    else if (Unaware)
        YouPrefix(tmp, "You dream that you hear ", line);
    else
        YouPrefix(tmp, "You hear ", line);
    vpline(strcat(tmp, line), VA_ARGS);
    VA_END();
}

/*VARARGS1*/
void You_see
VA_DECL(const char *, line)
{
    char *tmp;

    VA_START(line);
    VA_INIT(line, const char *);
    if (Unaware)
        YouPrefix(tmp, "You dream that you see ", line);
    else if (Blind) /* caller should have caught this... */
        YouPrefix(tmp, "You sense ", line);
    else
        YouPrefix(tmp, "You see ", line);
    vpline(strcat(tmp, line), VA_ARGS);
    VA_END();
}

/* Print a message inside double-quotes.
 * The caller is responsible for checking deafness.
 * Gods can speak directly to you in spite of deafness.
 */
/*VARARGS1*/
void verbalize
VA_DECL(const char *, line)
{
    char *tmp;

    VA_START(line);
    VA_INIT(line, const char *);
    tmp = You_buf(strlen(line) + sizeof "\"\"");
    Strcpy(tmp, "\"");
    Strcat(tmp, line);
    Strcat(tmp, "\"");
    vpline(tmp, VA_ARGS);
    VA_END();
}

/*VARARGS1*/
/* Note that these declarations rely on knowledge of the internals
 * of the variable argument handling stuff in "tradstdc.h"
 */

#if defined(USE_STDARG) || defined(USE_VARARGS)
STATIC_DCL void FDECL(vraw_printf, (const char *, va_list));

void raw_printf
VA_DECL(const char *, line)
{
    VA_START(line);
    VA_INIT(line, char *);
    vraw_printf(line, VA_ARGS);
    VA_END();
}

# ifdef USE_STDARG
STATIC_OVL void
vraw_printf(const char *line, va_list the_args)
# else
STATIC_OVL void
vraw_printf(line, the_args)
const char *line;
va_list the_args;
# endif

#else /* USE_STDARG | USE_VARARG */

void raw_printf
VA_DECL(const char *, line)
#endif
{
    char pbuf[BIGBUFSZ]; /* will be chopped down to BUFSZ-1 if longer */
    /* Do NOT use VA_START and VA_END in here... see above */

    if (index(line, '%')) {
        Vsprintf(pbuf, line, VA_ARGS);
        line = pbuf;
    }
    if ((int) strlen(line) > BUFSZ - 1) {
        if (line != pbuf)
            line = strncpy(pbuf, line, BUFSZ - 1);
        /* unlike pline, we don't futz around to keep last few chars */
        pbuf[BUFSZ - 1] = '\0'; /* terminate strncpy or truncate vsprintf */
    }
    raw_print(line);
#if defined(MSGHANDLER) && (defined(POSIX_TYPES) || defined(__GNUC__))
    execplinehandler(line);
#endif
#if !(defined(USE_STDARG) || defined(USE_VARARGS))
    VA_END(); /* (see vpline) */
#endif
}

/*VARARGS1*/
void impossible
VA_DECL(const char *, s)
{
    char pbuf[BIGBUFSZ]; /* will be chopped down to BUFSZ-1 if longer */

    VA_START(s);
    VA_INIT(s, const char *);
    if (program_state.in_impossible)
    {
        panic("impossible called impossible");
        return;
    }

    program_state.in_impossible = 1;
    Vsprintf(pbuf, s, VA_ARGS);
    pbuf[BUFSZ - 1] = '\0'; /* sanity */
    paniclog("impossible", pbuf);
    if (iflags.debug_fuzzer)
    {
        panic("%s", pbuf);
        return;
    }
    pline_ex(ATR_NONE, CLR_MSG_ERROR, "impossible: %s", VA_PASS1(pbuf));
    if (issue_gui_command)
        issue_gui_command(GUI_CMD_POST_DIAGNOSTIC_DATA, DIAGNOSTIC_DATA_IMPOSSIBLE, pbuf);

    /* reuse pbuf[] */
    Strcpy(pbuf, "Program in disorder!");
    if (program_state.something_worth_saving)
        Strcat(pbuf, "  (Saving and reloading may fix this problem.)");
    pline_ex(ATR_NONE, CLR_MSG_ERROR, "%s", VA_PASS1(pbuf));

    program_state.in_impossible = 0;
    VA_END();
}

#if defined(MSGHANDLER) && (defined(POSIX_TYPES) || defined(__GNUC__))
STATIC_VAR boolean use_pline_handler = TRUE;

STATIC_OVL void
execplinehandler(line)
const char *line;
{
    int f;
    const char *args[3];
    char *env;

    if (!use_pline_handler)
        return;

    if (!(env = nh_getenv("NETHACK_MSGHANDLER"))) {
        use_pline_handler = FALSE;
        return;
    }

    f = fork();
    if (f == 0) { /* child */
        args[0] = env;
        args[1] = line;
        args[2] = NULL;
        (void) setgid(getgid());
        (void) setuid(getuid());
        (void) execv(args[0], (char *const *) args);
        perror((char *) 0);
        (void) fprintf(stderr, "Exec to message handler %s failed.\n", env);
        nh_terminate(EXIT_FAILURE);
    } else if (f > 0) {
        int status;

        waitpid(f, &status, 0);
    } else if (f == -1) {
        perror((char *) 0);
        use_pline_handler = FALSE;
        pline("%s", VA_PASS1("Fork to message handler failed."));
    }
}
#endif /* MSGHANDLER && (POSIX_TYPES || __GNUC__) */

/*
 * varargs handling for files.c
 */
#if defined(USE_STDARG) || defined(USE_VARARGS)
STATIC_DCL void FDECL(vconfig_error_add, (const char *, va_list));

/*VARARGS1*/
void
config_error_add
VA_DECL(const char *, str)
{
    VA_START(str);
    VA_INIT(str, char *);
    vconfig_error_add(str, VA_ARGS);
    VA_END();
}

# ifdef USE_STDARG
STATIC_OVL void
vconfig_error_add(const char *str, va_list the_args)
# else
STATIC_OVL void
vconfig_error_add(str, the_args)
const char *str;
va_list the_args;
# endif

#else /* !(USE_STDARG || USE_VARARG) => USE_OLDARGS */

/*VARARGS1*/
void
config_error_add
VA_DECL(const char *, str)
#endif /* ?(USE_STDARG || USE_VARARG) */
{       /* start of vconf...() or of nested block in USE_OLDARG's conf...() */
    char buf[BIGBUFSZ]; /* will be chopped down to BUFSZ-1 if longer */

    Vsprintf(buf, str, VA_ARGS);
    buf[BUFSZ - 1] = '\0';
    config_erradd(buf);

#if !(defined(USE_STDARG) || defined(USE_VARARGS))
    VA_END(); /* (see pline/vpline -- ends nested block for USE_OLDARGS) */
#endif
}

void
reset_pline(VOID_ARGS)
{
    pline_attr = 0;
    pline_color = NO_COLOR;
    pline_flags = 0;
    pline_prefix_attr = 0;
    pline_prefix_color = NO_COLOR;
    pline_prefix_text = 0;
    pline_separator_attr = 0;
    pline_separator_color = NO_COLOR;
    pline_separator_text = 0;
    pline_multiattrs = 0;
    pline_multicolors = 0;

    prevmsg[0] = 0;

#if defined(MSGHANDLER) && (defined(POSIX_TYPES) || defined(__GNUC__))
    use_pline_handler = TRUE;
#endif
}

/*pline.c*/
