/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

/* GnollHack 4.0    questpgr.c    $NHDT-Date: 1505172128 2017/09/11 23:22:08 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.38 $ */
/*      Copyright 1991, M. Stephenson                             */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "dlb.h"
#include "sp_lev.h"

/*  quest-specific pager routines. */

#include "qtext.h"

#define QTEXT_FILE "quest.dat"

#ifdef TTY_GRAPHICS
#include "wintty.h"
#endif

/* from sp_lev.c, for deliver_splev_message() */
extern struct lev_msg *lev_message;

STATIC_DCL void NDECL(dump_qtlist);
STATIC_DCL void FDECL(Fread, (genericptr_t, long, long, dlb *));
STATIC_DCL struct qtmsg *FDECL(construct_qtlist, (long));
STATIC_DCL const char *NDECL(intermed);
STATIC_DCL struct obj *FDECL(find_qarti, (struct obj *));
STATIC_DCL const char *NDECL(neminame);
STATIC_DCL const char *NDECL(guardname);
STATIC_DCL const char *NDECL(homebase);
STATIC_DCL void FDECL(qtext_pronoun, (CHAR_P, CHAR_P));
STATIC_DCL struct qtmsg *FDECL(msg_in, (struct qtmsg *, int));
STATIC_DCL void FDECL(convert_arg, (CHAR_P));
STATIC_DCL void FDECL(convert_line, (char *,char *));
STATIC_DCL void FDECL(deliver_by_pline, (struct qtmsg *, int, int, struct monst*, BOOLEAN_P));
STATIC_DCL void FDECL(deliver_by_window, (struct qtmsg *, int, int, int));
STATIC_DCL void FDECL(deliver_by_file_write, (dlb*, struct qtmsg*, int, int));
STATIC_DCL void FDECL(file_write_pager, (dlb*, struct qtmsg*, int, int));
STATIC_DCL boolean FDECL(skip_pager, (BOOLEAN_P));


STATIC_VAR char cvt_buf[64];
STATIC_VAR struct qtlists qt_list;
STATIC_VAR dlb *msg_file;
/* used by ldrname() and neminame(), then copied into cvt_buf */
STATIC_VAR char nambuf[sizeof cvt_buf];

/* dump the character msg list to check appearance;
   build with DEBUG enabled and use DEBUGFILES=questpgr.c
   in sysconf file or environment */
STATIC_OVL void
dump_qtlist()
{
#ifdef DEBUG
    struct qtmsg *msg;

    if (!explicitdebug(__FILE__))
        return;

    for (msg = qt_list.chrole; msg->msgnum > 0; msg++) {
        (void) dlb_fseek(msg_file, msg->offset, SEEK_SET);
        deliver_by_window(msg, ATR_NONE, NO_COLOR, NHW_MAP);
    }
#endif /* DEBUG */
    return;
}

STATIC_OVL void
Fread(ptr, size, nitems, stream)
genericptr_t ptr;
long size, nitems;
dlb *stream;
{
    long cnt;

    if ((cnt = dlb_fread(ptr, size, nitems, stream)) != nitems) {
        panic("PREMATURE EOF ON QUEST TEXT FILE! Expected %ld bytes, got %ld",
              (size * nitems), (size * cnt));
    }
}

STATIC_OVL struct qtmsg *
construct_qtlist(hdr_offset)
long hdr_offset;
{
    struct qtmsg *msg_list;
    int n_msgs;

    (void) dlb_fseek(msg_file, hdr_offset, SEEK_SET);
    Fread(&n_msgs, sizeof(int), 1, msg_file);
    msg_list = (struct qtmsg *) alloc(((size_t)n_msgs + 1) * sizeof (struct qtmsg));

    /*
     * Load up the list.
     */
    Fread((genericptr_t) msg_list, n_msgs * sizeof (struct qtmsg), 1,
          msg_file);

    msg_list[n_msgs].msgnum = -1;
    return msg_list;
}

