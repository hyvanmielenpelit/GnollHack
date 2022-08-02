/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-13 */

/* GnollHack 4.0    rumors.c    $NHDT-Date: 1545132266 2018/12/18 11:24:26 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.34 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h"
#include "dlb.h"

/*      [note: this comment is fairly old, but still accurate for 3.1]
 * Rumors have been entirely rewritten to speed up the access.  This is
 * essential when working from floppies.  Using fseek() the way that's done
 * here means rumors following longer rumors are output more often than those
 * following shorter rumors.  Also, you may see the same rumor more than once
 * in a particular game (although the odds are highly against it), but
 * this also happens with real fortune cookies.  -dgk
 */

/*      3.6
 * The rumors file consists of a "do not edit" line, then a line containing
 * three sets of three counts (first two in decimal, third in hexadecimal).
 * The first set has the number of true rumors, the count in bytes for all
 * true rumors, and the file offset to the first one.  The second set has
 * the same group of numbers for the false rumors.  The third set has 0 for
 * count, 0 for size, and the file offset for end-of-file.  The offset of
 * the first true rumor plus the size of the true rumors matches the offset
 * of the first false rumor.  Likewise, the offset of the first false rumor
 * plus the size of the false rumors matches the offset for end-of-file.
 */

/*      3.1     [now obsolete for rumors but still accurate for oracles]
 * The rumors file consists of a "do not edit" line, a hexadecimal number
 * giving the number of bytes of useful/true rumors, followed by those
 * true rumors (one per line), followed by the useless/false/misleading/cute
 * rumors (also one per line).  Number of bytes of untrue rumors is derived
 * via fseek(EOF)+ftell().
 *
 * The oracles file consists of a "do not edit" comment, a decimal count N
 * and set of N+1 hexadecimal fseek offsets, followed by N multiple-line
 * records, separated by "---" lines.  The first oracle is a special case,
 * and placed there by 'makedefs'.
 */

STATIC_DCL void FDECL(init_rumors, (dlb *));
STATIC_DCL void FDECL(init_oracles, (dlb *));
STATIC_DCL void FDECL(couldnt_open_file, (const char *));

/* rumor size variables are signed so that value -1 can be used as a flag */
static long true_rumor_size = 0L, false_rumor_size;
/* rumor start offsets are unsigned because they're handled via %lx format */
static unsigned long true_rumor_start, false_rumor_start;
/* rumor end offsets are signed because they're compared with [dlb_]ftell() */
static long true_rumor_end, false_rumor_end;
/* oracles are handled differently from rumors... */
static int oracle_flg = 0; /* -1=>don't use, 0=>need init, 1=>init done */
static size_t oracle_cnt = 0;
static unsigned long *oracle_loc = 0;

STATIC_OVL void
init_rumors(fp)
dlb *fp;
{
    static const char rumors_header[] = "%d,%ld,%lx;%d,%ld,%lx;0,0,%lx\n";
    int true_count, false_count; /* in file but not used here */
    unsigned long eof_offset;
    char line[BUFSZ];

    (void) dlb_fgets(line, sizeof line, fp); /* skip "don't edit" comment */
    (void) dlb_fgets(line, sizeof line, fp);
    if (sscanf(line, rumors_header, &true_count, &true_rumor_size,
               &true_rumor_start, &false_count, &false_rumor_size,
               &false_rumor_start, &eof_offset) == 7
        && true_rumor_size > 0L
        && false_rumor_size > 0L) {
        true_rumor_end = (long) true_rumor_start + true_rumor_size;
        /* assert( true_rumor_end == false_rumor_start ); */
        false_rumor_end = (long) false_rumor_start + false_rumor_size;
        /* assert( false_rumor_end == eof_offset ); */
    } else {
        true_rumor_size = -1L; /* init failed */
        (void) dlb_fclose(fp);
    }
}

/* exclude_cookie is a hack used because we sometimes want to get rumors in a
 * context where messages such as "You swallowed the fortune!" that refer to
 * cookies should not appear.  This has no effect for true rumors since none
 * of them contain such references anyway.
 */
