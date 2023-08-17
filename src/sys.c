/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    sys.c    $NHDT-Date: 1547118632 2019/01/10 11:10:32 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.43 $ */
/* Copyright (c) Kenneth Lorber, Kensington, Maryland, 2008. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#ifndef SYSCF
/* !SYSCF configurations need '#define DEBUGFILES "foo.c bar.c"'
 * to enable debugging feedback for source files foo.c and bar.c;
 * to activate debugpline(), set an appropriate value and uncomment
 */
/* # define DEBUGFILES "*" */

/* note: DEBUGFILES value here or in sysconf.DEBUGFILES can be overridden
   at runtime by setting up a value for "DEBUGFILES" in the environment */
#endif

struct sysopt sysopt;

void
sys_early_init()
{
    reset_global_variables();

    sysopt.support = (char *) 0;
    sysopt.recover = (char *) 0;
    sysopt.livelog = 0;
#ifdef SYSCF
    sysopt.wizards = (char *) 0;
#else
    sysopt.wizards = dupstr(WIZARD_NAME);
#ifdef LIVELOGFILE
    sysopt.livelog = LIVELOG_DETAIL;
    sysopt.ll_conduct_turns = 0;
#endif
#endif
#if defined(SYSCF) || !defined(DEBUGFILES)
    sysopt.debugfiles = (char *) 0;
#else
    sysopt.debugfiles = dupstr(DEBUGFILES);
#endif
#if defined (DUMPLOG)
    sysopt.dumplogfile = (char *) 0;
    sysopt.dumplogurl = (char*)0;
#endif
#if defined (DUMPHTML)
    sysopt.dumphtmlfile = (char*)0;
    sysopt.dumphtmlfontname = (char*)0;
#if defined (DUMPHTML_WEBFONT_LINK)
    sysopt.dumphtmlfontlink = (char*)0;
#endif
#endif
    sysopt.dumphtml_css_fontface_normal = (char*)0;
    sysopt.dumphtml_css_fontface_bold = (char*)0;
    sysopt.dumphtml_css_fontface_italic = (char*)0;
    sysopt.dumphtml_css_fontface_bolditalic = (char*)0;
    sysopt.env_dbgfl = 0; /* haven't checked getenv("DEBUGFILES") yet */
    sysopt.shellers = (char *) 0;
    sysopt.explorers = (char *) 0;
    sysopt.genericusers = (char *) 0;
    sysopt.maxplayers = 0; /* XXX eventually replace MAX_NR_OF_PLAYERS */
    sysopt.bones_pools = 0;
    sysopt.min_difficulty = MIN_DIFFICULTY_LEVEL;
    sysopt.max_difficulty = MAX_DIFFICULTY_LEVEL;

    /* record file */
    sysopt.persmax = PERSMAX;
    sysopt.entrymax = ENTRYMAX;
    sysopt.pointsmin = POINTSMIN;
    sysopt.pers_is_uid = PERS_IS_UID;
    sysopt.tt_oname_maxrank = 10;

    /* sanity checks */
    if (PERSMAX < 1)
        sysopt.persmax = 1;
    if (ENTRYMAX < 10)
        sysopt.entrymax = 10;
    if (POINTSMIN < 1)
        sysopt.pointsmin = 1;
    if (PERS_IS_UID != 0 && PERS_IS_UID != 1)
    {
        panic("config error: PERS_IS_UID must be either 0 or 1");
        return;
    }
#ifdef PANICTRACE
    /* panic options */
    sysopt.gdbpath = dupstr(GDBPATH);
    sysopt.greppath = dupstr(GREPPATH);
#ifdef BETA
    sysopt.panictrace_gdb = 1;
#ifdef PANICTRACE_LIBC
    sysopt.panictrace_libc = 2;
#endif
#else
    sysopt.panictrace_gdb = 0;
#ifdef PANICTRACE_LIBC
    sysopt.panictrace_libc = 0;
#endif
#endif
#endif

    sysopt.check_save_uid = 1;
    sysopt.check_plname = 0;
    sysopt.select_pet_details = 0;
    sysopt.make_backup_savefiles = 0;
    sysopt.allow_loading_error_savefiles = 0;
    sysopt.seduce = 1; /* if it's compiled in, default to on */
    sysopt_seduce_set(sysopt.seduce);
    return;
}