void
load_qtlist()
{
    int n_classes, i;
    char qt_classes[N_HDR][LEN_HDR];
    long qt_offsets[N_HDR];

    msg_file = dlb_fopen(QTEXT_FILE, RDBMODE);
    if (!msg_file)
    {
        panic("CANNOT OPEN QUEST TEXT FILE %s.", QTEXT_FILE);
        return;
    }

    /*
     * Read in the number of classes, then the ID's & offsets for
     * each header.
     */

    Fread(&n_classes, sizeof (int), 1, msg_file);
    Fread(&qt_classes[0][0], sizeof (char) * LEN_HDR, n_classes, msg_file);
    Fread(qt_offsets, sizeof (long), n_classes, msg_file);

    /*
     * Now construct the message lists for quick reference later
     * on when we are actually paging the messages out.
     */

    qt_list.common = qt_list.chrole = (struct qtmsg *) 0;

    for (i = 0; i < n_classes; i++) {
        if (!strncmp(COMMON_ID, qt_classes[i], LEN_HDR))
            qt_list.common = construct_qtlist(qt_offsets[i]);
        else if (!strncmp(urole.filecode, qt_classes[i], LEN_HDR))
            qt_list.chrole = construct_qtlist(qt_offsets[i]);
#if 0 /* UNUSED but available */
        else if (!strncmp(urace.filecode, qt_classes[i], LEN_HDR))
            qt_list.chrace = construct_qtlist(qt_offsets[i]);
#endif
    }

    if (!qt_list.common || !qt_list.chrole)
        impossible("load_qtlist: cannot load quest text.");
    dump_qtlist();
    return; /* no ***DON'T*** close the msg_file */
}

/* called at program exit */
void
unload_qtlist()
{
    if (msg_file)
        (void) dlb_fclose(msg_file), msg_file = 0;
    if (qt_list.common)
        free((genericptr_t) qt_list.common), qt_list.common = 0;
    if (qt_list.chrole)
        free((genericptr_t) qt_list.chrole), qt_list.chrole = 0;
    return;
}

short
quest_info(typ)
int typ;
{
    switch (typ) {
    case 0:
        return urole.questarti;
    case MS_LEADER:
        return urole.ldrnum;
    case MS_NEMESIS:
        return urole.neminum;
    case MS_GUARDIAN:
        return urole.guardnum;
    default:
        impossible("quest_info(%d)", typ);
    }
    return 0;
}

/* return your role leader's name */
const char *
ldrname()
{
    int i = urole.ldrnum;

    Sprintf(nambuf, "%s%s", is_mname_proper_name(&mons[i]) ? "" : "the ",
            mons[i].mname);
    return nambuf;
}

/* return your intermediate target string */
STATIC_OVL const char *
intermed()
{
    return urole.intermed;
}

boolean
is_quest_artifact(otmp)
struct obj *otmp;
{
    return (boolean) (otmp->oartifact == urole.questarti);
}

STATIC_OVL struct obj *
find_qarti(ochain)
struct obj *ochain;
{
    struct obj *otmp, *qarti;

    for (otmp = ochain; otmp; otmp = otmp->nobj) {
        if (is_quest_artifact(otmp))
            return otmp;
        if (Has_contents(otmp) && (qarti = find_qarti(otmp->cobj)) != 0)
            return qarti;
    }
    return (struct obj *) 0;
}

/* check several object chains for the quest artifact to determine
   whether it is present on the current level */
struct obj *
find_quest_artifact(whichchains)
unsigned whichchains;
{
    struct monst *mtmp;
    struct obj *qarti = 0;

    if ((whichchains & (1 << OBJ_INVENT)) != 0)
        qarti = find_qarti(invent);
    if (!qarti && (whichchains & (1 << OBJ_FLOOR)) != 0)
        qarti = find_qarti(fobj);
    if (!qarti && (whichchains & (1 << OBJ_MINVENT)) != 0)
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if ((qarti = find_qarti(mtmp->minvent)) != 0)
                break;
        }
    if (!qarti && (whichchains & (1 << OBJ_MIGRATING)) != 0) {
        /* check migrating objects and minvent of migrating monsters */
        for (mtmp = migrating_mons; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if ((qarti = find_qarti(mtmp->minvent)) != 0)
                break;
        }
        if (!qarti)
            qarti = find_qarti(migrating_objs);
    }
    if (!qarti && (whichchains & (1 << OBJ_BURIED)) != 0)
        qarti = find_qarti(level.buriedobjlist);

    return qarti;
}