char *
getrumor(truth, rumor_buf, exclude_cookie)
int truth; /* 1=true, -1=false, 0=either */
char *rumor_buf;
boolean exclude_cookie;
{
    dlb *rumors;
    long tidbit, beginning;
    char *endp, line[BUFSZ], xbuf[BUFSZ];

    rumor_buf[0] = '\0';
    if (true_rumor_size < 0L) /* we couldn't open RUMORFILE */
        return rumor_buf;

    rumors = dlb_fopen(RUMORFILE, "r");

    if (rumors) {
        int count = 0;
        int adjtruth;

        do {
            rumor_buf[0] = '\0';
            if (true_rumor_size == 0L) { /* if this is 1st outrumor() */
                init_rumors(rumors);
                if (true_rumor_size < 0L) { /* init failed */
                    Sprintf(rumor_buf, "Error reading \"%.80s\".", RUMORFILE);
                    return rumor_buf;
                }
            }
            /*
             *  input:      1    0   -1
             *   rn2 \ +1  2=T  1=T  0=F
             *   adj./ +0  1=T  0=F -1=F
             */
            switch (adjtruth = truth + rn2(2)) {
            case 2: /*(might let a bogus input arg sneak thru)*/
            case 1:
                beginning = (long) true_rumor_start;
                tidbit = (long)rn2((int)true_rumor_size);
                break;
            case 0: /* once here, 0 => false rather than "either"*/
            case -1:
                beginning = (long) false_rumor_start;
                tidbit = (long)rn2((int)false_rumor_size);
                break;
            default:
                impossible("strange truth value for rumor");
                return strcpy(rumor_buf, "Oops...");
            }
            (void) dlb_fseek(rumors, beginning + tidbit, SEEK_SET);
            (void) dlb_fgets(line, sizeof line, rumors);
            if (!dlb_fgets(line, sizeof line, rumors)
                || (adjtruth > 0 && dlb_ftell(rumors) > true_rumor_end)) {
                /* reached end of rumors -- go back to beginning */
                (void) dlb_fseek(rumors, beginning, SEEK_SET);
                (void) dlb_fgets(line, sizeof line, rumors);
            }
            if ((endp = index(line, '\n')) != 0)
            {
                if (endp > line && *(endp - 1) == '\r')
                    endp--;
                *endp = 0;
            }
            Strcat(rumor_buf, xcrypt(line, xbuf));
        } while (
            count++ < 50 && exclude_cookie
            && (strstri(rumor_buf, "fortune") || strstri(rumor_buf, "pity")));
        (void) dlb_fclose(rumors);
        if (count >= 50)
            impossible("Can't find non-cookie rumor?");
        else if (!in_mklev) /* avoid exercizing wisdom for graffiti */
            exercise(A_WIS, (adjtruth > 0));
    } else {
        couldnt_open_file(RUMORFILE);
        true_rumor_size = -1; /* don't try to open it again */
    }
/* this is safe either way, so do it always since we can't get the definition
 * out of makedefs.c
 */
#define PAD_RUMORS_TO
#ifdef PAD_RUMORS_TO
    /* remove padding */
    {
        char *x = eos(rumor_buf) - 1;

        while (x > rumor_buf && *x == '_')
            x--;
        *++x = '\n';
        *x = '\0';
    }
#endif
    return rumor_buf;
}

/*
 * test that the true/false rumor boundaries are valid.
 */
