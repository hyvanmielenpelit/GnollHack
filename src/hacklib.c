/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    hacklib.c    $NHDT-Date: 1552639487 2019/03/15 08:44:47 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.67 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2007. */
/* Copyright (c) Robert Patrick Rankin, 1991                      */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h" /* for config.h+extern.h */

/*=
    Assorted 'small' utility routines.  They're virtually independent of
    GnollHack, except that rounddiv may call panic().  setrandom calls one
    of srandom(), srand48(), or srand() depending upon configuration.

      return type     routine name    argument type(s)
        boolean         digit           (char)
        boolean         letter          (char)
        char            highc           (char)
        char            lowc            (char)
        char *          lcase           (char *)
        char *          ucase           (char *)
        char *          upstart         (char *)
        char *          mungspaces      (char *)
        char *          trimspaces      (char *)
        char *          strip_newline   (char *)
        char *          stripchars      (char *, const char *, const char *)
        char *          eos             (char *)
        boolean         str_end_is      (const char *, const char *)
        char *          strkitten       (char *,char)
        void            copynchars      (char *,const char *,int)
        char            chrcasecpy      (int,int)
        char *          strcasecpy      (char *,const char *)
        char *          s_suffix        (const char *)
        char *          ing_suffix      (const char *)
        char *          xcrypt          (const char *, char *)
        boolean         onlyspace       (const char *)
        char *          tabexpand       (char *)
        char *          visctrl         (char)
        char *          strsubst        (char *, const char *, const char *)
        int             strNsubst       (char *,const char *,const char *,int)
        const char *    ordin           (int)
        char *          sitoa           (int)
        int             sgn             (int)
        int             rounddiv        (int, int)
        int             distmin         (int, int, int, int)
        int             dist2           (int, int, int, int)
        boolean         online2         (int, int)
        boolean         pmatch          (const char *, const char *)
        boolean         pmatchi         (const char *, const char *)
        boolean         pmatchz         (const char *, const char *)
        int             strncmpi        (const char *, const char *, int)
        char *          strstri         (const char *, const char *)
        boolean         fuzzymatch      (const char *, const char *,
                                         const char *, boolean)
        void            setrandom       (void)
        void            init_random     (fn)
        void            reseed_random   (fn)
        time_t          getnow          (void)
        int             getyear         (void)
        char *          yymmdd          (time_t)
        long            yyyymmdd        (time_t)
        long            hhmmss          (time_t)
        char *          yyyymmddhhmmss  (time_t)
        time_t          time_from_yyyymmddhhmmss (char *)
        int             phase_of_the_moon (void)
        boolean         friday_13th     (void)
        int             night           (void)
        int             midnight        (void)
        void            strbuf_init     (strbuf *, const char *)
        void            strbuf_append   (strbuf *, const char *)
        void            strbuf_reserve  (strbuf *, int)
        void            strbuf_empty    (strbuf *)
        void            strbuf_nl_to_crlf (strbuf_t *)
=*/
#ifdef LINT
#define Static /* pacify lint */
#else
#define Static static
#endif

STATIC_DCL boolean FDECL(pmatch_internal, (const char *, const char *,
                                       BOOLEAN_P, const char *));


const nhsym cp437toUnicode[256] = {
    0x0020, 0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022,
    0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C,
    0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8,
    0x2191, 0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC,
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
    0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
    0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
    0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
    0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
    0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x2302,
    0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7,
    0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
    0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9,
    0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192,
    0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba,
    0x00bf, 0x2310, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
    0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
    0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
    0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
    0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
    0x03b1, 0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x00b5, 0x03c4,
    0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
    0x2261, 0x00b1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00f7, 0x2248,
    0x00b0, 0x2219, 0x00b7, 0x221a, 0x207f, 0x00b2, 0x25a0, 0x00a0
};


/* is 'c' a digit? */
boolean
digit(c)
char c;
{
    return !!('0' <= c && c <= '9');
}