/* return your role nemesis' name */
STATIC_OVL const char *
neminame()
{
    int i = urole.neminum;

    Sprintf(nambuf, "%s%s", is_mname_proper_name(&mons[i]) ? "" : "the ",
            mons[i].mname);
    return nambuf;
}

STATIC_OVL const char *
guardname() /* return your role leader's guard monster name */
{
    int i = urole.guardnum;

    return mons[i].mname;
}

STATIC_OVL const char *
homebase() /* return your role leader's location */
{
    return urole.homebase;
}

/* replace deity, leader, nemesis, or artifact name with pronoun;
   overwrites cvt_buf[] */
STATIC_OVL void
qtext_pronoun(who, which)
char who,  /* 'd' => deity, 'l' => leader, 'n' => nemesis, 'o' => artifact */
    which; /* 'h'|'H'|'i'|'I'|'j'|'J' */
{
    const char *pnoun;
    int g;
    char lwhich = lowc(which); /* H,I,J -> h,i,j */

    /*
     * Invalid subject (not d,l,n,o) yields neuter, singular result.
     *
     * For %o, treat all artifacts as neuter; some have plural names,
     * which genders[] doesn't handle; cvt_buf[] already contains name.
     */
    if (who == 'o'
        && (strstri(cvt_buf, "Eyes ")
            || strcmpi(cvt_buf, makesingular(cvt_buf)))) {
        pnoun = (lwhich == 'h') ? "they"
                : (lwhich == 'i') ? "them"
                : (lwhich == 'j') ? "their" : "?";
    } else {
        g = (who == 'd') ? quest_status.godgend
            : (who == 'l') ? quest_status.ldrgend
            : (who == 'n') ? quest_status.nemgend
            : 2; /* default to neuter */
        pnoun = (lwhich == 'h') ? genders[g].he
                : (lwhich == 'i') ? genders[g].him
                : (lwhich == 'j') ? genders[g].his : "?";
    }
    Strcpy(cvt_buf, pnoun);
    /* capitalize for H,I,J */
    if (lwhich != which)
        cvt_buf[0] = highc(cvt_buf[0]);
    return;
}

STATIC_OVL struct qtmsg *
msg_in(qtm_list, msgnum)
struct qtmsg *qtm_list;
int msgnum;
{
    struct qtmsg *qt_msg;

    for (qt_msg = qtm_list; qt_msg->msgnum > 0; qt_msg++)
        if (qt_msg->msgnum == msgnum)
            return qt_msg;

    return (struct qtmsg *) 0;
}

STATIC_OVL void
convert_arg(c)
char c;
{
    register const char *str = 0;
    char buf[BUFSZ] = "";

    switch (c) {
    case 'p':
        if (iflags.using_gui_sounds)
            str = "adventurer";
        else
            str = plname;
        break;
    case 'c':
        str = (flags.female && urole.name.f) ? urole.name.f : urole.name.m;
        break;
    case 'v': /* Variable version of 'r' for voice acting purposes */
        if (iflags.using_gui_sounds)
            str = (flags.female && urole.name.f) ? urole.name.f : urole.name.m;
        else
            str = rank_of(u.ulevel, Role_switch, flags.female);
        break;
    case 'r':
        str = rank_of(u.ulevel, Role_switch, flags.female);
        break;
    case 'R':
        str = rank_of(MIN_QUEST_LEVEL, Role_switch, flags.female);
        break;
    case 's':
        str = (flags.female) ? "sister" : "brother";
        break;
    case 'S':
        str = (flags.female) ? "daughter" : "son";
        break;
    case 'l':
        str = ldrname();
        break;
    case 'i':
        str = intermed();
        break;
    case 'O':
    case 'o':
        str = the(artiname(urole.questarti));
        if (c == 'O') {
            /* shorten "the Foo of Bar" to "the Foo"
               (buffer returned by the() is modifiable) */
            char *p = strstri(str, " of ");

            if (p)
                *p = '\0';
        }
        break;
    case 'n':
        str = neminame();
        break;
    case 'g':
        str = guardname();
        break;
    case 'G':
        str = align_gtitle(u.ualignbase[A_ORIGINAL]);
        break;
    case 'H':
        str = homebase();
        break;
    case 'a':
        str = align_str(u.ualignbase[A_ORIGINAL]);
        break;
    case 'A':
        str = align_str(u.ualign.type);
        break;
    case 'd':
        str = align_gname(u.ualignbase[A_ORIGINAL]);
        break;
    case 'D':
        str = align_gname(A_LAWFUL);
        break;
    case 'f':
        str = align_gdesc(u.ualignbase[A_ORIGINAL]);
        break;
    case 'h':
        str = align_ghisher(u.ualignbase[A_ORIGINAL]);
        break;
    case 'e':
        str = align_gname(u.ualign.type);
        break;
    case 'C':
        str = "chaotic";
        break;
    case 'N':
        str = "neutral";
        break;
    case 'L':
        str = "lawful";
        break;
    case 'x':
        str = Blind ? "sense" : "see";
        break;
    case 'Z':
        str = dungeons[0].dname;
        break;
    case '%':
        str = "%";
        break;
    default:
        str = "";
        break;
    }

    if(str)
        Strcpy(cvt_buf, str);
    else
        Strcpy(cvt_buf, buf);
}