void
rumor_check()
{
    dlb *rumors = 0;
    winid tmpwin;
    char *endp, line[BUFSZ], xbuf[BUFSZ], rumor_buf[BUFSZ];

    if (true_rumor_size < 0L) { /* we couldn't open RUMORFILE */
    no_rumors:
        pline("rumors not accessible.");
        if (rumors)
            (void) dlb_fclose(rumors);
        return;
    }

    rumors = dlb_fopen(RUMORFILE, "r");

    if (rumors) {
        long ftell_rumor_start = 0L;

        rumor_buf[0] = '\0';
        if (true_rumor_size == 0L) { /* if this is 1st outrumor() */
            init_rumors(rumors);
            if (true_rumor_size < 0L)
                goto no_rumors; /* init failed */
        }
        tmpwin = create_nhwindow(NHW_TEXT);

        /*
         * reveal the values.
         */

        Sprintf(
            rumor_buf,
            "T start=%06ld (%06lx), end=%06ld (%06lx), size=%06ld (%06lx)",
            (long) true_rumor_start, true_rumor_start, true_rumor_end,
            (unsigned long) true_rumor_end, true_rumor_size,
            (unsigned long) true_rumor_size);
        putstr(tmpwin, 0, rumor_buf);

        Sprintf(
            rumor_buf,
            "F start=%06ld (%06lx), end=%06ld (%06lx), size=%06ld (%06lx)",
            (long) false_rumor_start, false_rumor_start, false_rumor_end,
            (unsigned long) false_rumor_end, false_rumor_size,
            (unsigned long) false_rumor_size);
        putstr(tmpwin, 0, rumor_buf);

        /*
         * check the first rumor (start of true rumors) by
         * skipping the first two lines.
         *
         * Then seek to the start of the false rumors (based on
         * the value read in rumors, and display it.
         */
        rumor_buf[0] = '\0';
        (void) dlb_fseek(rumors, (long) true_rumor_start, SEEK_SET);
        ftell_rumor_start = dlb_ftell(rumors);
        (void) dlb_fgets(line, sizeof line, rumors);
        if ((endp = index(line, '\n')) != 0)
            *endp = 0;
        Sprintf(rumor_buf, "T %06ld %s", ftell_rumor_start,
                xcrypt(line, xbuf));
        putstr(tmpwin, 0, rumor_buf);
        /* find last true rumor */
        while (dlb_fgets(line, sizeof line, rumors)
               && dlb_ftell(rumors) < true_rumor_end)
            continue;
        if ((endp = index(line, '\n')) != 0)
            *endp = 0;
        Sprintf(rumor_buf, "  %6s %s", "", xcrypt(line, xbuf));
        putstr(tmpwin, 0, rumor_buf);

        rumor_buf[0] = '\0';
        (void) dlb_fseek(rumors, (long) false_rumor_start, SEEK_SET);
        ftell_rumor_start = dlb_ftell(rumors);
        (void) dlb_fgets(line, sizeof line, rumors);
        if ((endp = index(line, '\n')) != 0)
            *endp = 0;
        Sprintf(rumor_buf, "F %06ld %s", ftell_rumor_start,
                xcrypt(line, xbuf));
        putstr(tmpwin, 0, rumor_buf);
        /* find last false rumor */
        while (dlb_fgets(line, sizeof line, rumors)
               && dlb_ftell(rumors) < false_rumor_end)
            continue;
        if ((endp = index(line, '\n')) != 0)
            *endp = 0;
        Sprintf(rumor_buf, "  %6s %s", "", xcrypt(line, xbuf));
        putstr(tmpwin, 0, rumor_buf);

        (void) dlb_fclose(rumors);
        display_nhwindow(tmpwin, TRUE);
        destroy_nhwindow(tmpwin);
    } else {
        couldnt_open_file(RUMORFILE);
        true_rumor_size = -1; /* don't try to open it again */
    }
}

/* Gets a random line of text from file 'fname', and returns it.
   rng is the random number generator to use, and should act like rn2 does. */
char *
get_rnd_text(fname, buf, rng)
const char *fname;
char *buf;
int FDECL((*rng), (int));
{
    dlb *fh;

    buf[0] = '\0';

    fh = dlb_fopen(fname, "r");

    if (fh) {
        /* TODO: cache sizetxt, starttxt, endtxt. maybe cache file contents?
         */
        long sizetxt = 0, starttxt = 0, endtxt = 0, tidbit = 0;
        char *endp, line[BUFSZ], xbuf[BUFSZ];
        (void) dlb_fgets(line, sizeof line,
                         fh); /* skip "don't edit" comment */

        (void) dlb_fseek(fh, 0L, SEEK_CUR);
        starttxt = dlb_ftell(fh);
        (void) dlb_fseek(fh, 0L, SEEK_END);
        endtxt = dlb_ftell(fh);
        sizetxt = endtxt - starttxt;
        tidbit = (long)rng((int)sizetxt);

        (void) dlb_fseek(fh, starttxt + tidbit, SEEK_SET);
        (void) dlb_fgets(line, sizeof line, fh);
        if (!dlb_fgets(line, sizeof line, fh)) {
            (void) dlb_fseek(fh, starttxt, SEEK_SET);
            (void) dlb_fgets(line, sizeof line, fh);
        }
        if ((endp = index(line, '\n')) != 0)
            *endp = 0;
        Strcat(buf, xcrypt(line, xbuf));
        (void) dlb_fclose(fh);
    } else {
        couldnt_open_file(fname);
    }

    return buf;
}