/* is 'c' a letter?  note: '@' classed as letter */
boolean
letter(c)
char c;
{
    return (boolean) ('@' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

/* force 'c' into uppercase */
char
highc(c)
char c;
{
    return (char) (('a' <= c && c <= 'z') ? (c & ~040) : c);
}

/* force 'c' into lowercase */
char
lowc(c)
char c;
{
    return (char) (('A' <= c && c <= 'Z') ? (c | 040) : c);
}

/* convert a string into all lowercase */
char *
lcase(s)
char *s;
{
    register char *p;

    for (p = s; *p; p++)
        if ('A' <= *p && *p <= 'Z')
            *p |= 040;
    return s;
}

/* convert a string into all uppercase */
char *
ucase(s)
char *s;
{
    register char *p;

    for (p = s; *p; p++)
        if ('a' <= *p && *p <= 'z')
            *p &= ~040;
    return s;
}

/* convert first character of a string to uppercase */
char *
upstart(s)
char *s;
{
    if (s)
        *s = highc(*s);
    return s;
}

/* remove excess whitespace from a string buffer (in place) */
char *
mungspaces(bp)
char *bp;
{
    register char c, *p, *p2;
    boolean was_space = TRUE;

    for (p = p2 = bp; (c = *p) != '\0'; p++) {
        if (c == '\n')
            break; /* treat newline the same as end-of-string */
        if (c == '\t')
            c = ' ';
        if (c != ' ' || !was_space)
            *p2++ = c;
        was_space = (c == ' ');
    }
    if (was_space && p2 > bp)
        p2--;
    *p2 = '\0';
    return bp;
}

/* skip leading whitespace; remove trailing whitespace, in place */
char *
trimspaces(txt)
char *txt;
{
    char *end;

    /* leading whitespace will remain in the buffer */
    while (*txt == ' ' || *txt == '\t')
        txt++;
    end = eos(txt);
    while (--end >= txt && (*end == ' ' || *end == '\t'))
        *end = '\0';

    return txt;
}

/* remove \n from end of line; remove \r too if one is there */
char *
strip_newline(str)
char *str;
{
    char *p = rindex(str, '\n');

    if (p) {
        if (p > str && *(p - 1) == '\r')
            --p;
        *p = '\0';
    }
    return str;
}

/* return the end of a string (pointing at '\0') */
char *
eos(s)
register char *s;
{
    while (*s)
        s++; /* s += strlen(s); */
    return s;
}

/* determine whether 'str' ends in 'chkstr' */
boolean
str_end_is(str, chkstr)
const char *str, *chkstr;
{
    int clen = (int) strlen(chkstr);

    if ((int) strlen(str) >= clen)
        return (boolean) (!strncmp(eos((char *) str) - clen, chkstr, clen));
    return FALSE;
}

/* append a character to a string (in place): strcat(s, {c,'\0'}); */
char *
strkitten(s, c)
char *s;
char c;
{
    char *p = eos(s);

    *p++ = c;
    *p = '\0';
    return s;
}


char*
strcpy_capitalized_for_title(dest_buffer, source)
char* dest_buffer;
const char* source;
{
    if (!dest_buffer || !source)
        return dest_buffer;

    Strcpy(dest_buffer, source);

    char* bp;
    for (bp = dest_buffer; *bp; bp++)
    {
        if ((bp == dest_buffer 
            || ((*(bp - 1) == ' ' || *(bp - 1) == '-') &&
                strncmp(bp, "the ", 4) && strncmp(bp, "an ", 3) && strncmp(bp, "a ", 2) && 
                strncmp(bp, "of ", 3) && strncmp(bp, "and ", 4) && strncmp(bp, "or ", 3) &&
                strncmp(bp, "at ", 3) && strncmp(bp, "on ", 3) && strncmp(bp, "in ", 3) && 
                strncmp(bp, "to ", 3) && strncmp(bp, "into ", 5) && strncmp(bp, "from ", 5)
               )))
            *bp = highc(*bp);
    }

    return dest_buffer;
}

/* truncating string copy */
void
copynchars(dst, src, n)
char *dst;
const char *src;
int n;
{
    /* copies at most n characters, stopping sooner if terminator reached;
       treats newline as input terminator; unlike strncpy, always supplies
       '\0' terminator so dst must be able to hold at least n+1 characters */
    while (n > 0 && *src != '\0' && *src != '\n') {
        *dst++ = *src++;
        --n;
    }
    *dst = '\0';
}

/* convert char nc into oc's case; mostly used by strcasecpy */
char
chrcasecpy(oc, nc)
int oc, nc;
{
#if 0 /* this will be necessary if we switch to <ctype.h> */
    oc = (int) (unsigned char) oc;
    nc = (int) (unsigned char) nc;
#endif
    if ('a' <= oc && oc <= 'z') {
        /* old char is lower case; if new char is upper case, downcase it */
        if ('A' <= nc && nc <= 'Z')
            nc += 'a' - 'A'; /* lowc(nc) */
    } else if ('A' <= oc && oc <= 'Z') {
        /* old char is upper case; if new char is lower case, upcase it */
        if ('a' <= nc && nc <= 'z')
            nc += 'A' - 'a'; /* highc(nc) */
    }
    return (char) nc;
}

/* overwrite string, preserving old chars' case;
   for case-insensitive editions of makeplural() and makesingular();
   src might be shorter, same length, or longer than dst */
char *
strcasecpy(dst, src)
char *dst;
const char *src;
{
    char *result = dst;
    int ic, oc, dst_exhausted = 0;

    /* while dst has characters, replace each one with corresponding
       character from src, converting case in the process if they differ;
       once dst runs out, propagate the case of its last character to any
       remaining src; if dst starts empty, it must be a pointer to the
       tail of some other string because we examine the char at dst[-1] */
    while ((ic = (int) *src++) != '\0') {
        if (!dst_exhausted && !*dst)
            dst_exhausted = 1;
        oc = (int) *(dst - dst_exhausted);
        *dst++ = chrcasecpy(oc, ic);
    }
    *dst = '\0';
    return result;
}

/* return a name converted to possessive */
char *
s_suffix(s)
const char *s;
{
    Static char buf[BUFSZ];

    Strcpy(buf, s);
    if (!strcmpi(buf, "it")) /* it -> its */
        Strcat(buf, "s");
    else if (!strcmpi(buf, "you")) /* you -> your */
        Strcat(buf, "r");
    else if (*(eos(buf) - 1) == 's') /* Xs -> Xs' */
        Strcat(buf, "'");
    else /* X -> X's */
        Strcat(buf, "'s");
    return buf;
}

/* construct a gerund (a verb formed by appending "ing" to a noun) */
char *
ing_suffix(s)
const char *s;
{
    static const char vowel[] = "aeiouwy";
    static char buf[BUFSZ];
    char onoff[10];
    char *p;

    Strcpy(buf, s);
    p = eos(buf);
    onoff[0] = *p = *(p + 1) = '\0';
    if ((p >= &buf[3] && !strcmpi(p - 3, " on"))
        || (p >= &buf[4] && !strcmpi(p - 4, " off"))
        || (p >= &buf[5] && !strcmpi(p - 5, " with"))) {
        p = rindex(buf, ' ');
        Strcpy(onoff, p);
        *p = '\0';
    }
    if (p >= &buf[3] && !index(vowel, *(p - 1))
        && index(vowel, *(p - 2)) && !index(vowel, *(p - 3))) {
        /* tip -> tipp + ing */
        *p = *(p - 1);
        *(p + 1) = '\0';
    } else if (p >= &buf[2] && !strcmpi(p - 2, "ie")) { /* vie -> vy + ing */
        *(p - 2) = 'y';
        *(p - 1) = '\0';
    } else if (p >= &buf[1] && *(p - 1) == 'e') /* grease -> greas + ing */
        *(p - 1) = '\0';
    Strcat(buf, "ing");
    if (onoff[0])
        Strcat(buf, onoff);
    return buf;
}

/* trivial text encryption routine (see makedefs) */
char *
xcrypt(str, buf)
const char *str;
char *buf;
{
    register const char *p;
    register char *q;
    register int bitmask;

    for (bitmask = 1, p = str, q = buf; *p; q++) {
        *q = *p++;
        if (*q & (32 | 64))
            *q ^= bitmask;
        if ((bitmask <<= 1) >= 32)
            bitmask = 1;
    }
    *q = '\0';
    return buf;
}

/* is a string entirely whitespace? */
boolean
onlyspace(s)
const char *s;
{
    for (; *s; s++)
        if (*s != ' ' && *s != '\t')
            return FALSE;
    return TRUE;
}

/* expand tabs into proper number of spaces */
char *
tabexpand(sbuf)
char *sbuf;
{
    char buf[BUFSZ];
    register char *bp, *s = sbuf;
    register int idx;

    if (!*s)
        return sbuf;
    /* warning: no bounds checking performed */
    for (bp = buf, idx = 0; *s; s++)
        if (*s == '\t') {
            do
                *bp++ = ' ';
            while (++idx % 8);
        } else {
            *bp++ = *s;
            idx++;
        }
    *bp = 0;
    return strcpy(sbuf, buf);
}

#define VISCTRL_NBUF 5
/* make a displayable string from a character */
char *
visctrl(c)
char c;
{
    Static char visctrl_bufs[VISCTRL_NBUF][5];
    static int nbuf = 0;
    register int i = 0;
    char *ccc = visctrl_bufs[nbuf];
    nbuf = (nbuf + 1) % VISCTRL_NBUF;

    if ((uchar) c & 0200) {
        ccc[i++] = 'M';
        ccc[i++] = '-';
    }
    c &= 0177;
    if (c < 040) {
        ccc[i++] = '^';
        ccc[i++] = c | 0100; /* letter */
    } else if (c == 0177) {
        ccc[i++] = '^';
        ccc[i++] = c & ~0100; /* '?' */
    } else {
        ccc[i++] = c; /* printable character */
    }
    ccc[i] = '\0';
    return ccc;
}

/* strip all the chars in stuff_to_strip from orig */
/* caller is responsible for ensuring that bp is a
   valid pointer to a BUFSZ buffer */
char *
stripchars(bp, stuff_to_strip, orig)
char *bp;
const char *stuff_to_strip, *orig;
{
    int i = 0;
    char *s = bp;

    if (s) {
        while (*orig && i < (BUFSZ - 1)) {
            if (!index(stuff_to_strip, *orig)) {
                *s++ = *orig;
                i++;
            }
            orig++;
        }
        *s = '\0';
    } else
        impossible("no output buf in stripchars");
    return bp;
}

/* substitute a word or phrase in a string (in place) */
/* caller is responsible for ensuring that bp points to big enough buffer */
char *
strsubst(bp, orig, replacement)
char *bp;
const char *orig, *replacement;
{
    char *found, buf[BUFSZ];

    if (bp) {
        /* [this could be replaced by strNsubst(bp, orig, replacement, 1)] */
        found = strstr(bp, orig);
        if (found) {
            Strcpy(buf, found + strlen(orig));
            Strcpy(found, replacement);
            Strcat(bp, buf);
        }
    }
    return bp;
}

/* substitute the Nth occurrence of a substring within a string (in place);
   if N is 0, substitute all occurrences; returns the number of subsitutions;
   maximum output length is BUFSZ (BUFSZ-1 chars + terminating '\0') */
int
strNsubst(inoutbuf, orig, replacement, n)
char *inoutbuf; /* current string, and result buffer */
const char *orig, /* old substring; if "" then insert in front of Nth char */
           *replacement; /* new substring; if "" then delete old substring */
int n; /* which occurrence to replace; 0 => all */
{
    char *bp, *op, workbuf[BUFSZ];
    const char *rp;
    size_t len = strlen(orig);
    int ocount = 0, /* number of times 'orig' has been matched */
        rcount = 0; /* number of subsitutions made */

    for (bp = inoutbuf, op = workbuf; *bp && op < &workbuf[BUFSZ - 1]; ) {
        if ((!len || !strncmp(bp, orig, len)) && (++ocount == n || n == 0)) {
            /* Nth match found */
            for (rp = replacement; *rp && op < &workbuf[BUFSZ - 1]; )
                *op++ = *rp++;
            ++rcount;
            if (len) {
                bp += len; /* skip 'orig' */
                continue;
            }
        }
        /* no match (or len==0) so retain current character */
        *op++ = *bp++;
    }
    if (!len && n == ocount + 1) {
        /* special case: orig=="" (!len) and n==strlen(inoutbuf)+1,
           insert in front of terminator (in other words, append);
           [when orig=="", ocount will have been incremented once for
           each input char] */
        for (rp = replacement; *rp && op < &workbuf[BUFSZ - 1]; )
            *op++ = *rp++;
        ++rcount;
    }
    if (rcount) {
        *op = '\0';
        Strcpy(inoutbuf, workbuf);
    }
    return rcount;
}

/* return the ordinal suffix of a number */
const char *
ordin(n)
int n;               /* note: should be non-negative */
{
    register int dd = n % 10;

    return (dd == 0 || dd > 3 || (n % 100) / 10 == 1) ? "th"
               : (dd == 1) ? "st" : (dd == 2) ? "nd" : "rd";
}

/* make a signed digit string from a number */
char *
sitoa(n)
int n;
{
    Static char buf[13];

    Sprintf(buf, (n < 0) ? "%d" : "+%d", n);
    return buf;
}

/* return the sign of a number: -1, 0, or 1 */
int
sgn(n)
int n;
{
    return (n < 0) ? -1 : (n != 0);
}

/* calculate x/y, rounding as appropriate */
int
rounddiv(x, y)
int x;
int y;
{
    int r, m;
    int divsgn = 1;

    if (y == 0)
    {
        panic("division by zero in rounddiv");
        return 0;
    }
    else if (y < 0) {
        divsgn = -divsgn;
        y = -y;
    }
    if (x < 0) {
        divsgn = -divsgn;
        x = -x;
    }
    r = x / y;
    m = x % y;
    if (2 * m >= y)
        r++;

    return divsgn * r;
}

/* distance between two points, in moves */
int
distmin(x0, y0, x1, y1)
int x0, y0, x1, y1;
{
    register int dx = x0 - x1, dy = y0 - y1;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    /*  The minimum number of moves to get from (x0,y0) to (x1,y1) is the
     *  larger of the [absolute value of the] two deltas.
     */
    return (dx < dy) ? dy : dx;
}

/* square of euclidean distance between pair of pts */
int
dist2(x0, y0, x1, y1)
int x0, y0, x1, y1;
{
    register int dx = x0 - x1, dy = y0 - y1;

    return dx * dx + dy * dy;
}

/* integer square root function without using floating point */
int
isqrt(val)
int val;
{
    int rt = 0;
    int odd = 1;
    /*
     * This could be replaced by a faster algorithm, but has not been because:
     * + the simple algorithm is easy to read;
     * + this algorithm does not require 64-bit support;
     * + in current usage, the values passed to isqrt() are not really that
     *   large, so the performance difference is negligible;
     * + isqrt() is used in only few places, which are not bottle-necks.
     */
    while (val >= odd) {
        val = val - odd;
        odd = odd + 2;
        rt = rt + 1;
    }
    return rt;
}

/* are two points lined up (on a straight line)? */
boolean
online2(x0, y0, x1, y1)
int x0, y0, x1, y1;
{
    int dx = x0 - x1, dy = y0 - y1;
    /*  If either delta is zero then they're on an orthogonal line,
     *  else if the deltas are equal (signs ignored) they're on a diagonal.
     */
    return (boolean) (!dy || !dx || dy == dx || dy == -dx);
}

/* guts of pmatch(), pmatchi(), and pmatchz();
   match a string against a pattern */
STATIC_OVL boolean
pmatch_internal(patrn, strng, ci, sk)
const char *patrn, *strng;
boolean ci;     /* True => case-insensitive, False => case-sensitive */
const char *sk; /* set of characters to skip */
{
    char s, p;
    /*
     *  Simple pattern matcher:  '*' matches 0 or more characters, '?' matches
     *  any single character.  Returns TRUE if 'strng' matches 'patrn'.
     */
pmatch_top:
    if (!sk) {
        s = *strng++;
        p = *patrn++; /* get next chars and pre-advance */
    } else {
        /* fuzzy match variant of pmatch; particular characters are ignored */
        do {
            s = *strng++;
        } while (index(sk, s));
        do {
            p = *patrn++;
        } while (index(sk, p));
    }
    if (!p)                           /* end of pattern */
        return (boolean) (s == '\0'); /* matches iff end of string too */
    else if (p == '*')                /* wildcard reached */
        return (boolean) ((!*patrn
                           || pmatch_internal(patrn, strng - 1, ci, sk))
                          ? TRUE
                          : s ? pmatch_internal(patrn - 1, strng, ci, sk)
                              : FALSE);
    else if ((ci ? lowc(p) != lowc(s) : p != s) /* check single character */
             && (p != '?' || !s))               /* & single-char wildcard */
        return FALSE;                           /* doesn't match */
    else                 /* return pmatch_internal(patrn, strng, ci, sk); */
        goto pmatch_top; /* optimize tail recursion */
}

/* case-sensitive wildcard match */
boolean
pmatch(patrn, strng)
const char *patrn, *strng;
{
    return pmatch_internal(patrn, strng, FALSE, (const char *) 0);
}

/* case-insensitive wildcard match */
boolean
pmatchi(patrn, strng)
const char *patrn, *strng;
{
    return pmatch_internal(patrn, strng, TRUE, (const char *) 0);
}

/* case-insensitive wildcard fuzzymatch */
boolean
pmatchz(patrn, strng)
const char *patrn, *strng;
{
    /* ignore spaces, tabs (just in case), dashes, and underscores */
    static const char fuzzychars[] = " \t-_";

    return pmatch_internal(patrn, strng, TRUE, fuzzychars);
}

#ifndef STRNCMPI
/* case insensitive counted string comparison */
int
strncmpi(s1, s2, n) /*{ aka strncasecmp }*/
register const char *s1, *s2;
register size_t n;
{
    register char t1, t2;
    if (!n)
        return 0;

    while (n--) {
        if (!*s2)
            return (*s1 != 0); /* s1 >= s2 */
        else if (!*s1)
            return -1; /* s1  < s2 */
        t1 = lowc(*s1++);
        t2 = lowc(*s2++);
        if (t1 != t2)
            return (t1 > t2) ? 1 : -1;
    }
    return 0; /* s1 == s2 */
}
#endif /* STRNCMPI */

#ifndef STRSTRI
/* case insensitive substring search */
char *
strstri(str, sub)
const char *str;
const char *sub;
{
    register const char *s1, *s2;
    register int i, k;
#define TABSIZ 0x20                  /* 0x40 would be case-sensitive */
    char tstr[TABSIZ], tsub[TABSIZ]; /* nibble count tables */
#if 0
    assert( (TABSIZ & ~(TABSIZ-1)) == TABSIZ ); /* must be exact power of 2 */
    assert( &lowc != 0 );                       /* can't be unsafe macro */
#endif

    /* special case: empty substring */
    if (!*sub)
        return (char *) str;

    /* do some useful work while determining relative lengths */
    for (i = 0; i < TABSIZ; i++)
        tstr[i] = tsub[i] = 0; /* init */
    for (k = 0, s1 = str; *s1; k++)
        tstr[*s1++ & (TABSIZ - 1)]++;
    for (s2 = sub; *s2; --k)
        tsub[*s2++ & (TABSIZ - 1)]++;

    /* evaluate the info we've collected */
    if (k < 0)
        return (char *) 0;       /* sub longer than str, so can't match */
    for (i = 0; i < TABSIZ; i++) /* does sub have more 'x's than str? */
        if (tsub[i] > tstr[i])
            return (char *) 0; /* match not possible */

    /* now actually compare the substring repeatedly to parts of the string */
    for (i = 0; i <= k; i++) {
        s1 = &str[i];
        s2 = sub;
        while (lowc(*s1++) == lowc(*s2++))
            if (!*s2)
                return (char *) &str[i]; /* full match */
    }
    return (char *) 0; /* not found */
}
#endif /* STRSTRI */

/* compare two strings for equality, ignoring the presence of specified
   characters (typically whitespace) and possibly ignoring case */
boolean
fuzzymatch(s1, s2, ignore_chars, caseblind)
const char *s1, *s2;
const char *ignore_chars;
boolean caseblind;
{
    register char c1, c2;

    do {
        while ((c1 = *s1++) != '\0' && index(ignore_chars, c1) != 0)
            continue;
        while ((c2 = *s2++) != '\0' && index(ignore_chars, c2) != 0)
            continue;
        if (!c1 || !c2)
            break; /* stop when end of either string is reached */

        if (caseblind) {
            c1 = lowc(c1);
            c2 = lowc(c2);
        }
    } while (c1 == c2);

    /* match occurs only when the end of both strings has been reached */
    return (boolean) (!c1 && !c2);
}

/*
 * Time routines
 *
 * The time is used for:
 *  - seed for rand()
 *  - year on tombstone and yyyymmdd in record file
 *  - phase of the moon (various monsters react to NEW_MOON or FULL_MOON)
 *  - night and midnight (the undead are dangerous at midnight)
 *  - determination of what files are "very old"
 */

/* TIME_type: type of the argument to time(); we actually use &(time_t) */
#if defined(BSD) && !defined(POSIX_TYPES)
#define TIME_type int64_t *
#else
#define TIME_type time_t *
#endif
/* LOCALTIME_type: type of the argument to localtime() */
#if (defined(ULTRIX) && !(defined(ULTRIX_PROTO) || defined(NHSTDC))) \
    || (defined(BSD) && !defined(POSIX_TYPES))
#define LOCALTIME_type int64_t *
#else
#define LOCALTIME_type time_t *
#endif

#if defined(AMIGA) && !defined(AZTEC_C) && !defined(__SASC_60) \
    && !defined(_DCC) && !defined(__GNUC__)
extern struct tm *FDECL(localtime, (time_t *));
#endif
STATIC_DCL struct tm *NDECL(getlt);

/* Sets the seed for the random number generator */
#ifdef USE_ISAAC64

STATIC_OVL void
set_random(seed, fn)
uint64_t seed;
int FDECL((*fn), (int));
{
    init_isaac64(seed, fn);
}

#else /* USE_ISAAC64 */

/*ARGSUSED*/
STATIC_OVL void
set_random(seed, fn)
uint64_t seed;
int FDECL((*fn), (int)) UNUSED;
{
    /* the types are different enough here that sweeping the different
     * routine names into one via #defines is even more confusing
     */
# ifdef RANDOM /* srandom() from sys/share/random.c */
    srandom((unsigned int) seed);
# else
#  if (defined(__APPLE__) || defined(BSD) || defined(LINUX) || defined(ULTRIX) \
    || defined(CYGWIN32)) /* system srandom() */
#   if defined(BSD) && !defined(POSIX_TYPES) && defined(SUNOS4)
    (void)
#   endif
        srandom((int) seed);
#  else
#   ifdef UNIX /* system srand48() */
    srand48((int64_t) seed);
#   else       /* poor quality system routine */
    srand((int) seed);
#   endif
#  endif
# endif
}

#endif /* USE_ISAAC64 */

/* An appropriate version of this must always be provided in
   port-specific code somewhere. It returns a number suitable
   as seed for the random number generator */
extern uint64_t NDECL(sys_random_seed);

/*
 * Initializes the random number generator.
 * Only call once.
 */
void
init_random(fn)
int FDECL((*fn), (int));
{
    set_random(sys_random_seed(), fn);
}

/* Reshuffles the random number generator. */
void
reseed_random(fn)
int FDECL((*fn), (int));
{
   /* only reseed if we are certain that the seed generation is unguessable
    * by the players. */
    if (has_strong_rngseed)
        init_random(fn);
}

time_t
getnow(VOID_ARGS)
{
    time_t datetime = 0;

    (void) time((TIME_type) &datetime);
    return datetime;
}

time_t
convert2UTC(raw_time)
time_t raw_time;
{
    struct tm* utctm = gmtime((TIME_type)&raw_time);
    return mktime(utctm);
}

time_t
getUTC(VOID_ARGS)
{
    return convert2UTC(getnow());
}

STATIC_OVL struct tm *
getlt(VOID_ARGS)
{
    time_t date = getnow();
    return localtime((LOCALTIME_type) &date);
}

int
getyear(VOID_ARGS)
{
    return (1900 + getlt()->tm_year);
}

#if 0
/* This routine is no longer used since in 20YY it yields "1YYmmdd". */
char *
yymmdd(date)
time_t date;
{
    Static char datestr[10];
    struct tm *lt;

    if (date == 0)
        lt = getlt();
    else
        lt = localtime((LOCALTIME_type) &date);

    Sprintf(datestr, "%02d%02d%02d",
            lt->tm_year, lt->tm_mon + 1, lt->tm_mday);
    return datestr;
}
#endif

int64_t
yyyymmdd(date)
time_t date;
{
    int64_t datenum;
    struct tm *lt;

    if (date == 0)
        lt = getlt();
    else
        lt = localtime((LOCALTIME_type) &date);

    /* just in case somebody's localtime supplies (year % 100)
       rather than the expected (year - 1900) */
    if (lt->tm_year < 70)
        datenum = (int64_t) lt->tm_year + 2000L;
    else
        datenum = (int64_t) lt->tm_year + 1900L;
    /* yyyy --> yyyymm */
    datenum = datenum * 100L + (int64_t) (lt->tm_mon + 1);
    /* yyyymm --> yyyymmdd */
    datenum = datenum * 100L + (int64_t) lt->tm_mday;
    return datenum;
}

int64_t
hhmmss(date)
time_t date;
{
    int64_t timenum;
    struct tm *lt;

    if (date == 0)
        lt = getlt();
    else
        lt = localtime((LOCALTIME_type) &date);

    timenum = lt->tm_hour * 10000L + lt->tm_min * 100L + lt->tm_sec;
    return timenum;
}

char *
yyyymmddhhmmss(date)
time_t date;
{
    int64_t datenum;
    static char datestr[15];
    struct tm *lt;

    if (date == 0)
        lt = getlt();
    else
#if (defined(ULTRIX) && !(defined(ULTRIX_PROTO) || defined(NHSTDC))) \
    || defined(BSD)
        lt = localtime((int64_t *) (&date));
#else
        lt = localtime(&date);
#endif
    /* just in case somebody's localtime supplies (year % 100)
       rather than the expected (year - 1900) */
    if (lt->tm_year < 70)
        datenum = (int64_t) lt->tm_year + 2000L;
    else
        datenum = (int64_t) lt->tm_year + 1900L;
    Sprintf(datestr, "%04lld%02d%02d%02d%02d%02d", (long long)datenum, lt->tm_mon + 1,
            lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    debugpline1("yyyymmddhhmmss() produced date string %s", datestr);
    return datestr;
}

void
print_realtime(buf, realtime)
char* buf;
int64_t realtime;
{
    if (!buf)
        return;

    int64_t hours = realtime / (60 * 60);
    int64_t minutes = (realtime % (60 * 60)) / 60;
    int64_t seconds = realtime - hours * 60 * 60 - minutes * 60;

    char hourbuf[BUFSZ] = "", minutebuf[BUFSZ] = "";
    if (hours > 0)
        Sprintf(hourbuf, "%lld hour%s", (long long)hours, plur(hours));
    if (minutes > 0)
        Sprintf(minutebuf, "%s%lld minute%s", *hourbuf ? ", " : "", (long long)minutes, plur(minutes));

    Sprintf(buf, "%s%s%s%lld second%s", hourbuf, minutebuf, *hourbuf || *minutebuf ? " and " : "", (long long)seconds, plur(seconds));
}

time_t
time_from_yyyymmddhhmmss(buf)
char *buf;
{
    int k;
    time_t timeresult = (time_t) 0;
    struct tm t, *lt;
    char *g, *p, y[5], mo[3], md[3], h[3], mi[3], s[3];

    if (buf && strlen(buf) == 14) {
        g = buf;
        p = y; /* year */
        for (k = 0; k < 4; ++k)
            *p++ = *g++;
        *p = '\0';
        p = mo; /* month */
        for (k = 0; k < 2; ++k)
            *p++ = *g++;
        *p = '\0';
        p = md; /* day */
        for (k = 0; k < 2; ++k)
            *p++ = *g++;
        *p = '\0';
        p = h; /* hour */
        for (k = 0; k < 2; ++k)
            *p++ = *g++;
        *p = '\0';
        p = mi; /* minutes */
        for (k = 0; k < 2; ++k)
            *p++ = *g++;
        *p = '\0';
        p = s; /* seconds */
        for (k = 0; k < 2; ++k)
            *p++ = *g++;
        *p = '\0';
        lt = getlt();
        if (lt) {
            t = *lt;
            t.tm_year = atoi(y) - 1900;
            t.tm_mon = atoi(mo) - 1;
            t.tm_mday = atoi(md);
            t.tm_hour = atoi(h);
            t.tm_min = atoi(mi);
            t.tm_sec = atoi(s);
            timeresult = mktime(&t);
        }
        if ((int) timeresult == -1)
            debugpline1("time_from_yyyymmddhhmmss(%s) would have returned -1",
                        buf ? buf : "");
        else
            return timeresult;
    }
    return (time_t) 0;
}

/*
 * moon period = 29.53058 days ~= 30, year = 365.2422 days
 * days moon phase advances on first day of year compared to preceding year
 *      = 365.2422 - 12*29.53058 ~= 11
 * years in Metonic cycle (time until same phases fall on the same days of
 *      the month) = 18.6 ~= 19
 * moon phase on first day of year (epact) ~= (11*(year%19) + 29) % 30
 *      (29 as initial condition)
 * current phase in days = first day phase + days elapsed in year
 * 6 moons ~= 177 days
 * 177 ~= 8 reported phases * 22
 * + 11/22 for rounding
 */
int
phase_of_the_moon() /* 0-7, with 0: new, 4: full */
{
    register struct tm *lt = getlt();
    register int epact, diy, goldn;

    diy = lt->tm_yday;
    goldn = (lt->tm_year % 19) + 1;
    epact = (11 * goldn + 18) % 30;
    if ((epact == 25 && goldn > 11) || epact == 24)
        epact++;

    return ((((((diy + epact) * 6) + 11) % 177) / 22) & 7);
}

boolean
friday_13th()
{
    register struct tm *lt = getlt();

    /* tm_wday (day of week; 0==Sunday) == 5 => Friday */
    return (boolean) (lt->tm_wday == 5 && lt->tm_mday == 13);
}

int
night()
{
    register int hour = getlt()->tm_hour;

    return (hour < 6 || hour > 21);
}

int
midnight()
{
    return (getlt()->tm_hour == 0);
}

/* strbuf_init() initializes strbuf state for use */
void
strbuf_init(strbuf)
strbuf_t *strbuf;
{
    strbuf->str = NULL;
    strbuf->len = 0;
}

/* strbuf_append() appends given str to strbuf->str */
void
strbuf_append(strbuf, str)
strbuf_t *strbuf;
const char *str;
{
    if (!strbuf || !strbuf->str)
        return;

    size_t len = strlen(str) + 1;

    strbuf_reserve(strbuf,
                   len + (strbuf->str ? strlen(strbuf->str) : 0));
    Strcat(strbuf->str, str);
}

/* strbuf_reserve() ensure strbuf->str has storage for len characters */
void
strbuf_reserve(strbuf, len)
strbuf_t *strbuf;
size_t len;
{
    if (strbuf->str == NULL) 
    {
        strbuf->str = strbuf->buf;
        strbuf->str[0] = '\0';
        strbuf->len = sizeof strbuf->buf;
    }

    if (len > strbuf->len) 
    {
        char *oldbuf = strbuf->str;

        strbuf->len = len + sizeof strbuf->buf;
        strbuf->str = (char *) alloc(strbuf->len);
        Strcpy(strbuf->str, oldbuf);
        if (oldbuf != strbuf->buf)
            free((genericptr_t) oldbuf);
    }
}

/* strbuf_empty() frees allocated memory and set strbuf to initial state */
void
strbuf_empty(strbuf)
strbuf_t *strbuf;
{
    if (strbuf->str != NULL && strbuf->str != strbuf->buf)
        free((genericptr_t) strbuf->str);
    strbuf_init(strbuf);
}

/* strbuf_nl_to_crlf() converts all occurences of \n to \r\n */
void
strbuf_nl_to_crlf(strbuf)
strbuf_t *strbuf;
{
    if (strbuf->str) {
        size_t len = strlen(strbuf->str);
        int count = 0;
        char *cp = strbuf->str;

        while (*cp)
            if (*cp++ == '\n')
                count++;
        if (count) {
            strbuf_reserve(strbuf, len + count + 1);
            for (cp = strbuf->str + len + (size_t)count; count; --cp)
                if ((*cp = cp[-count]) == '\n') {
                    *--cp = '\r';
                    --count;
                }
        }
    }
}

#if 0
void
convertUTF8toCharUnicode(buf, bufsize)
char* buf;
size_t bufsize;
{
    if (!buf || !*buf)
        return;

    char* bp, * wp;
    char copybuf[BUFSZ * 8] = "";
    wp = copybuf;
    uchar uc, uc2, uc3, uc4;
    unsigned long unicode = 0, byte2bits, byte1bits, byte3bits, byte4bits;
    char unicodechar = 0;

    for (bp = buf; *bp && bp < bp + bufsize && wp < copybuf + sizeof(copybuf); bp++)
    {
        uc = (uchar)(*bp);
        if (uc >= 0x80)
        {
            if ((uc & 0xE0) == 0xC0)
            {
                byte1bits = (uc & 0x1F) << 6;
                bp++;
                uc2 = (uchar)(*bp);
                byte2bits = (uc2 & 0x3F);
                unicode = byte1bits | byte2bits;
            }
            else if ((uc & 0xF0) == 0xE0)
            {
                byte1bits = (uc & 0x0F) << 12;
                bp++;
                uc2 = (uchar)(*bp);
                byte2bits = (uc2 & 0x3F) << 6;
                bp++;
                uc3 = (uchar)(*bp);
                byte3bits = (uc3 & 0x3F);
                unicode = byte1bits | byte2bits | byte3bits;
            }
            else if ((uc & 0xF8) == 0xF0)
            {
                byte1bits = (uc & 0x07) << 18;
                bp++;
                uc2 = (uchar)(*bp);
                byte2bits = (uc2 & 0x3F) << 12;
                bp++;
                uc3 = (uchar)(*bp);
                byte3bits = (uc3 & 0x3F) << 6;
                bp++;
                uc4 = (uchar)(*bp);
                byte4bits = (uc4 & 0x3F);
                unicode = byte1bits | byte2bits | byte3bits | byte4bits;
            }
            else
            {
                unicode = '?';
            }

            unicodechar = '?';
            if (unicode != '?' && unicode <= 0xFF)
            {
                unicodechar = (char)unicode;
            }
            *wp = unicodechar;
            wp++;

        }
        else
        {
            *wp = *bp;
            wp++;
        }
    }

    if (wp < copybuf + sizeof(copybuf))
        *wp = '\0';
    else
        copybuf[sizeof(copybuf) - 1] = '\0';

    Strcpy(buf, copybuf);
}
#endif

void
copyUTF8toCP437(cp437destbuf, cp437destbufsize, utf8sourcebuf, utf8sourcebufsize)
char* cp437destbuf,* utf8sourcebuf;
size_t cp437destbufsize, utf8sourcebufsize;
{
    if (!cp437destbuf || !utf8sourcebuf || !cp437destbufsize || !utf8sourcebufsize)
        return;

    char* bp, * wp;
    wp = cp437destbuf;
    uchar uc, uc2, uc3, uc4;
    unsigned long unicode = 0, byte2bits, byte1bits, byte3bits, byte4bits;
    char cp437char = 0;
    //boolean was_unicode = FALSE;

    for (bp = utf8sourcebuf; *bp && bp < utf8sourcebuf + utf8sourcebufsize && wp < cp437destbuf + cp437destbufsize; bp++)
    {
        uc = (uchar)(*bp);
        if (uc >= 0x80)
        {
#if 0
            if (uc == 0xc3) /* A replaceble character */
            {
                bp++;
                uc = (uchar)(*bp);
                if (0xa5 == uc)
                    *wp = (char)134;
                else if (0xa4 == uc)
                    *wp = (char)132;
                else if (0xb6 == uc)
                    *wp = (char)148;
                else if (0x85 == uc)
                    *wp = (char)143;
                else if (0x84 == uc)
                    *wp = (char)142;
                else if (0x96 == uc)
                    *wp = (char)153;
                else
                    *wp = '?';

                wp++;
            }
            else
            {
                if (!was_unicode)
                {
                    *wp = '?';
                    wp++;
                }
            }

            was_unicode = TRUE;
#endif
            if ((uc & 0xE0) == 0xC0)
            {
                byte1bits = (uc & 0x1F) << 6;
                bp++;
                uc2 = (uchar)(*bp);
                byte2bits = (uc2 & 0x3F);
                unicode = byte1bits | byte2bits;
            }
            else if ((uc & 0xF0) == 0xE0)
            {
                byte1bits = (uc & 0x0F) << 12;
                bp++;
                uc2 = (uchar)(*bp);
                byte2bits = (uc2 & 0x3F) << 6;
                bp++;
                uc3 = (uchar)(*bp);
                byte3bits = (uc3 & 0x3F);
                unicode = byte1bits | byte2bits | byte3bits;
            }
            else if ((uc & 0xF8) == 0xF0)
            {
                byte1bits = (uc & 0x07) << 18;
                bp++;
                uc2 = (uchar)(*bp);
                byte2bits = (uc2 & 0x3F) << 12;
                bp++;
                uc3 = (uchar)(*bp);
                byte3bits = (uc3 & 0x3F) << 6;
                bp++;
                uc4 = (uchar)(*bp);
                byte4bits = (uc4 & 0x3F);
                unicode = byte1bits | byte2bits | byte3bits | byte4bits;
            }
            else
            {
                unicode = '?';
            }

            cp437char = '?';
            if (unicode != '?' && unicode <= 0xFFFF)
            {
                nhsym wch = (nhsym)unicode;
                int i;
                for (i = 128; i < 256; i++)
                {
                    if (cp437toUnicode[i] == wch)
                    {
                        cp437char = (char)i;
                        break;
                    }
                }
            }
            *wp = cp437char;
            wp++;

        }
        else
        {
            *wp = *bp;
            wp++;
            //was_unicode = FALSE;
        }
    }

    if (wp < cp437destbuf + cp437destbufsize)
        *wp = '\0';
    else
        cp437destbuf[cp437destbufsize - 1] = '\0';

}

void
convertUTF8toCP437(buf, bufsize)
char* buf;
size_t bufsize;
{
    if (!buf || !*buf || !bufsize)
        return;

    char* copybuf = (char*)alloc(bufsize);
    if (!copybuf)
        return;

    copyUTF8toCP437(copybuf, bufsize, buf, bufsize);
    Strcpy(buf, copybuf);
    free(copybuf);
}

char
unicode_to_CP437(ch)
nhsym ch;
{
    int i;

    if (ch < 32 || (ch < 256 && cp437toUnicode[ch] == ch))
        return (char)ch;

    for (i = 32; i < 256; i++)
        if (cp437toUnicode[i] == ch)
            return (char)i;

    return (char)ch;
}

int
converted_nh_poskey(x, y, mod)
int* x, * y, * mod;
{
    int c = nh_poskey(x, y, mod);

#ifdef UNIX
    if (c == '\033' && tty_arrow_key_support_enabled())
    {
        c = nhgetch();
        if (c == EOF || c == 0 || c == '\033')
        {
            c = '\033';
        }
        else if (c == 91)
        {
            c = nhgetch();
            switch (c)
            {
            case 0: /* Alt+[ */
                c = '['; //91
                c |= 0200;
                break;
            case 65:
                c = Cmd.move_N;
                break;
            case 66:
                c = Cmd.move_S;
                break;
            case 67:
                c = Cmd.move_E;
                break;
            case 68:
                c = Cmd.move_W;
                break;
            default:
                c = 0;
                break;
            }
        }
        else
        {
#ifdef ALTMETA
            if(iflags.altmeta)
                c = c | 0200;
#endif
        }
    }
#endif

    return c;
}


boolean
use_utf8_encoding()
{
    /* Windows ASCII GnollHack does not use UTF-8 encoding with Unicode */
#ifdef WIN32
    return ((flags.ibm2utf8 && SYMHANDLING(H_IBM)));
#else
    return ((flags.ibm2utf8 && SYMHANDLING(H_IBM)) || SYMHANDLING(H_UNICODE));
#endif
}



#if defined(UNIX) && !defined(GNH_MOBILE) 
extern int unix_is_stdin_empty(); /* From unixmain.c */
#endif

int
tty_arrow_key_support_enabled()
{
    return iflags.enablettyarrowkeys;
}

void
write_nhsym_utf8(buf_ptr, ch, is_CP437)
char** buf_ptr;
nhsym ch;
boolean is_CP437; /* if false, then ch is assumed to be unicode */
{
    if (!buf_ptr)
        return;

    char* buf = *buf_ptr;
    if (!buf)
        return;

    if (ch < 0 && ch >= -128)
        ch += 256; /* Assume this is a char of over 127 value */

    /* Convert cp437 to Unicode first, if need be */
    if (is_CP437 && ch >= 0 && ch < 256)
        ch = cp437toUnicode[ch];

    unsigned long c = (unsigned long)ch;
    if (c < 0x80) {
        *buf++ = (char)c;
    }
    else if (c < 0x800) {
        *buf++ = ((char)(0xC0 | (c >> 6)));
        *buf++ = ((char)(0x80 | (c & 0x3F)));
    }
    else if (c < 0x10000) {
        *buf++ = ((char)(0xE0 | (c >> 12)));
        *buf++ = ((char)(0x80 | (c >> 6 & 0x3F)));
        *buf++ = ((char)(0x80 | (c & 0x3F)));
    }
    else if (c < 0x200000) {
        *buf++ = ((char)(0xF0 | (c >> 18)));
        *buf++ = ((char)(0x80 | (c >> 12 & 0x3F)));
        *buf++ = ((char)(0x80 | (c >> 6 & 0x3F)));
        *buf++ = ((char)(0x80 | (c & 0x3F)));
    }
    *buf_ptr = buf;
}

void
write_text2buf_utf8(buf, bufsize, text)
char* buf;
size_t bufsize;
const char* text;
{
    nhsym sym;
    const char* tp = text;
    char* bp = buf;
    while (*tp && bp - buf < (int)(bufsize - 5))
    {
        sym = (nhsym)((uchar)*tp);
        write_nhsym_utf8(&bp, sym, TRUE); /* All text is internally stored as CP437 */
        tp++;
    }
    *bp = '\0';
}

void
write_CP437_to_buf_unicode(buf, bufsize, text)
char* buf;
size_t bufsize;
const char* text;
{
    int ch;
    const char* tp = text;
    char* bp = buf;
    while (*tp && bp - buf < (int)(bufsize - 1))
    {
        ch = (int)((uchar)*tp);
        *bp = ch < 32 ? ch : cp437toUnicode[ch] < 256 ? (char)cp437toUnicode[ch] : '?';
        bp++;
        tp++;
    }
    *bp = '\0';
}

void
write_gui_debuglog(str)
const char* str;
{
    if (!str || open_special_view == 0)
        return;

    struct special_view_info info = { 0 };
    info.viewtype = SPECIAL_VIEW_DEBUGLOG;
    info.text = str;
    (void)open_special_view(info);
}

int
hl_attridx_to_attrmask(idx)
int idx;
{
    switch (idx)
    {
    case HL_ATTCLR_DIM:     return (1 << ATR_DIM);
    case HL_ATTCLR_BLINK:    return (1 << ATR_BLINK);
    case HL_ATTCLR_ULINE:   return (1 << ATR_ULINE);
    case HL_ATTCLR_INVERSE:    return (1 << ATR_INVERSE);
    case HL_ATTCLR_BOLD:    return (1 << ATR_BOLD);
    }
    return 0;
}

int 
hl_attrmask_to_attridx(mask)
int mask;
{
    int attr = 0;
    if (mask & HL_DIM) attr |= (1 << ATR_DIM);
    if (mask & HL_BLINK) attr |= (1 << ATR_BLINK);
    if (mask & HL_ULINE) attr |= (1 << ATR_ULINE);
    if (mask & HL_INVERSE) attr |= (1 << ATR_INVERSE);
    if (mask & HL_BOLD) attr |= (1 << ATR_BOLD);
    return attr;
}

int
hl_attrmask_to_atr(mask)
int mask;
{
    if (mask & HL_BOLD) return ATR_BOLD;
    if (mask & HL_DIM) return ATR_DIM;
    if (mask & HL_BLINK) return ATR_BLINK;
    if (mask & HL_ULINE) return ATR_ULINE;
    if (mask & HL_INVERSE) return ATR_INVERSE;
    return 0;
}

int
hl_attridx_to_atr(idx)
int idx;
{
    switch (idx)
    {
    case HL_ATTCLR_DIM:     return ATR_DIM;
    case HL_ATTCLR_BLINK:    return ATR_BLINK;
    case HL_ATTCLR_ULINE:   return ATR_ULINE;
    case HL_ATTCLR_INVERSE:    return ATR_INVERSE;
    case HL_ATTCLR_BOLD:    return ATR_BOLD;
    }
    return 0;
}


/*hacklib.c*/