STATIC_OVL void
convert_line(in_line, out_line)
char *in_line, *out_line;
{
    char *c, *cc;
    char xbuf[BUFSZ];

    cc = out_line;
    for (c = xcrypt(in_line, xbuf); *c; c++) {
        *cc = 0;
        switch (*c) {
        case '\r':
        case '\n':
            *(++cc) = 0;
            return;

        case '%':
            if (*(c + 1)) {
                convert_arg(*(++c));
                switch (*(++c)) {
                /* insert "a"/"an" prefix */
                case 'B':
                    cvt_buf[0] = lowc(cvt_buf[0]);
                    /*FALLTHRU*/
                case 'A':
                    Strcat(cc, An(cvt_buf));
                    cc += strlen(cc);
                    continue; /* for */
                case 'b':
                    cvt_buf[0] = lowc(cvt_buf[0]);
                    /*FALLTHRU*/
                case 'a':
                    Strcat(cc, an(cvt_buf));
                    cc += strlen(cc);
                    continue; /* for */

                /* capitalize */
                case 'C':
                    cvt_buf[0] = highc(cvt_buf[0]);
                    break;
                case 'l':
                    cvt_buf[0] = lowc(cvt_buf[0]);
                    break;
                /* replace name with pronoun;
                   valid for %d, %l, %n, and %o */
                case 'h': /* he/she */
                case 'H': /* He/She */
                case 'i': /* him/her */
                case 'I':
                case 'j': /* his/her */
                case 'J':
                    if (index("dlno", lowc(*(c - 1))))
                        qtext_pronoun(*(c - 1), *c);
                    else
                        --c; /* default action */
                    break;

                /* pluralize */
                case 'P':
                    cvt_buf[0] = highc(cvt_buf[0]);
                    /*FALLTHRU*/
                case 'p':
                    Strcpy(cvt_buf, makeplural(cvt_buf));
                    break;

                /* append possessive suffix */
                case 'S':
                    cvt_buf[0] = highc(cvt_buf[0]);
                    /*FALLTHRU*/
                case 's':
                    Strcpy(cvt_buf, s_suffix(cvt_buf));
                    break;

                /* strip any "the" prefix */
                case 't':
                    if (!strncmpi(cvt_buf, "the ", 4)) {
                        Strcat(cc, &cvt_buf[4]);
                        cc += strlen(cc);
                        continue; /* for */
                    }
                    break;

                default:
                    --c; /* undo switch increment */
                    break;
                }
                Strcat(cc, cvt_buf);
                cc += strlen(cvt_buf);
                break;
            } /* else fall through */

        default:
            *cc++ = *c;
            break;
        }
    }
    if (cc > & out_line[BUFSZ - 1])
    {
        panic("convert_line: overflow");
        return;
    }
    *cc = 0;
    return;
}