void
outrumor(mtmp, otmp, truth, mechanism)
struct monst* mtmp;
struct obj* otmp;
int truth; /* 1=true, -1=false, 0=either */
int mechanism;
{
    static const char fortune_msg[] =
        "This cookie has a scrap of paper inside.";
    const char *line, *title = 0;
    char buf[BUFSZ];
    char titlebuf[BUFSZ] = "Rumor";
    boolean reading = (mechanism == BY_COOKIE || mechanism == BY_PAPER);

    if (reading) {
        /* deal with various things that prevent reading */
        if (is_fainted() && mechanism == BY_COOKIE)
            return;
        else if (Blind) {
            if (mechanism == BY_COOKIE)
                pline(fortune_msg);
            pline("What a pity that you cannot read it!");
            return;
        }
    }
    line = getrumor(truth, buf, reading ? FALSE : TRUE);
    if (!*line)
        line = "GnollHack rumors file closed for renovation.";

    boolean use_otmp = FALSE;
    switch (mechanism) {
    case BY_ORACLE:
        /* Oracle delivers the rumor */
        if (iflags.using_gui_sounds)
            pline("The Oracle seems to fall into a trance for a while. She then %sgives a note to you. It reads: ",
                (!rn2(4) ? "offhandedly "
                    : (!rn2(3) ? "casually "
                        : (rn2(2) ? "nonchalantly " : ""))));
        else
            pline("True to her word, the Oracle %ssays: ",
                  (!rn2(4) ? "offhandedly "
                           : (!rn2(3) ? "casually "
                                      : (rn2(2) ? "nonchalantly " : ""))));
        //verbalize1(line);
        popup_talk_line(mtmp, line);
        /* [WIS exercized by getrumor()] */
        return;
    case BY_COOKIE:
        pline(fortune_msg);
        pline("It reads:");
        title = "Fortune Cookie";
        break;
    case BY_PAPER:
        pline("It reads:");
        title = "Paper";
        use_otmp = TRUE;
        break;
    case BY_SPELL:
        pline("You sense a magical message in your mind:");
        title = "Magical Message";
        break;
    }

    if (use_otmp && otmp)
        strcpy_capitalized_for_title(titlebuf, cxname(otmp));
    else if(title)
        strcpy(titlebuf, title);

    pline1(line);
    display_popup_text(line, titlebuf, POPUP_TEXT_MESSAGE, ATR_NONE, NO_COLOR, NO_GLYPH, POPUP_FLAGS_ADD_QUOTES);

}

STATIC_OVL void
init_oracles(fp)
dlb *fp;
{
    register int i;
    char line[BUFSZ];
    int cnt = 0;

    /* this assumes we're only called once */
    (void) dlb_fgets(line, sizeof line, fp); /* skip "don't edit" comment*/
    (void) dlb_fgets(line, sizeof line, fp);
    if (sscanf(line, "%5d\n", &cnt) == 1 && cnt > 0) {
        oracle_cnt = (size_t) cnt;
        oracle_loc = (unsigned long *) alloc((size_t)cnt * sizeof(long));
        for (i = 0; i < cnt; i++) {
            (void) dlb_fgets(line, sizeof line, fp);
            (void) sscanf(line, "%5lx\n", &oracle_loc[i]);
        }
    }
    return;
}