void
sysopt_release()
{
    if (sysopt.support)
        free((genericptr_t) sysopt.support), sysopt.support = (char *) 0;
    if (sysopt.recover)
        free((genericptr_t) sysopt.recover), sysopt.recover = (char *) 0;
    if (sysopt.wizards)
        free((genericptr_t) sysopt.wizards), sysopt.wizards = (char *) 0;
    if (sysopt.explorers)
        free((genericptr_t) sysopt.explorers), sysopt.explorers = (char *) 0;
    if (sysopt.shellers)
        free((genericptr_t) sysopt.shellers), sysopt.shellers = (char *) 0;
    if (sysopt.debugfiles)
        free((genericptr_t) sysopt.debugfiles),
        sysopt.debugfiles = (char *) 0;
#if defined (DUMPLOG)
    if (sysopt.dumplogfile)
        free((genericptr_t)sysopt.dumplogfile), sysopt.dumplogfile=(char *)0;
#endif
#if defined (DUMPHTML)
    if (sysopt.dumphtmlfile)
        free((genericptr_t)sysopt.dumphtmlfile), sysopt.dumphtmlfile = (char*)0;
    if (sysopt.dumphtmlfontname)
        free((genericptr_t)sysopt.dumphtmlfontname), sysopt.dumphtmlfontname = (char*)0;
#if defined (DUMPHTML_WEBFONT_LINK)
    if (sysopt.dumphtmlfontlink)
        free((genericptr_t)sysopt.dumphtmlfontlink), sysopt.dumphtmlfontlink = (char*)0;
#endif
    if (sysopt.dumphtml_css_fontface_normal)
        free((genericptr_t)sysopt.dumphtml_css_fontface_normal), sysopt.dumphtml_css_fontface_normal = (char*)0;
    if (sysopt.dumphtml_css_fontface_bold)
        free((genericptr_t)sysopt.dumphtml_css_fontface_bold), sysopt.dumphtml_css_fontface_bold = (char*)0;
    if (sysopt.dumphtml_css_fontface_italic)
        free((genericptr_t)sysopt.dumphtml_css_fontface_italic), sysopt.dumphtml_css_fontface_italic = (char*)0;
    if (sysopt.dumphtml_css_fontface_bolditalic)
        free((genericptr_t)sysopt.dumphtml_css_fontface_bolditalic), sysopt.dumphtml_css_fontface_bolditalic = (char*)0;
#endif
    if (sysopt.genericusers)
        free((genericptr_t) sysopt.genericusers),
        sysopt.genericusers = (char *) 0;
#ifdef PANICTRACE
    if (sysopt.gdbpath)
        free((genericptr_t) sysopt.gdbpath), sysopt.gdbpath = (char *) 0;
    if (sysopt.greppath)
        free((genericptr_t) sysopt.greppath), sysopt.greppath = (char *) 0;
#endif
    /* this one's last because it might be used in panic feedback, although
       none of the preceding ones are likely to trigger a controlled panic */
    if (sysopt.fmtd_wizard_list)
        free((genericptr_t) sysopt.fmtd_wizard_list),
        sysopt.fmtd_wizard_list = (char *) 0;
    return;
}

extern const struct attack sa_yes[NATTK];
extern const struct attack sa_no[NATTK];

void
sysopt_seduce_set(val)
int val;
{
#if 0
/*
 * Attack substitution is now done on the fly in getmattk(mhitu.c).
 */
    struct attack *setval = val ? sa_yes : sa_no;
    int x;

    for (x = 0; x < NATTK; x++) {
        mons[PM_INCUBUS].mattk[x] = setval[x];
        //mons[PM_SUCCUBUS].mattk[x] = setval[x];
    }
#else
    nhUse(val);
#endif /*0*/
    return;
}