STATIC_OVL void
deliver_by_pline(qt_msg, attr, color, mtmp, dopopup)
struct qtmsg *qt_msg;
int attr, color;
struct monst* mtmp;
boolean dopopup;
{
    long size;
    char in_line[BUFSZ] = "", out_line[BUFSZ] = "";
#define TOUTBUFSZ BUFSZ * 10
    char total_out_line[TOUTBUFSZ] = "";

    /* *in_line = '\0'; */
    for (size = 0; size < qt_msg->size; size += (long) strlen(in_line)) 
    {
        (void) dlb_fgets(in_line, sizeof in_line, msg_file);
        convert_line(in_line, out_line);
        if (strlen(total_out_line) + strlen(out_line) + 1 >= TOUTBUFSZ)
            break;
        if(*total_out_line && *out_line)
            Sprintf(eos(total_out_line), " ");
        Sprintf(eos(total_out_line), "%s", out_line);
    }

    if (dopopup)
    {
        if(mtmp)
            popup_talk_line_ex(mtmp, total_out_line, attr, color, TRUE, FALSE);
        else
        {
            pline_ex(attr, color, "%s", total_out_line);
            display_popup_text(total_out_line, "", POPUP_TEXT_GENERAL, attr, color, NO_GLYPH, POPUP_FLAGS_NONE);
        }
    }
    else
        pline_ex(attr, color, "%s", total_out_line);
}

STATIC_OVL void
deliver_by_window(qt_msg, attr, color, how)
struct qtmsg *qt_msg;
int attr, color, how;
{
    long size;
    char in_line[BUFSZ], out_line[BUFSZ], attrs[BUFSZ], colors[BUFSZ];
    boolean qtdump = (how == NHW_MAP);
    winid datawin = create_nhwindow_ex(qtdump ? NHW_TEXT : how, GHWINDOW_STYLE_PAGER_GENERAL, NO_GLYPH, zerocreatewindowinfo);

#ifdef DEBUG
    if (qtdump) 
    {
        char buf[BUFSZ];

        /* when dumping quest messages at startup, all of them are passed to
         * deliver_by_window(), even if normally given to deliver_by_pline()
         */
        Sprintf(buf, "msgnum: %d, delivery: %c",
                qt_msg->msgnum, qt_msg->delivery);
        putstr(datawin, 0, buf);
        putstr(datawin, 0, "");
    }
#endif
    for (size = 0; size < qt_msg->size; size += (long) strlen(in_line)) {
        (void) dlb_fgets(in_line, sizeof in_line, msg_file);
        convert_line(in_line, out_line);
        putstr(datawin, 0, out_line);
    }
    display_nhwindow(datawin, TRUE);
    destroy_nhwindow(datawin);

    /* block messages delivered by window aren't kept in message history
       but have a one-line summary which is put there for ^P recall */
    *out_line = '\0';
    if (qt_msg->summary_size) {
        (void) dlb_fgets(in_line, sizeof in_line, msg_file);
        convert_line(in_line, out_line);
#ifdef BETA
    } else if (qt_msg->delivery == 'c') { /* skip for 'qtdump' of 'p' */
        /* delivery 'c' and !summary_size, summary expected but not present;
           this doesn't prefix the number with role code vs 'general'
           but should be good enough for summary verification purposes */
        Sprintf(out_line, "[missing block message summary for #%05d]",
                qt_msg->msgnum);
#endif
    }
    if (*out_line)
    {
        size_t len = strlen(out_line);
        memset(attrs, attr, len);
        memset(colors, color, len);
        attrs[len] = colors[len] = 0;
        putmsghistory_ex(out_line, attrs, colors, FALSE);
    }
}

boolean
skip_pager(common)
boolean common;
{
    /* WIZKIT: suppress plot feedback if starting with quest artifact */
    if (program_state.wizkit_wishing)
        return TRUE;
    if (!(common ? qt_list.common : qt_list.chrole)) {
        panic("%s: no %s quest text data available",
              common ? "com_pager" : "qt_pager",
              common ? "common" : "role-specific");
        /*NOTREACHED*/
        return TRUE;
    }
    return FALSE;
}