void
save_oracles(fd, mode)
int fd, mode;
{
    if (perform_bwrite(mode)) {
        bwrite(fd, (genericptr_t) &oracle_cnt, sizeof oracle_cnt);
        if (oracle_cnt)
            bwrite(fd, (genericptr_t) oracle_loc, oracle_cnt * sizeof(unsigned long));
    }
    if (release_data(mode)) {
        if (oracle_cnt) {
            free((genericptr_t) oracle_loc);
            oracle_loc = 0, oracle_cnt = 0, oracle_flg = 0;
        }
    }
}
void
reset_oracles(VOID_ARGS)
{
    save_oracles(0, FREE_SAVE);
}

void
restore_oracles(fd)
int fd;
{
    mread(fd, (genericptr_t) &oracle_cnt, sizeof oracle_cnt);
    if (oracle_cnt) {
        oracle_loc = (unsigned long *) alloc(oracle_cnt * sizeof(unsigned long));
        mread(fd, (genericptr_t) oracle_loc, oracle_cnt * sizeof(unsigned long));
        oracle_flg = 1; /* no need to call init_oracles() */
    }
}

void
outoracle(mtmp, otmp, special, oraclesstyle)
struct monst* mtmp;
struct obj* otmp UNUSED;
boolean special;
int oraclesstyle; /* 0 = cookie, 1 = oracle, 2 = spell */
{
    winid tmpwin;
    dlb *oracles;
    int oracle_idx;
    char *endp, line[COLNO], xbuf[BUFSZ];

    /* early return if we couldn't open ORACLEFILE on previous attempt,
       or if all the oracularities are already exhausted */
    if (oracle_flg < 0 || (oracle_flg > 0 && oracle_cnt == 0))
        return;

    oracles = dlb_fopen(ORACLEFILE, "r");

    if (oracles) {
        if (oracle_flg == 0) { /* if this is the first outoracle() */
            init_oracles(oracles);
            oracle_flg = 1;
            if (oracle_cnt == 0)
                goto close_oracles;
        }
        /* oracle_loc[0] is the special oracle;
           oracle_loc[1..oracle_cnt-1] are normal ones */
        if (oracle_cnt <= 1 && !special)
            goto close_oracles; /*(shouldn't happen)*/
        oracle_idx = special ? 0 : rnd((int) oracle_cnt - 1);
        (void) dlb_fseek(oracles, (long) oracle_loc[oracle_idx], SEEK_SET);
        if (!special) /* move offset of very last one into this slot */
            oracle_loc[oracle_idx] = oracle_loc[--oracle_cnt];

        int glyph = any_mon_to_glyph(mtmp, rn2_on_display_rng);
        int gui_glyph = maybe_get_replaced_glyph(glyph, mtmp->mx, mtmp->my, data_to_replacement_info(glyph, LAYER_MONSTER, (struct obj*)0, mtmp, 0UL));
        tmpwin = create_nhwindow_ex(NHW_TEXT, GHWINDOW_STYLE_PAGER_SPEAKER, iflags.using_gui_tiles ? gui_glyph : glyph, extended_create_window_info_from_mon(mtmp));
        //tmpwin = create_nhwindow(NHW_TEXT);
        if (oraclesstyle == 1)
            putstr(tmpwin, 0,
                   special
                     ? "The Oracle scornfully takes all your money and says:"
                     : "The Oracle meditates for a moment and then intones:");
        else if (oraclesstyle == 2)
            putstr(tmpwin, 0, "You sense a magical message in your mind:");
        else
            putstr(tmpwin, 0, "The message reads:");
        putstr(tmpwin, 0, "");

        if (mtmp && oraclesstyle == 1)
            play_voice_oracle_major_consultation(mtmp, oracle_idx - 1);

        while (dlb_fgets(line, COLNO, oracles) && strcmp(line, "---\n")) 
        {
            if ((endp = index(line, '\n')) != 0)
                *endp = 0;
            char* xcryptbuf = xcrypt(line, xbuf);
            putstr(tmpwin, 0, xcryptbuf);
            if(!u.uevent.elbereth_known && strstri(xcryptbuf, "Elbereth"))
                u.uevent.elbereth_known = 1;
        }
        display_nhwindow(tmpwin, TRUE);
        destroy_nhwindow(tmpwin);

 close_oracles:
        (void) dlb_fclose(oracles);
    } else {
        couldnt_open_file(ORACLEFILE);
        oracle_flg = -1; /* don't try to open it again */
    }
}