void
reset_global_variables(VOID_ARGS)
{
    wizard = discover = ModernMode = CasualMode = FALSE;
    save_initial_objects_values();
    save_initial_urolerace_values();
    save_initial_artifact_values();
#ifdef SYSFLAGS
    memset((genericptr_t)&sysflags, 0, sizeof(sysflags));
#endif
    memset((genericptr_t)&program_state, 0, sizeof(program_state));
    memset((genericptr_t)&iflags, 0, sizeof(iflags));
    memset((genericptr_t)&flags, 0, sizeof(flags)); // Just in case
    memset((genericptr_t)&youmonst, 0, sizeof(youmonst));
    memset((genericptr_t)&bhitpos, 0, sizeof(bhitpos));
    memset((genericptr_t)&urealtime, 0, sizeof(urealtime));
    memset((genericptr_t)&hearing_array, 0, sizeof(hearing_array));
    memset((genericptr_t)&fqn_prefix, 0, sizeof(fqn_prefix));
    memset((genericptr_t)&level, 0, sizeof(level));
    memset((genericptr_t)&m_shot, 0, sizeof(m_shot));
    memset((genericptr_t)&level_info, 0, sizeof(level_info));
    memset((genericptr_t)&chosen_windowtype, 0, sizeof(chosen_windowtype));
    memset((genericptr_t)&smeq, 0, sizeof(smeq));
    memset((genericptr_t)&Cmd, 0, sizeof(Cmd));

    WIN_MESSAGE = WIN_ERR;
    WIN_STATUS = WIN_ERR;
    WIN_MAP = WIN_ERR;
    WIN_INVEN = WIN_ERR;
    WIN_HERE = WIN_ERR;

    exit_hack_code = 0;

    saving = FALSE;
    check_pointing = FALSE;
    restoring = FALSE;
    ransacked = FALSE;
    occupation = 0;
    occsoundset = 0;
    occtyp = 0;
    afternmv = 0;
    hname = 0;
    hackpid = 0;
    multi = 0;
    multi_reason = 0;
    nroom = 0;
    nsubroom = 0;
    occtime = 0;
    in_doagain = 0;
    skip_savech = 0;
    save_cm = 0;
    yn_number = 0;
    done_money = 0;
    nomovemsg = 0;
    nomovemsg_attr = ATR_NONE;
    nomovemsg_color = NO_COLOR;
    tbx = tby = 0;
    defer_see_monsters = FALSE;
    in_mklev = FALSE;
    stoned = FALSE; /* done to monsters hit by 'c' */
    unweapon1 = FALSE;
    unweapon2 = FALSE;
    mrg_to_wielded = FALSE;
    in_steed_dismounting = FALSE;
    has_strong_rngseed = FALSE;
    ubirthday = 0;
    moves = 1L;
    monstermoves = 1L;
    wailmsg = 0L;
    disallow_keyboard_commands_in_wait_loop = FALSE;
    *dogname = 0;
    *catname = 0;
    *horsename = 0;
    *ramname = 0;
    *luggagename = 0;
    *wolfname = 0;
    doggender = 0;
    catgender = 0;
    horsegender = 0;
    ramgender = 0;
    wolfgender = 0;
    preferred_pet = 0;
    domove_attempting = 0L;
    domove_succeeded = 0L;
    vision_full_recalc = 0;
    viz_array = 0;
    hearing_full_recalc = 0;
    *toplines = 0;
    memset(toplineattrs, ATR_NONE, sizeof(toplineattrs));
    memset(toplinecolors, NO_COLOR, sizeof(toplinecolors));
    toplineattrs[sizeof(toplineattrs) - 1] = toplinecolors[sizeof(toplinecolors) - 1] = 0;
    upstairs_room = dnstairs_room = sstairs_room = 0;

    reset_item_global_variables();

    /* This needs to be setup already here early due to tiledata processing */
    int i;
    for (i = 0; i < NUM_OBJECTS; i++)
    {
        /* initialize object descriptions */
        objects[i].oc_name_idx = objects[i].oc_descr_idx = i;
    }

}

void
reset_item_global_variables(VOID_ARGS)
{
    invent = (struct obj*)0;
    uwep = (struct obj*)0;
    uarm = (struct obj*)0;
    uswapwep = (struct obj*)0;
    uswapwep2 = (struct obj*)0;
    uquiver = (struct obj*)0;
    uarmu = (struct obj*)0;
    uarmo = (struct obj*)0;
    uarmb = (struct obj*)0;
    uskin = (struct obj*)0;
    uarmc = (struct obj*)0;
    uarmh = (struct obj*)0;
    uarms = (struct obj*)0;
    uarmg = (struct obj*)0;
    uarmf = (struct obj*)0;
    uamul = (struct obj*)0;
    umisc = (struct obj*)0;
    umisc2 = (struct obj*)0;
    umisc3 = (struct obj*)0;
    umisc4 = (struct obj*)0;
    umisc5 = (struct obj*)0;
    uright = (struct obj*)0;
    uleft = (struct obj*)0;
    ublindf = (struct obj*)0;
    uchain = (struct obj*)0;
    uball = (struct obj*)0;
    current_wand = 0;  /* wand currently zapped/applied */
    thrownobj = 0;     /* object in flight due to throwing */
    kickedobj = 0;     /* object in flight due to kicking */
}

/*sys.c*/