void
com_pager(mtmp, msgnum)
struct monst* mtmp;
int msgnum;
{
    com_pager_ex(mtmp, msgnum, ATR_NONE, NO_COLOR, FALSE);
}

void
com_pager_ex(mtmp, msgnum, attr, color, dopopup)
struct monst* mtmp;
int msgnum, attr, color;
boolean dopopup;
{
    struct qtmsg *qt_msg;

    if (skip_pager(TRUE))
        return;

    if (!(qt_msg = msg_in(qt_list.common, msgnum))) {
        impossible("com_pager: message %d not found.", msgnum);
        return;
    }

    (void) dlb_fseek(msg_file, qt_msg->offset, SEEK_SET);
    if (qt_msg->delivery == 'p')
    {
        play_voice_com_pager(mtmp, msgnum, TRUE);
        deliver_by_pline(qt_msg, attr, color, mtmp, dopopup);
    }
    else if (msgnum == 1)
    {
        play_intro_text();
        deliver_by_window(qt_msg, attr, color, NHW_MENU);
        stop_all_immediate_sounds();
    }
    else
    {
        play_voice_com_pager(mtmp, msgnum, FALSE);
        deliver_by_window(qt_msg, attr, color, NHW_TEXT);
        stop_all_immediate_sounds();
    }
    return;
}

void
qt_pager(mtmp, msgnum)
struct monst* mtmp;
int msgnum;
{
    qt_pager_ex(mtmp, msgnum, ATR_NONE, NO_COLOR, FALSE);
}

void
qt_pager_ex(mtmp, msgnum, attr, color, dopopup)
struct monst* mtmp;
int msgnum, attr, color;
boolean dopopup;
{
    struct qtmsg *qt_msg;

    if (skip_pager(FALSE))
        return;

    qt_msg = msg_in(qt_list.chrole, msgnum);
    if (!qt_msg) {
        /* some roles have an alternate message for return to the goal
           level when the quest artifact is absent (handled by caller)
           but some don't; for the latter, use the normal goal message;
           note: for first visit, artifact is assumed to always be
           present which might not be true for wizard mode but we don't
           worry about quest message references in that situation */
        if (msgnum == QT_ALTGOAL)
            qt_msg = msg_in(qt_list.chrole, QT_NEXTGOAL);
    }
    if (!qt_msg) {
        impossible("qt_pager: message %d not found.", msgnum);
        return;
    }

    (void) dlb_fseek(msg_file, qt_msg->offset, SEEK_SET);
    if (qt_msg->delivery == 'p' && strcmp(windowprocs.name, "X11"))
    {
        play_voice_quest_pager(mtmp, qt_msg->msgnum, TRUE);
        deliver_by_pline(qt_msg, attr, color, mtmp, dopopup);
    }
    else
    {
        play_voice_quest_pager(mtmp, qt_msg->msgnum, FALSE);
        deliver_by_window(qt_msg, attr, color, NHW_TEXT);
        stop_all_immediate_sounds();
    }
    return;
}

struct permonst *
qt_montype()
{
    int qpm;

    if (rn2(5)) {
        qpm = urole.enemy1num;
        if (qpm != NON_PM && rn2(5) && !(mvitals[qpm].mvflags & MV_GENOCIDED))
            return &mons[qpm];
        return mkclass(urole.enemy1sym, 0);
    }
    qpm = urole.enemy2num;
    if (qpm != NON_PM && rn2(5) && !(mvitals[qpm].mvflags & MV_GENOCIDED))
        return &mons[qpm];
    return mkclass(urole.enemy2sym, 0);
}