int
doconsult(oracl)
struct monst *oracl;
{
    long umoney;
    long u_pay, minor_cost = max(1L, (long)(25.0 * service_cost_charisma_adjustment(ACURR(A_CHA)))), major_cost = max(1, (int)((double)(250 + 25 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    //int unid_cnt = count_unidentified(invent);
    int oracleaction = 0;
    int add_xpts;
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);

    if (!oracl)
    {
        There("is no one here to consult.");
        return 0;
    }
    else if (!is_peaceful(oracl)) 
    {
        pline("%s is in no mood for consultations.", Monnam(oracl));
        return 0;
    }
    else if (!umoney) 
    {
        You("have no money.");
        return 0;
    }

    if (Deaf)
    {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s says something but you cannot hear anything.", Monnam(oracl));
        return 1;
    }

    play_monster_special_dialogue_line(oracl, ORACLE_LINE_WILT_THOU_SETTLE_FOR_A_MINOR_CONSULTATION);
    Sprintf(qbuf, "\"Wilt thou settle for a minor consultation?\" (%ld %s)",
        minor_cost, currency(minor_cost));
    switch (ynq_mon(oracl, qbuf)) 
    {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < minor_cost) 
        {
            You_ex(ATR_NONE, CLR_MSG_FAIL, "don't even have enough money for that!");
            return 0;
        }

        u_pay = minor_cost;
        oracleaction = 1;
        break;
    case 'n':
        if (umoney <= minor_cost /* don't even ask */
            || (oracle_cnt == 1 || oracle_flg < 0))
            return 0;

        play_monster_special_dialogue_line(oracl, ORACLE_LINE_THEN_DOST_THOU_DESIRE_A_MAJOR_ONE);
        Sprintf(qbuf, "\"Then dost thou desire a major one?\" (%ld %s)",
            major_cost, currency(major_cost));
        if (yn_query(qbuf) != 'y')
            return 0;
        u_pay = (umoney < major_cost) ? umoney : major_cost;
        oracleaction = 2;
        break;
    }

    money2mon(oracl, u_pay);
    context.botl = 1;
    add_xpts = 0; /* first oracle of each type gives experience points */

    boolean cheapskate;

    switch (oracleaction) 
    {
    case 1:
        outrumor(oracl, (struct obj*)0, 1, BY_ORACLE);
        if (!u.uevent.minor_oracle)
            add_xpts = (int)u_pay / (u.uevent.major_oracle ? 25 : 10);
        /* 5 pts if very 1st, or 2 pts if major already done */
        u.uevent.minor_oracle = TRUE;
        break;
    case 2:
        cheapskate = u_pay < major_cost;

        outoracle(oracl, (struct obj*)0, cheapskate, 1);
        if (!cheapskate && !u.uevent.major_oracle)
            add_xpts = (int)u_pay / (u.uevent.minor_oracle ? 25 : 10);

        /* ~100 pts if very 1st, ~40 pts if minor already done */
        u.uevent.major_oracle = TRUE;
        exercise(A_WIS, !cheapskate);
        break;
    default:
        break;
    }

    if (add_xpts) 
    {
        more_experienced(add_xpts, 0); //  u_pay / 50);
        newexplevel();
    }

    if (!u.uachieve.consulted_oracle)
    {
        //achievement_gained("Consulted the Oracle");
        u.uachieve.consulted_oracle = 1;
    }

    return 1;
}

int
do_oracle_identify(oracl)
struct monst* oracl;
{
    long umoney;
    long minor_id_cost = max(1L, (long)((double)(150 + 10 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))) ; // 175 + 15 * u.ulevel;
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);