/* special levels can include a custom arrival message; display it */
void
deliver_splev_message()
{
    char *str, *nl, in_line[BUFSZ], out_line[BUFSZ];
    struct lev_msg* lm, *nextlm;
    boolean dopopup = FALSE, isugod = FALSE;
    char popupbuf[BUFSZ * 4] = "";

    /* there's no provision for delivering via window instead of pline */
    for (lm = lev_message; lm; lm = nextlm)
    {
        nextlm = lm->next;

        /* Play sound if any */
        switch (lm->sound_type)
        {
        case 1:
            play_voice_god_simple_line_by_align(u.ualign.type, lm->sound_id);
            break;
        case 2:
            play_sfx_sound(lm->sound_id);
            break;
        default:
            break;
        }

        /* lev_message can span multiple lines using embedded newline chars;
           any segments too long to fit within in_line[] will be truncated */
        for (str = lm->message; *str; str = nl + 1) 
        {
            /* copying will stop at newline if one is present */
            copynchars(in_line, str, (int) (sizeof in_line) - 1);

            /* convert_line() expects encrypted input */
            (void) xcrypt(in_line, in_line);
            convert_line(in_line, out_line);
            pline_ex(lm->attr, lm->color, "%s", out_line);
            if (lm->msgflags & 1UL)
            {
                if(*popupbuf)
                    Strcat(popupbuf, " ");
                Strcat(popupbuf, out_line);
            }
            if ((nl = index(str, '\n')) == 0)
                break; /* done if no newline */
        }
        if (lm->msgflags & 1UL)
            dopopup = TRUE;
        if (lm->msgflags & 2UL)
            isugod = TRUE;

        free((genericptr_t)lm->message);
        free((genericptr_t)lm);
    }
    if (dopopup && *popupbuf)
        display_popup_text(popupbuf, isugod ? u_gname() : "Message", POPUP_TEXT_MESSAGE, ATR_NONE, isugod ? CLR_MSG_GOD : NO_COLOR, NO_GLYPH, isugod ? POPUP_FLAGS_ADD_QUOTES | POPUP_FLAGS_COLOR_TEXT : POPUP_FLAGS_NONE);
    lev_message = 0;
}


STATIC_OVL void
deliver_by_file_write(temp_msg_file, qt_msg, fd, msgnum)
dlb* temp_msg_file;
struct qtmsg* qt_msg;
int fd, msgnum;
{
    long size;
    char in_line[BUFSZ], out_line[BUFSZ];
    char cur_outline[BUFSIZ * 10];
    char prev_outline[BUFSIZ * 10];
    char buf[BUFSIZ];
    Strcpy(prev_outline, "");

    int rounds = flags.initrole == ROLE_PRIEST ? NUM_ROLES : 1;
    int k;
    for (k = 0; k < rounds; k++)
    {
        if (k == ROLE_PRIEST)
            continue;

        if (flags.initrole == ROLE_PRIEST)
            flags.pantheon = k;

        int j;
        for (j = A_CHAOTIC + 1; j <= A_LAWFUL + 1; j++)
        {
            flags.initalign = j;
            if (!validalign(flags.initrole, flags.initrace, flags.initalign))
                continue;

            role_init();
            u.ualignbase[A_CURRENT] = u.ualignbase[A_ORIGINAL] = u.ualign.type =
                aligns[flags.initalign].value;

            (void)dlb_fseek(temp_msg_file, qt_msg->offset, SEEK_SET);

            *in_line = '\0';
            *cur_outline = '\0';
            for (size = 0; size < qt_msg->size; size += (long)strlen(in_line))
            {
                (void)dlb_fgets(in_line, sizeof in_line, temp_msg_file);
                convert_line(in_line, out_line);
                Strcat(cur_outline, out_line);
                Strcat(cur_outline, " ");
            }
            if (!strcmp(cur_outline, prev_outline))
                continue;

            Strcpy(prev_outline, cur_outline);

            Sprintf(buf, "%d. ", msgnum);
            (void)write(fd, buf, strlen(buf));
            (void)write(fd, cur_outline, strlen(cur_outline));
            Sprintf(buf, "\n");
            (void)write(fd, buf, strlen(buf));
        }
    }
}

STATIC_OVL
void
file_write_pager(temp_msg_file, msg_chain, fd, msgnum)
dlb* temp_msg_file;
struct qtmsg* msg_chain;
int fd, msgnum;
{
    struct qtmsg* qt_msg;

    qt_msg = msg_in(msg_chain, msgnum);
    
    if (!qt_msg)
    {
        /* some roles have an alternate message for return to the goal
           level when the quest artifact is absent (handled by caller)
           but some don't; for the latter, use the normal goal message;
           note: for first visit, artifact is assumed to always be
           present which might not be true for wizard mode but we don't
           worry about quest message references in that situation */
        if (msgnum == QT_ALTGOAL)
            qt_msg = msg_in(msg_chain, QT_NEXTGOAL);
    }
    
    if (!qt_msg) 
    {
        /* Nothing found */
        return;
    }

    deliver_by_file_write(temp_msg_file, qt_msg, fd, msgnum);

    return;
}



void
write_quest_texts(fd)
int fd;
{
    int n_classes, i, j, k;
    char qt_classes[N_HDR][LEN_HDR];
    long qt_offsets[N_HDR];
    
    struct qtlists temp_qt_list;

    dlb* msg_file2 = dlb_fopen(QTEXT_FILE, RDBMODE);
    if (!msg_file2)
    {
        panic("CANNOT OPEN QUEST TEXT FILE %s.", QTEXT_FILE);
        return;
    }

    /*
     * Read in the number of classes, then the ID's & offsets for
     * each header.
     */

    Fread(&n_classes, sizeof(int), 1, msg_file2);
    Fread(&qt_classes[0][0], sizeof(char) * LEN_HDR, n_classes, msg_file2);
    Fread(qt_offsets, sizeof(long), n_classes, msg_file2);

    /*
     * Now construct the message lists for quick reference later
     * on when we are actually paging the messages out.
     */

    temp_qt_list.common = temp_qt_list.chrole = (struct qtmsg*)0;

    struct Role save_role = urole;
    struct Race save_race = urace;
    int save_pantheon = flags.pantheon;
    boolean common_printed = FALSE;
    char plbuf[PL_NSIZ];
    Strcpy(plbuf, plname);
    Strcpy(plname, "adventurer");
    int current_alignbase = u.ualignbase[A_CURRENT];
    int original_alignbase = u.ualignbase[A_ORIGINAL];
    int ualigntype = u.ualign.type;

    for (j = 0; j < NUM_ROLES; j++)
    {
        const char* role_filecode = roles[j].filecode;
#if 0 /* UNUSED but available */
        const char* race_filecode = races[j].filecode;
#endif
        for (i = 0; i < n_classes; i++)
        {
            if (!strncmp(COMMON_ID, qt_classes[i], LEN_HDR))
            {
                temp_qt_list.common = construct_qtlist(qt_offsets[i]);
            }
            else if (!strncmp(role_filecode, qt_classes[i], LEN_HDR))
            {
                temp_qt_list.chrole = construct_qtlist(qt_offsets[i]);
            }
#if 0 /* UNUSED but available */
            else if (!strncmp(race_filecode, qt_classes[i], LEN_HDR))
            {
                temp_qt_list.chrace = construct_qtlist(qt_offsets[i]);
            }
#endif
        }

        char buf[BUFSIZ];
        if (!temp_qt_list.common || !temp_qt_list.chrole)
            impossible("load_qtlist: cannot load quest text for '%s'.", role_filecode);
        else
        {
            flags.initrole = j;
            flags.initrace = randrace(flags.initrole);
            flags.initgend = 1;

            if (!common_printed)
            {
                Sprintf(buf, "-- Common --\n");
                (void)write(fd, buf, strlen(buf));
                for (k = 1; k <= 60; k++)
                {
                    file_write_pager(msg_file2, temp_qt_list.common, fd, k);
                }
                common_printed = TRUE;
            }
            Sprintf(buf, "-- %s Quest --\n", roles[j].name.m);
            (void)write(fd, buf, strlen(buf));
            for (k = 1; k <= 91; k++)
            {
                file_write_pager(msg_file2, temp_qt_list.chrole, fd, k);
            }
        }
        if (temp_qt_list.common)
            free(temp_qt_list.common);

        if (temp_qt_list.chrole)
            free(temp_qt_list.chrole);
    }

    urole = save_role;
    urace = save_race;
    flags.pantheon = save_pantheon;
    Strcpy(plname, plbuf);
    u.ualign.type = ualigntype;
    u.ualignbase[A_CURRENT] = current_alignbase;
    u.ualignbase[A_ORIGINAL] = original_alignbase;

    dlb_fclose(msg_file2);

}

/*questpgr.c*/