    if (!oracl) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        There("is no one here to identify items.");
        return 0;
    }
    else if (!is_peaceful(oracl)) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline("%s is in no mood for identification.", Monnam(oracl));
        return 0;
    }
    else if (!umoney) 
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }
    else if (count_unidentified(invent) == 0)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1_popup("have no unidentified items.", "Nothing to Identify", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    int res = 0;

    play_monster_special_dialogue_line(oracl, ORACLE_LINE_DOST_THOU_DESIRE_AN_IDENTIFICATION);
    Sprintf(qbuf, "\"Dost thou desire an identification?\" (%ld %s)",
        minor_id_cost, currency(minor_id_cost));

    switch (ynq_mon(oracl, qbuf))
    {
    default:
    case 'n':
    case 'q':
        return 0;
    case 'y':
        if (umoney < minor_id_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        break;
    }

    res = service_identify(oracl, minor_id_cost);

    //int u_pay;
    //int cnt = 0;
    //do
    //{
    //    if (!cnt)
    //    {
    //        play_monster_special_dialogue_line(oracl, ORACLE_LINE_DOST_THOU_DESIRE_AN_IDENTIFICATION);
    //        Sprintf(qbuf, "\"Dost thou desire an identification?\" (%d %s)",
    //            minor_id_cost, currency((long)minor_id_cost));
    //    }
    //    else
    //    {
    //        play_monster_special_dialogue_line(oracl, ORACLE_LINE_WOULDST_THOU_DESIRE_A_FURTHER_IDENTIFICATION);
    //        Sprintf(qbuf, "\"Wouldst thou desire a further identification?\" (%d %s)",
    //            minor_id_cost, currency((long)minor_id_cost));
    //    }

    //    switch (ynq_mon(oracl, qbuf))
    //    {
    //    default:
    //    case 'n':
    //    case 'q':
    //        return 0;
    //    case 'y':
    //        if (umoney < (long)minor_id_cost)
    //        {
    //            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
    //            You("don't have enough money for that!");
    //            return 0;
    //        }
    //        u_pay = minor_id_cost;
    //        break;
    //    }

    //    res = identify_pack(1, FALSE);

    //    if (res)
    //    {
    //        money2mon(oracl, (long)u_pay);
    //        context.botl = 1;
    //        umoney = money_cnt(invent);
    //        cnt += res;
    //    }
    //} while (res > 0 && count_unidentified(invent) > 0 && umoney >= (long)minor_id_cost && cnt < 100); /* Paranoid limit */

    return (res > 0);
}

int
do_oracle_enlightenment(oracl)
struct monst* oracl;
{
    long umoney, u_pay;
    long enl_cost = max(1, (int)((double)(objects[POT_ENLIGHTENMENT].oc_cost + 5L * (long)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);

    if (!oracl)
    {
        There("is no one here to enlighten you.");
        return 0;
    }
    else if (!is_peaceful(oracl)) 
    {
        pline("%s is in no mood for enlightenment.", Monnam(oracl));
        return 0;
    }
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    play_monster_special_dialogue_line(oracl, ORACLE_LINE_DOST_THOU_DESIRE_TO_ENLIGHTEN_YOURSELF);
    Sprintf(qbuf, "\"Dost thou desire to enlighten yourself?\" (%ld %s)",
        enl_cost, currency(enl_cost));
    if (yn_query(qbuf) != 'y')
        return 0;

    if (umoney < enl_cost)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }
    u_pay = enl_cost;

    money2mon(oracl, u_pay);
    context.botl = 1;

    /* enlightenment */
    You_feel("self-knowledgeable...");
    display_nhwindow(WIN_MESSAGE, FALSE);
    enlightenment(MAGICENLIGHTENMENT, ENL_GAMEINPROGRESS);

    return 1;
}


STATIC_OVL void
couldnt_open_file(filename)
const char *filename;
{
    int save_something = program_state.something_worth_saving;

    /* most likely the file is missing, so suppress impossible()'s
       "saving and restoring might fix this" (unless the fuzzer,
       which escalates impossible to panic, is running) */
    if (!iflags.debug_fuzzer)
        program_state.something_worth_saving = 0;

    impossible("Can't open '%s' file.", filename);
    program_state.something_worth_saving = save_something;
}

/*rumors.c*/
