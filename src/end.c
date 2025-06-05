/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    end.c    $NHDT-Date: 1557094801 2019/05/05 22:20:01 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.170 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#define NEED_VARARGS /* comment line for pre-compiled headers */

#include "hack.h"
#include "lev.h"
#ifndef NO_SIGNAL
#include <signal.h>
#endif
#include <ctype.h>
#ifndef LONG_MAX
#include <limits.h>
#endif
#include "dlb.h"
#include <math.h>

/* add b to int64_t a, convert wraparound to max value */
#define nowrap_add(a, b) (a = ((a + game_score_difficulty_adjustment(b)) < 0 ? LONG_MAX : (a + game_score_difficulty_adjustment(b))))

#if 0
struct valuable_data {
    int64_t count;
    int typ;
};

STATIC_VAR struct valuable_data
    gems[LAST_GEM + 1 - FIRST_GEM + 1], /* 1 extra for glass */
    amulets[LAST_AMULET + 1 - FIRST_AMULET],
    miscellaneousitems[LAST_MISCITEM + 1 - FIRST_MISCITEM];

STATIC_VAR struct val_list {
    struct valuable_data *list;
    int size;
} valuables[] = { { gems, sizeof gems / sizeof *gems },
                  { amulets, sizeof amulets / sizeof *amulets },
                  { miscellaneousitems, sizeof miscellaneousitems / sizeof *miscellaneousitems },
                  { 0, 0 } };
#endif

#ifndef NO_SIGNAL
STATIC_PTR void FDECL(done_intr, (int));
#if defined(UNIX) || defined(VMS) || defined(__EMX__)
STATIC_DCL void FDECL(done_hangup, (int));
#endif
#endif
STATIC_DCL void FDECL(disclose, (int, BOOLEAN_P));
#if 0
STATIC_DCL void FDECL(get_valuables, (struct obj *));
STATIC_DCL void FDECL(sort_valuables, (struct valuable_data *, int));
STATIC_DCL void FDECL(artifact_score, (struct obj *, BOOLEAN_P, winid));
#endif
STATIC_DCL void FDECL(really_done, (int)) NORETURN;
STATIC_DCL void FDECL(savelife, (int));
STATIC_PTR int FDECL(CFDECLSPEC vanqsort_cmp, (const genericptr,
                                               const genericptr));
STATIC_DCL int NDECL(set_vanq_order);
STATIC_DCL void FDECL(list_vanquished, (CHAR_P, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL void FDECL(list_genocided, (CHAR_P, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL boolean FDECL(should_query_disclose_option, (int, char *));
#if defined (DUMPLOG) || defined (DUMPHTML)
STATIC_DCL void NDECL(dump_plines);
#endif
STATIC_DCL void FDECL(dump_everything, (int, time_t));
STATIC_DCL int NDECL(num_extinct);

STATIC_DCL void FDECL(reset_objchn, (struct obj*));
STATIC_DCL void FDECL(reset_monchn, (struct monst*));
STATIC_DCL void FDECL(reset_trapchn, (struct trap*));
STATIC_DCL void NDECL(reset_lev);
STATIC_DCL void NDECL(reset_levchn);
STATIC_DCL void NDECL(reset_damage);
STATIC_DCL void NDECL(reset_msghistory);
STATIC_DCL void NDECL(reset_remaining_dynamic_data);
STATIC_DCL void NDECL(reset_remaining_static_variables);

STATIC_DCL char FDECL(special_yn_query, (const char*, const char*));

#define done_stopprint program_state.stopprint

#ifndef PANICTRACE
#define NH_abort NH_abort_
#endif

#ifdef AMIGA
#define NH_abort_() Abort(0)
#else
#ifdef SYSV
#define NH_abort_() (void) abort()
#else
#ifdef WIN32
#define NH_abort_() win32_abort()
#else
#define NH_abort_() abort()
#endif
#endif /* !SYSV */
#endif /* !AMIGA */

#ifdef PANICTRACE
#include <errno.h>
#ifdef PANICTRACE_LIBC
#include <execinfo.h>
#endif

/* What do we try and in what order?  Tradeoffs:
 * libc: +no external programs required
 *        -requires newish libc/glibc
 *        -requires -rdynamic
 * gdb:   +gives more detailed information
 *        +works on more OS versions
 *        -requires -g, which may preclude -O on some compilers
 */
#ifdef SYSCF
#define SYSOPT_PANICTRACE_GDB sysopt.panictrace_gdb
#ifdef PANICTRACE_LIBC
#define SYSOPT_PANICTRACE_LIBC sysopt.panictrace_libc
#else
#define SYSOPT_PANICTRACE_LIBC 0
#endif
#else
#define SYSOPT_PANICTRACE_GDB (nh_getenv("NETHACK_USE_GDB") == 0 ? 0 : 2)
#ifdef PANICTRACE_LIBC
#define SYSOPT_PANICTRACE_LIBC 1
#else
#define SYSOPT_PANICTRACE_LIBC 0
#endif
#endif

STATIC_DCL void NDECL(NH_abort);
#ifndef NO_SIGNAL
STATIC_DCL void FDECL(panictrace_handler, (int));
#endif
STATIC_DCL boolean NDECL(NH_panictrace_libc);
STATIC_DCL boolean NDECL(NH_panictrace_gdb);

#ifndef NO_SIGNAL
/* called as signal() handler, so sent at least one arg */
/*ARGUSED*/
void
panictrace_handler(sig_unused)
int sig_unused UNUSED;
{
#define SIG_MSG "\nSignal received.\n"
    int f2;
    
    f2 = (int) write(2, SIG_MSG, sizeof SIG_MSG - 1);
    nhUse(f2);  /* what could we do if write to fd#2 (stderr) fails  */
    NH_abort(); /* ... and we're already in the process of quitting? */
}

void
panictrace_setsignals(set)
boolean set;
{
#define SETSIGNAL(sig) \
    (void) signal(sig, set ? (SIG_RET_TYPE) panictrace_handler : SIG_DFL);
#ifdef SIGILL
    SETSIGNAL(SIGILL);
#endif
#ifdef SIGTRAP
    SETSIGNAL(SIGTRAP);
#endif
#ifdef SIGIOT
    SETSIGNAL(SIGIOT);
#endif
#ifdef SIGBUS
    SETSIGNAL(SIGBUS);
#endif
#ifdef SIGFPE
    SETSIGNAL(SIGFPE);
#endif
#ifdef SIGSEGV
    SETSIGNAL(SIGSEGV);
#endif
#ifdef SIGSTKFLT
    SETSIGNAL(SIGSTKFLT);
#endif
#ifdef SIGSYS
    SETSIGNAL(SIGSYS);
#endif
#ifdef SIGEMT
    SETSIGNAL(SIGEMT);
#endif
#undef SETSIGNAL
}
#endif /* NO_SIGNAL */

STATIC_VAR boolean aborting = FALSE;
STATIC_OVL void
NH_abort()
{
    int gdb_prio = SYSOPT_PANICTRACE_GDB;
    int libc_prio = SYSOPT_PANICTRACE_LIBC;

    if (aborting)
        return;
    aborting = TRUE;

#ifndef VMS
    if (gdb_prio == libc_prio && gdb_prio > 0)
        gdb_prio++;

    if (gdb_prio > libc_prio) {
        (void) (NH_panictrace_gdb() || (libc_prio && NH_panictrace_libc()));
    } else {
        (void) (NH_panictrace_libc() || (gdb_prio && NH_panictrace_gdb()));
    }

#else /* VMS */
    /* overload otherwise unused priority for debug mode: 1 = show
       traceback and exit; 2 = show traceback and stay in debugger */
    /* if (wizard && gdb_prio == 1) gdb_prio = 2; */
    vms_traceback(gdb_prio);
    nhUse(libc_prio);

#endif /* ?VMS */

#ifndef NO_SIGNAL
    panictrace_setsignals(FALSE);
#endif
    NH_abort_();
}

STATIC_OVL boolean
NH_panictrace_libc()
{
#ifdef PANICTRACE_LIBC
    void *bt[20];
    size_t count, x;
    char **info;

    raw_print("Generating more information you may report:\n");
    count = backtrace(bt, SIZE(bt));
    info = backtrace_symbols(bt, count);
    for (x = 0; x < count; x++) {
        raw_printf("[%lu] %s", (unsigned long) x, info[x]);
    }
    /* free(info);   -- Don't risk it. */
    return TRUE;
#else
    return FALSE;
#endif /* !PANICTRACE_LIBC */
}

/*
 *   fooPATH  file system path for foo
 *   fooVAR   (possibly const) variable containing fooPATH
 */
#ifdef PANICTRACE_GDB
#ifdef SYSCF
#define GDBVAR sysopt.gdbpath
#define GREPVAR sysopt.greppath
#else /* SYSCF */
#define GDBVAR GDBPATH
#define GREPVAR GREPPATH
#endif /* SYSCF */
#endif /* PANICTRACE_GDB */

STATIC_OVL boolean
NH_panictrace_gdb()
{
#ifdef PANICTRACE_GDB
    /* A (more) generic method to get a stack trace - invoke
     * gdb on ourself. */
    char *gdbpath = GDBVAR;
    char *greppath = GREPVAR;
    char buf[BUFSZ];
    FILE *gdb;

    if (gdbpath == NULL || gdbpath[0] == 0)
        return FALSE;
    if (greppath == NULL || greppath[0] == 0)
        return FALSE;

    Sprintf(buf, "%s -n -q %s %d 2>&1 | %s '^#'", gdbpath, ARGV0, getpid(),
            greppath);
    gdb = popen(buf, "w");
    if (gdb) {
        raw_print("Generating more information you may report:\n");
        fprintf(gdb, "bt\nquit\ny");
        fflush(gdb);
        sleep(4); /* ugly */
        pclose(gdb);
        return TRUE;
    } else {
        return FALSE;
    }
#else
    return FALSE;
#endif /* !PANICTRACE_GDB */
}
#endif /* PANICTRACE */

/*
 * The order of these needs to match the macros in hack.h.
 */
STATIC_VAR NEARDATA const char *deaths[NUM_GAME_END_TYPES] = {
    /* the array of death */
    "died", "choked", "poisoned", "starvation", "drowning", "drowned", "burning",
    "dissolving under the heat and pressure", "crushed", "strangled", "suffocated", "turned to stone", "disintegrated",
    "turned into slime", "illness", "mummy rot", "genocided", "panic", "trickery", "quit",
    "escaped", "ascended", "snapshot"
};

STATIC_VAR NEARDATA const char *ends[NUM_GAME_END_TYPES] = {
    /* "when you %s" */
    "died", "choked", "were poisoned",
    "starved", "drowned", "were drowned", "burned",
    "dissolved in the lava",
    "were crushed", "were strangled", "suffocated", "turned to stone", "were disintegrated",
    "turned into slime", "died of your illness", "died of mummy rot", "were genocided",
    "panicked", "were tricked", "quit",
    "escaped", "ascended", "snapshot"
};

STATIC_VAR boolean Schroedingers_cat = FALSE;

/*ARGSUSED*/
void
done1(sig_unused) /* called as signal() handler, so sent at least one arg */
int sig_unused UNUSED;
{
#ifndef NO_SIGNAL
    (void) signal(SIGINT, SIG_IGN);
#endif
    if (flags.ignintr) {
#ifndef NO_SIGNAL
        (void) signal(SIGINT, (SIG_RET_TYPE) done1);
#endif
        clear_nhwindow(WIN_MESSAGE);
        curs_on_u();
        wait_synch();
        if (multi > 0)
            nomul(0);
    } else {
        (void) done2();
    }
}

/* "#quit" command or keyboard interrupt */
int
done2()
{
    if (iflags.debug_fuzzer)
        return 0;

    boolean has_existing_save_file = check_existing_save_file();
    char paranoidbuf[BUFSZ] = "";
    Sprintf(paranoidbuf, "%s%s%s", 
        !has_existing_save_file && (wizard || discover || CasualMode) ? "There is no saved game. " : "",
        !wizard && !has_existing_save_file ? "This will delete your character! " : "", 
        has_existing_save_file ? "Your progress will not be saved in the existing save file. Really quit?" : "Are you sure to quit?");
    if (!paranoid_query_ex(ATR_NONE, CLR_MSG_NEGATIVE, ParanoidQuit, "Confirm Quit", paranoidbuf)) {
#ifndef NO_SIGNAL
        (void) signal(SIGINT, (SIG_RET_TYPE) done1);
#endif
        clear_nhwindow(WIN_MESSAGE);
        curs_on_u();
        wait_synch();
        if (multi > 0)
            nomul(0);
        if (multi == 0) {
            u.uinvulnerable = FALSE; /* avoid ctrl-C bug -dlc */
            u.usleep = 0;
        }
        return 0;
    }
#if ((defined(UNIX) || defined(VMS) || defined(LATTICE)) && !defined(GNH_MOBILE))
    if (wizard) {
        int c;
#ifdef VMS
        extern int debuggable; /* sys/vms/vmsmisc.c, vmsunix.c */

        c = !debuggable ? 'n' : ynq2("Enter debugger?");
#else
#ifdef LATTICE
        c = ynq2("Create SnapShot?");
#else
        c = ynq2("Dump core?");
#endif
#endif
        if (c == 'y') {
#ifndef NO_SIGNAL
            (void) signal(SIGINT, (SIG_RET_TYPE) done1);
#endif
            exit_nhwindows((char *) 0);
            NH_abort();
        } else if (c == 'q')
            done_stopprint++;
    }
#endif
#ifndef LINT
    done(QUIT);
#endif
    return 0;
}

#ifndef NO_SIGNAL
/*ARGSUSED*/
STATIC_PTR void
done_intr(sig_unused) /* called as signal() handler, so sent at least 1 arg */
int sig_unused UNUSED;
{
    done_stopprint++;
    (void) signal(SIGINT, SIG_IGN);
#if defined(UNIX) || defined(VMS)
    (void) signal(SIGQUIT, SIG_IGN);
#endif
    return;
}

#if (defined(UNIX) || defined(VMS) || defined(__EMX__))  && !defined(GNH_MOBILE)
/* signal() handler */
STATIC_OVL void
done_hangup(sig)
int sig;
{
    program_state.done_hup++;
    sethanguphandler((void FDECL((*), (int) )) SIG_IGN);
    done_intr(sig);
    return;
}
#endif
#endif /* NO_SIGNAL */

void
get_killer_name_and_format(mtmp, buf, fmt_ptr)
struct monst* mtmp;
char* buf;
int* fmt_ptr;
{
    struct permonst* mptr = mtmp->data,
        * champtr = ((mtmp->cham >= LOW_PM)
            ? &mons[mtmp->cham]
            : mptr);
    boolean distorted = (boolean)(Hallucination && canspotmon(mtmp)),
        mimicker = (M_AP_TYPE(mtmp) == M_AP_MONSTER),
        imitator = (mptr != champtr || mimicker);


    buf[0] = '\0';
    *fmt_ptr = KILLED_BY_AN;
    /* "killed by the high priest of Crom" is okay,
       "killed by the high priest" alone isn't */
    if ((mptr->geno & G_UNIQ) != 0 && !(imitator && !mimicker)
        && !(mptr == &mons[PM_HIGH_PRIEST] && !mtmp->ispriest)) {
        if (!is_mname_proper_name(mptr))
            Strcat(buf, "the ");
        *fmt_ptr = KILLED_BY;
    }
    /* _the_ <invisible> <distorted> ghost of Dudley */
    if (mptr == &mons[PM_GHOST] && has_mname(mtmp)) {
        Strcat(buf, "the ");
        *fmt_ptr = KILLED_BY;
    }
    if (is_invisible(mtmp))
        Strcat(buf, "invisible ");
    if (distorted)
        Strcat(buf, "hallucinogen-distorted ");

    if (imitator) {
        char shape[BUFSZ];
        const char* realnm = pm_monster_name(champtr, mtmp->female), * fakenm = pm_monster_name(mptr, mtmp->female);
        boolean alt = is_vampshifter(mtmp);

        if (mimicker) {
            /* realnm is already correct because champtr==mptr;
               set up fake mptr for is_mname_proper_name/the_unique_pm */
            mptr = &mons[mtmp->mappearance];
            fakenm = pm_monster_name(mptr, mtmp->female);
        }
        else if (alt && strstri(realnm, "vampire")
            && !strcmp(fakenm, "vampire bat")) {
            /* special case: use "vampire in bat form" in preference
               to redundant looking "vampire in vampire bat form" */
            fakenm = "bat";
        }
        /* for the alternate format, always suppress any article;
           pname and the_unique should also have s_suffix() applied,
           but vampires don't take on any shapes which warrant that */
        if (alt || is_mname_proper_name(mptr)) /* no article */
            Strcpy(shape, fakenm);
        else if (the_unique_pm(mptr)) /* "the"; don't use the() here */
            Sprintf(shape, "the %s", fakenm);
        else /* "a"/"an" */
            Strcpy(shape, an(fakenm));
        /* omit "called" to avoid excessive verbosity */
        Sprintf(eos(buf),
            alt ? "%s in %s form"
            : mimicker ? "%s disguised as %s"
            : "%s imitating %s",
            realnm, shape);
        mptr = mtmp->data; /* reset for mimicker case */
    }
    else if (mptr == &mons[PM_GHOST]) {
        Strcat(buf, "ghost");
        if (has_mname(mtmp))
            Sprintf(eos(buf), " of %s", MNAME(mtmp));
    }
    else if (mtmp->isshk) {
        const char* shknm = true_shkname(mtmp), //True shopkeeper name, not hallucinated
            * honorific = shkname_is_pname(mtmp) ? ""
            : mtmp->female ? "Ms. " : "Mr. ";

        Sprintf(eos(buf), "%s%s, the shopkeeper", honorific, shknm);
        *fmt_ptr = KILLED_BY;
    }
    else if (mtmp->ispriest || mtmp->isminion) {
        /* m_monnam() suppresses "the" prefix plus "invisible", and
           it overrides the effect of Hallucination on priestname() */
        Strcat(buf, m_monnam(mtmp));
    }
    else {
        if (has_mname(mtmp) && mtmp->u_know_mname)
        {
            *fmt_ptr = KILLED_BY;
            Sprintf(eos(buf), "%s, the ", MNAME(mtmp));
        }
        Strcat(buf, pm_monster_name(mptr, mtmp->female));
        if (has_umname(mtmp))
            Sprintf(eos(buf), " called %s", UMNAME(mtmp));
    }
}

void
done_in_by(mtmp, how)
struct monst *mtmp;
int how;
{
    char buf[BUFSZ];
    Strcpy(buf, "");
    struct permonst* mptr = mtmp->data;

    //struct permonst *mptr = mtmp->data,
    //                *champtr = ((mtmp->cham >= LOW_PM)
    //                               ? &mons[mtmp->cham]
    //                               : mptr);
    //boolean distorted = (boolean) (Hallucination && canspotmon(mtmp)),
    //        mimicker = (M_AP_TYPE(mtmp) == M_AP_MONSTER),
    //        imitator = (mptr != champtr || mimicker);

    if(how == STONING)
        play_sfx_sound(SFX_PETRIFY);
    You_ex1(ATR_NONE, CLR_MSG_NEGATIVE, (how == STONING) ? "turn to stone..." : "die...");
    mark_synch(); /* flush buffered screen output */
   
    get_killer_name_and_format(mtmp, buf, &killer.format);

    //buf[0] = '\0';
    //killer.format = KILLED_BY_AN;
    ///* "killed by the high priest of Crom" is okay,
    //   "killed by the high priest" alone isn't */
    //if ((mptr->geno & G_UNIQ) != 0 && !(imitator && !mimicker)
    //    && !(mptr == &mons[PM_HIGH_PRIEST] && !mtmp->ispriest)) {
    //    if (!is_mname_proper_name(mptr))
    //        Strcat(buf, "the ");
    //    killer.format = KILLED_BY;
    //}
    ///* _the_ <invisible> <distorted> ghost of Dudley */
    //if (mptr == &mons[PM_GHOST] && has_mname(mtmp)) {
    //    Strcat(buf, "the ");
    //    killer.format = KILLED_BY;
    //}
    //if (is_invisible(mtmp))
    //    Strcat(buf, "invisible ");
    //if (distorted)
    //    Strcat(buf, "hallucinogen-distorted ");

    //if (imitator) {
    //    char shape[BUFSZ];
    //    const char *realnm = pm_monster_name(champtr, mtmp->female), *fakenm = pm_monster_name(mptr, mtmp->female);
    //    boolean alt = is_vampshifter(mtmp);

    //    if (mimicker) {
    //        /* realnm is already correct because champtr==mptr;
    //           set up fake mptr for is_mname_proper_name/the_unique_pm */
    //        mptr = &mons[mtmp->mappearance];
    //        fakenm = pm_monster_name(mptr, mtmp->female);
    //    } else if (alt && strstri(realnm, "vampire")
    //               && !strcmp(fakenm, "vampire bat")) {
    //        /* special case: use "vampire in bat form" in preference
    //           to redundant looking "vampire in vampire bat form" */
    //        fakenm = "bat";
    //    }
    //    /* for the alternate format, always suppress any article;
    //       pname and the_unique should also have s_suffix() applied,
    //       but vampires don't take on any shapes which warrant that */
    //    if (alt || is_mname_proper_name(mptr)) /* no article */
    //        Strcpy(shape, fakenm);
    //    else if (the_unique_pm(mptr)) /* "the"; don't use the() here */
    //        Sprintf(shape, "the %s", fakenm);
    //    else /* "a"/"an" */
    //        Strcpy(shape, an(fakenm));
    //    /* omit "called" to avoid excessive verbosity */
    //    Sprintf(eos(buf),
    //            alt ? "%s in %s form"
    //                : mimicker ? "%s disguised as %s"
    //                           : "%s imitating %s",
    //            realnm, shape);
    //    mptr = mtmp->data; /* reset for mimicker case */
    //} else if (mptr == &mons[PM_GHOST]) {
    //    Strcat(buf, "ghost");
    //    if (has_mname(mtmp))
    //        Sprintf(eos(buf), " of %s", MNAME(mtmp));
    //} else if (mtmp->isshk) {
    //    const char *shknm = true_shkname(mtmp), //True shopkeeper name, not hallucinated
    //               *honorific = shkname_is_pname(mtmp) ? ""
    //                               : mtmp->female ? "Ms. " : "Mr. ";

    //    Sprintf(eos(buf), "%s%s, the shopkeeper", honorific, shknm);
    //    killer.format = KILLED_BY;
    //} else if (mtmp->ispriest || mtmp->isminion) {
    //    /* m_monnam() suppresses "the" prefix plus "invisible", and
    //       it overrides the effect of Hallucination on priestname() */
    //    Strcat(buf, m_monnam(mtmp));
    //} else {
    //    if (has_mname(mtmp) && mtmp->u_know_mname)
    //    {
    //        killer.format = KILLED_BY;
    //        Sprintf(eos(buf), "%s, the ", MNAME(mtmp));
    //    }
    //    Strcat(buf, pm_monster_name(mptr, mtmp->female));
    //    if (has_umname(mtmp))
    //        Sprintf(eos(buf), " called %s", UMNAME(mtmp));
    //}

    if (how == DROWNED)
    {
        boolean moat = (levl[mtmp->mx][mtmp->my].typ != POOL)
            && (levl[mtmp->mx][mtmp->my].typ != WATER)
            && !Is_medusa_level(&u.uz)
            && !Is_waterlevel(&u.uz);

        Sprintf(eos(buf), " in a %s", moat ? "moat" : "pool of water");
    }

    Strcpy(killer.name, buf);
    /*
     * Chicken and egg issue:
     *  Ordinarily Unchanging ought to override something like this,
     *  but the transformation occurs at death.  With the current code,
     *  the effectiveness of Unchanging stops first, but a case could
     *  be made that it should last long enough to prevent undead
     *  transformation.  (Turning to slime isn't an issue here because
     *  Unchanging prevents that from happening.)
     */
    if (mptr == &mons[PM_SPECTRE])
        u.ugrave_arise = PM_SPECTRE;
    else if (mptr == &mons[PM_BARROW_WIGHT])
        u.ugrave_arise = PM_BARROW_WIGHT;
    else if (mptr->mlet == S_WRAITH)
        u.ugrave_arise = PM_WRAITH;
    else if (mptr->mlet == S_GREATER_UNDEAD && urace.mummynum != NON_PM)
        u.ugrave_arise = urace.mummynum;
    else if (mptr->mlet == S_VAMPIRE && Race_if(PM_HUMAN))
        u.ugrave_arise = PM_VAMPIRE;
    else if (mptr == &mons[PM_GHOUL])
        u.ugrave_arise = PM_GHOUL;
    /* this could happen if a high-end vampire kills the hero
       when ordinary vampires are genocided; ditto for wraiths */
    if (u.ugrave_arise >= LOW_PM
        && (mvitals[u.ugrave_arise].mvflags & MV_GENOCIDED))
        u.ugrave_arise = NON_PM;

    done(how);
    return;
}

/* some special cases for overriding while-helpless reason */
STATIC_VAR const struct {
    int why, unmulti;
    const char *exclude, *include;
} death_fixups[] = {
    /* "petrified by <foo>, while getting stoned" -- "while getting stoned"
       prevented any last-second recovery, but it was not the cause of
       "petrified by <foo>" */
    { STONING, 1, "getting stoned", (char *) 0 },
    /* "died of starvation, while fainted from lack of food" is accurate
       but sounds a fairly silly (and doesn't actually appear unless you
       splice together death and while-helpless from xlogfile) */
    { STARVING, 0, "fainted from lack of food", "fainted" },
};

/* clear away while-helpless when the cause of death caused that
   helplessness (ie, "petrified by <foo> while getting stoned") */
STATIC_DCL void
fixup_death(how)
int how;
{
    int i;

    if (multi_reason) {
        for (i = 0; i < SIZE(death_fixups); ++i)
            if (death_fixups[i].why == how
                && !strcmp(death_fixups[i].exclude, multi_reason)) {
                if (death_fixups[i].include) /* substitute alternate reason */
                    multi_reason = death_fixups[i].include;
                else /* remove the helplessness reason */
                    multi_reason = (char *) 0;
                if (death_fixups[i].unmulti) /* possibly hide helplessness */
                    multi = 0L;
                break;
            }
    }
}

#if defined(WIN32) && !defined(SYSCF)
#define NOTIFY_GNOLLHACK_BUGS
#endif

/*VARARGS1*/
void panic
VA_DECL(const char *, str)
{
    VA_START(str);
    VA_INIT(str, char *);

    if (program_state.panicking++)
        NH_abort(); /* avoid loops - this should never happen*/

    if (iflags.window_inited) {
        raw_print("\r\nOops...");
        wait_synch(); /* make sure all pending output gets flushed */
        exit_nhwindows((char *) 0);
        iflags.window_inited = 0; /* they're gone; force raw_print()ing */
    }

    raw_print(program_state.gameover
                  ? "Postgame wrapup disrupted."
                  : !program_state.something_worth_saving
                        ? "Program initialization has failed."
                        : "Suddenly, the dungeon collapses.");
#ifndef MICRO
#ifdef NOTIFY_GNOLLHACK_BUGS
    if (!wizard)
        raw_printf("Report the following error to \"%s\" or at \"%s\".",
                   DEVTEAM_EMAIL, DEVTEAM_URL);
    else if (program_state.something_worth_saving)
        raw_print("\nError save file being written.\n");
#else /* !NOTIFY_GNOLLHACK_BUGS */
    if (!wizard) {
        const char *maybe_rebuild = !program_state.something_worth_saving
                                     ? "."
                                     : ", and it may be possible to rebuild.";

        if (sysopt.support)
            raw_printf("To report this error, %s%s", sysopt.support,
                       maybe_rebuild);
        else if (sysopt.fmtd_wizard_list) /* formatted SYSCF WIZARDS */
            raw_printf("To report this error, contact %s%s",
                       sysopt.fmtd_wizard_list, maybe_rebuild);
        else
            raw_printf("Report error to \"%s\"%s", WIZARD_NAME,
                       maybe_rebuild);
    }
#endif /* ?NOTIFY_GNOLLHACK_BUGS */
    /* XXX can we move this above the prints?  Then we'd be able to
     * suppress "it may be possible to rebuild" based on dosave0()
     * or say it's NOT possible to rebuild. */
    if (program_state.something_worth_saving && !iflags.debug_fuzzer) {
        set_error_savefile();
        int saveres = dosave0(TRUE);
        if (saveres) {
            /* os/win port specific recover instructions */
            if (sysopt.recover)
                raw_printf("%s", sysopt.recover);
        }
    }
#endif /* !MICRO */
    {
        char buf[BUFSZ];

        Vsprintf(buf, str, VA_ARGS);
        raw_print(buf);
        paniclog("panic", buf);

#ifdef GNOLLHACK_MAIN_PROGRAM
        if (issue_gui_command)
        {
            char dbufs[BUFSZ * 18];
            Sprintf(dbufs, "panic: %s, P1:%s, P2:%s, P3:%s, P4:%s, B1:%s, B2:%s, B3:%s, B4:%s", buf, priority_debug_buf_1, priority_debug_buf_2, priority_debug_buf_3, priority_debug_buf_4, debug_buf_1, debug_buf_2, debug_buf_3, debug_buf_4);
            issue_debuglog_priority(0, dbufs);
        }

        if (open_special_view)
        {
            /* Add mode to posted panic */
            char mbuf[BUFSZ] = "";
            (void)describe_mode(mbuf);
            Sprintf(eos(buf), " [%s]", mbuf);

            struct special_view_info info = { 0 };
            info.viewtype = SPECIAL_VIEW_PANIC;
            info.text = buf;
            (void)open_special_view(info);
        }
        /* Special view now handles both sending the crash report and forum posting */
        //if (issue_gui_command)
        //    issue_gui_command(GUI_CMD_POST_DIAGNOSTIC_DATA, DIAGNOSTIC_DATA_PANIC, 0, buf);
#endif
    }
#ifdef WIN32
    interject(INTERJECT_PANIC);
#endif

#if 0
#if (defined(UNIX) || defined(VMS) || defined(LATTICE) || defined(WIN32)) && !defined(GNH_MOBILE)
    if (wizard)
        NH_abort(); /* generate core dump */
#endif
#endif
    VA_END();
    really_done(PANICKED);
}

STATIC_OVL boolean
should_query_disclose_option(category, defquery)
int category;
char *defquery;
{
    int idx;
    char disclose, *dop;

    *defquery = 'n';
    if ((dop = index(disclosure_options, category)) != 0) {
        idx = (int) (dop - disclosure_options);
        if (idx < 0 || idx >= NUM_DISCLOSURE_OPTIONS) {
            impossible(
                   "should_query_disclose_option: bad disclosure index %d %c",
                       idx, category);
            *defquery = DISCLOSE_PROMPT_DEFAULT_YES;
            return TRUE;
        }
        disclose = flags.end_disclose[idx];
        if (disclose == DISCLOSE_YES_WITHOUT_PROMPT) {
            *defquery = 'y';
            return FALSE;
        } else if (disclose == DISCLOSE_SPECIAL_WITHOUT_PROMPT) {
            *defquery = 'a';
            return FALSE;
        } else if (disclose == DISCLOSE_NO_WITHOUT_PROMPT) {
            *defquery = 'n';
            return FALSE;
        } else if (disclose == DISCLOSE_PROMPT_DEFAULT_YES) {
            *defquery = 'y';
            return TRUE;
        } else if (disclose == DISCLOSE_PROMPT_DEFAULT_SPECIAL) {
            *defquery = 'a';
            return TRUE;
        } else {
            *defquery = 'n';
            return TRUE;
        }
    }
    impossible("should_query_disclose_option: bad category %c", category);
    return TRUE;
}

#if defined (DUMPLOG) || defined (DUMPHTML)
STATIC_OVL void
dump_plines()
{
    int i, j;
    char buf[BUFSZ], buf2[BUFSZ], buf3[BUFSZ], ** strp;
    extern char *saved_plines[];
    extern char* saved_pline_attrs[];
    extern char* saved_pline_colors[];
    extern unsigned saved_pline_index;

    Strcpy(buf, " "); /* one space for indentation */
    *buf2 = ATR_NONE;
    *buf3 = NO_COLOR;
    putstr(0, ATR_HEADING, "Latest messages:");
    for (i = 0, j = (int) saved_pline_index; i < DUMPLOG_MSG_COUNT;
         ++i, j = (j + 1) % DUMPLOG_MSG_COUNT) {
        strp = &saved_plines[j];
        if (*strp) {
            copynchars(&buf[1], *strp, BUFSZ - 1 - 1);
            size_t len = strlen(&buf[1]);
            memcpy(&buf2[1], saved_pline_attrs[j], min(BUFSZ - 1, len));
            memcpy(&buf3[1], saved_pline_colors[j], min(BUFSZ - 1, len));
            putstr_ex2(0, buf, buf2, buf3, ATR_NONE, NO_COLOR, 0);
#ifdef FREE_ALL_MEMORY
            free(*strp), *strp = 0;
            free((genericptr_t)saved_pline_attrs[j]), saved_pline_attrs[j] = 0;
            free((genericptr_t)saved_pline_colors[j]), saved_pline_colors[j] = 0;
#endif
        }
    }
}
#endif


/*ARGSUSED*/
STATIC_OVL void
dump_everything(how, when)
int how;
time_t when; /* date+time at end of game */
{
#if defined (DUMPLOG) || defined (DUMPHTML)
    char pbuf[BUFSZ], datetimebuf[24]; /* [24]: room for 64-bit bogus value */

    dump_redirect(TRUE);
    if (!iflags.in_dumplog)
        return;

    //init_symbols(); /* Dumplog should now handle UTF-8 */ /* revert to default symbol set */

    /* one line version ID, which includes build date+time;
       it's conceivable that the game started with a different
       build date+time or even with an older GnollHack version,
       but we only have access to the one it finished under */
    putstr(0, ATR_SUBHEADING, getversionstring(pbuf));
    putstr(NHW_DUMPTXT, 0, "");

    /* game start and end date+time to disambiguate version date+time */
    Strcpy(datetimebuf, yyyymmddhhmmss(ubirthday));
    Sprintf(pbuf, "Game began %4.4s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s",
            &datetimebuf[0], &datetimebuf[4], &datetimebuf[6],
            &datetimebuf[8], &datetimebuf[10], &datetimebuf[12]);
    Strcpy(datetimebuf, yyyymmddhhmmss(when));
    Sprintf(eos(pbuf), ", %s %4.4s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s",
            how == SNAPSHOT ? "snapshot at" : "ended",
            &datetimebuf[0], &datetimebuf[4], &datetimebuf[6],
            &datetimebuf[8], &datetimebuf[10], &datetimebuf[12]);
    putstr(0, ATR_SUBHEADING, pbuf);
    putstr(NHW_DUMPTXT, 0, "");

    /* character name and basic role info */
    Sprintf(pbuf, "%s, %s %s %s %s", plname,
            aligns[1 - u.ualign.type].adj,
            genders[Ufemale].adj,
            urace.adj,
            (Ufemale && urole.name.f) ? urole.name.f : urole.name.m);
    putstr(0, ATR_SUBHEADING, pbuf);
    putstr(NHW_DUMPTXT, 0, "");

    dump_start_screendump();
    dump_map();
    putstr(NHW_DUMPTXT, 0, do_statusline1());
    putstr(NHW_DUMPTXT, 0, do_statusline2());
    if (iflags.wc2_statuslines > 2)
    {
        putstr(NHW_DUMPTXT, 0, do_statusline3());
    }
    if (iflags.wc2_statuslines > 3)
    {
        char partybuf[BUFSZ * 3];
        char partybuf2[BUFSZ * 3];
        char partybuf3[BUFSZ * 3];
        char partybuf4[BUFSZ * 3];
        char partybuf5[BUFSZ * 3];
        compose_partystatline(partybuf, partybuf2, partybuf3, partybuf4, partybuf5, BUFSZ * 3);
        char* partylines[5] = { partybuf, partybuf2, partybuf3, partybuf4, partybuf5 };
        int i;
        for (i = 0; i < iflags.wc2_statuslines - 3 && i < 5; i++)
        {
            putstr(NHW_DUMPTXT, 0, partylines[i]);
        }
    }
    status_initialize(TRUE);
    bot();
    dump_end_screendump();
    putstr(NHW_DUMPTXT, 0, "");

    dump_plines();
    putstr(NHW_DUMPTXT, 0, "");
    putstr(0, ATR_HEADING, "Inventory:");
    (void) display_inventory((char *) 0, TRUE, SHOWWEIGHTS_NONE);
    container_contents(invent, how != SNAPSHOT, TRUE, FALSE, SHOWWEIGHTS_NONE);
    magic_chest_contents(how != SNAPSHOT, TRUE, FALSE, SHOWWEIGHTS_NONE);
    enlightenment(how == SNAPSHOT ? BASICENLIGHTENMENT | GAMEENLIGHTENMENT : (BASICENLIGHTENMENT | MAGICENLIGHTENMENT | GAMEENLIGHTENMENT),
                  how == SNAPSHOT ? ENL_GAMEINPROGRESS : (how >= PANICKED) ? ENL_GAMEOVERALIVE : ENL_GAMEOVERDEAD);
    putstr(NHW_DUMPTXT, 0, "");
    dump_skills();
    putstr(NHW_DUMPTXT, 0, "");
    dump_spells();
    putstr(NHW_DUMPTXT, 0, "");
    show_gamelog(how == SNAPSHOT ? ENL_GAMEINPROGRESS : (how >= PANICKED) ? ENL_GAMEOVERALIVE : ENL_GAMEOVERDEAD);
    putstr(NHW_DUMPTXT, 0, "");
    list_vanquished('d', FALSE, TRUE); /* 'd' => 'y' */
    putstr(NHW_DUMPTXT, 0, "");
    list_genocided('d', FALSE, TRUE); /* 'd' => 'y' */
    putstr(NHW_DUMPTXT, 0, "");
    show_conduct(how == SNAPSHOT ? 0 : (how >= PANICKED) ? 1 : 2);
    putstr(NHW_DUMPTXT, 0, "");
    show_overview(how == SNAPSHOT ? 0 : (how >= PANICKED) ? 1 : 2, how);
    putstr(NHW_DUMPTXT, 0, "");
    dump_redirect(FALSE);
#else
    nhUse(how);
    nhUse(when);
#endif
}

/* #wizdumplog command - test dump_everything(). */
int
wiz_dumplog(VOID_ARGS)
{
#if defined (DUMPLOG) || defined (DUMPHTML)
    if (wizard) {
        time_t dumptime = getnow();
        char* dumplogfilename = 0;
        char* htmldumplogfilename = 0;
        char buf[BUFSIZ] = "";

        dump_open_log(dumptime, TRUE);

#if defined (DUMPLOG)
        dumplogfilename = print_dumplog_filename_to_buffer(buf);
#endif
        if (dumplogfilename)
            pline("Writing dumplog to %s...", dumplogfilename);

#if defined (DUMPHTML)
        htmldumplogfilename = print_dumphtml_filename_to_buffer(buf);
#endif
        if (htmldumplogfilename)
            pline("Writing HTML dumplog to %s...", buf);

        dump_everything(ASCENDED, dumptime);
        dump_close_log();
        pline1("Done.");
    }
    else
#endif
        pline(unavailcmd, visctrl((int)cmd_from_func(wiz_dumplog)));

    return 0;
}

int
dosnapshot(VOID_ARGS)
{
#if (defined (DUMPLOG) || defined (DUMPHTML)) && defined(ALLOW_SNAPSHOT)
    time_t dumptime = getnow();
    char* dumplogfilename = 0;
    char* htmldumplogfilename = 0;
    char* buf = (char*)alloc(BUFSIZ); /* BUFSIZ can be a big number, and perhaps cannot be allocated twice from the stack */
    char* htmlbuf = (char*)alloc(BUFSIZ);
    if (!buf || !htmlbuf)
        return 0;

    dump_open_log(dumptime, TRUE);

#if defined (DUMPLOG)
    dumplogfilename = print_dumplog_filename_to_buffer(buf);
#endif
#if defined (DUMPHTML)
    htmldumplogfilename = print_dumphtml_filename_to_buffer(htmlbuf);
#endif

    dump_everything(SNAPSHOT, dumptime);
    dump_close_log();

    write_snapshot_json(dumptime, dumplogfilename, htmldumplogfilename);

    free(buf);
    free(htmlbuf);
    if (windowprocs.wincap2 & WC2_SCREEN_TEXT)
    {
        display_screen_text("Snapshot taken", (const char*)0, (const char*)0, SCREEN_TEXT_SNAPSHOT, ATR_NONE, NO_COLOR, 0UL);
    }
    else
    {
        custompline(SUPPRESS_HISTORY, "%s", "Snapshot taken.");
    }
#else
    pline(unavailcmd, visctrl((int)cmd_from_func(dosnapshot)));
#endif

    return 0;
}



STATIC_OVL void
disclose(how, taken)
int how;
boolean taken;
{
    char c = '\0', defquery;
    char qbuf[QBUFSZ];
    boolean ask = FALSE;

    if (invent && !done_stopprint) {
        if (taken)
            Sprintf(qbuf, "Do you want to see what you had when you %s?",
                    (how == QUIT) ? "quit" : "died");
        else
            Strcpy(qbuf, "Do you want your possessions identified?");

        ask = should_query_disclose_option('i', &defquery);
        c = ask ? yn_function_end(qbuf, ynqchars, defquery, ynq2descs) : defquery;
        
        if (c == 'y') {
            /* caller has already ID'd everything */
            (void) display_inventory((char *) 0, FALSE, SHOWWEIGHTS_NONE);
            container_contents(invent, TRUE, TRUE, FALSE, SHOWWEIGHTS_NONE);
            magic_chest_contents(TRUE, TRUE, FALSE, SHOWWEIGHTS_NONE);
        }
        if (c == 'q')
            done_stopprint++;
    }

    if (!done_stopprint) {
        ask = should_query_disclose_option('a', &defquery);
        c = ask ? yn_function_end("Do you want to see your attributes?", ynqchars,
                              defquery, ynq2descs)
                : defquery;
        if (c == 'y')
            enlightenment((BASICENLIGHTENMENT | MAGICENLIGHTENMENT | GAMEENLIGHTENMENT),
                          (how >= PANICKED) ? ENL_GAMEOVERALIVE
                                            : ENL_GAMEOVERDEAD);
        if (c == 'q')
            done_stopprint++;
    }

    if (!done_stopprint) {
        ask = should_query_disclose_option('v', &defquery);
        list_vanquished(defquery, ask, TRUE);
    }

    if (!done_stopprint) {
        ask = should_query_disclose_option('g', &defquery);
        list_genocided(defquery, ask, TRUE);
    }

    if (!done_stopprint) {
        ask = should_query_disclose_option('c', &defquery);
        c = ask ? yn_function_end("Do you want to see your conduct?", ynqchars,
                              defquery, ynq2descs)
                : defquery;
        if (c == 'y')
            show_conduct((how >= PANICKED) ? 1 : 2);
        if (c == 'q')
            done_stopprint++;
    }

    if (!done_stopprint) {
        ask = should_query_disclose_option('o', &defquery);
        c = ask ? yn_function_end("Do you want to see the dungeon overview?",
                              ynqchars, defquery, ynq2descs)
                : defquery;
        if (c == 'y')
            show_overview((how >= PANICKED) ? 1 : 2, how);
        if (c == 'q')
            done_stopprint++;
    }
}

/* try to get the player back in a viable state after being killed */
STATIC_OVL void
savelife(how)
int how;
{
    int uhpmin = max(2 * u.ulevel, 10);

    if (u.uhpmax < uhpmin)
        u.uhpmax = uhpmin;
    u.uhp = u.uhpmax;
    if (Upolyd) /* Unchanging, or death which bypasses losing hit points */
        u.mh = u.mhmax;

    issue_simple_gui_command(GUI_CMD_CLEAR_CONDITION_TEXTS);

    if (u.uhunger < 500 || how == CHOKING) {
        init_uhunger();
    }
    /* cure impending doom of sickness hero won't have time to fix */
    if ((Sick & TIMEOUT) == 1L) {
        make_sick(0L, (char *) 0, FALSE, 0);
    }
    if ((FoodPoisoned & TIMEOUT) == 1L) {
        make_food_poisoned(0L, (char*)0, FALSE, 0);
    }
    if ((Confusion & TIMEOUT) == 1L) {
        make_confused(0L, FALSE);
    }
    if ((Stunned & TIMEOUT) == 1L) {
        make_stunned(0L, FALSE);
    }

    nomovemsg = "You survived that attempt on your life.";
    nomovemsg_color = CLR_MSG_SUCCESS;
    context.move = 0;
    if (multi > 0)
        multi = 0;
    else
        multi = -1;
    if (u.utrap && u.utraptype == TT_LAVA)
        reset_utrap(FALSE);
    context.botl = context.botlx = TRUE;
    u.ugrave_arise = NON_PM;
    HUnchanging = 0L;
    HSleeping = 0L;
    HParalyzed = 0L;
    curs_on_u();
    if (!context.mon_moving)
        endmultishot(FALSE);
    if (u.uswallow) {
        /* might drop hero onto a trap that kills her all over again */
        expels(u.ustuck, u.ustuck->data, TRUE);
    } else if (u.ustuck) {
        if (Upolyd && sticks(youmonst.data))
            You("release %s.", mon_nam(u.ustuck));
        else
            pline("%s releases you.", Monnam(u.ustuck));
        unstuck(u.ustuck);
    }
}

#if 0
/*
 * Get valuables from the given list.  Revised code: the list always remains
 * intact.
 */
STATIC_OVL void
get_valuables(list)
struct obj *list; /* inventory or container contents */
{
    register struct obj *obj;
    register int i;

    /* find amulets and gems, ignoring all artifacts */
    for (obj = list; obj; obj = obj->nobj)
        if (Has_contents(obj)) {
            get_valuables(obj->cobj);
        } else if (obj->oartifact) {
            continue;
        } else if (obj->oclass == AMULET_CLASS) {
            i = obj->otyp - FIRST_AMULET;
            if (!amulets[i].count) {
                amulets[i].count = obj->quan;
                amulets[i].typ = obj->otyp;
            } else
                amulets[i].count += obj->quan; /* always adds one */
        }
        else if (obj->oclass == MISCELLANEOUS_CLASS) {
            i = obj->otyp - FIRST_MISCITEM;
            if (!miscellaneousitems[i].count) {
                miscellaneousitems[i].count = obj->quan;
                miscellaneousitems[i].typ = obj->otyp;
            }
            else
                miscellaneousitems[i].count += obj->quan; /* always adds one */
        } else if (obj->oclass == GEM_CLASS && obj->otyp < LUCKSTONE) {
            i = min(obj->otyp, LAST_GEM + 1) - FIRST_GEM;
            if (!gems[i].count) {
                gems[i].count = obj->quan;
                gems[i].typ = obj->otyp;
            } else
                gems[i].count += obj->quan;
        }
    return;
}

/*
 *  Sort collected valuables, most frequent to least.  We could just
 *  as easily use qsort, but we don't care about efficiency here.
 */
STATIC_OVL void
sort_valuables(list, size)
struct valuable_data list[];
int size; /* max value is less than 20 */
{
    register int i, j;
    struct valuable_data ltmp;

    /* move greater quantities to the front of the list */
    for (i = 1; i < size; i++) {
        if (list[i].count == 0)
            continue;   /* empty slot */
        ltmp = list[i]; /* structure copy */
        for (j = i; j > 0; --j)
            if (list[j - 1].count >= ltmp.count)
                break;
            else {
                list[j] = list[j - 1];
            }
        list[j] = ltmp;
    }
    return;
}
#endif

#if 0
/*
 * odds_and_ends() was used for 3.6.0 and 3.6.1.
 * Schroedinger's Cat is handled differently starting with 3.6.2.
 */
STATIC_DCL boolean FDECL(odds_and_ends, (struct obj *, int));

#define CAT_CHECK 2

STATIC_OVL boolean
odds_and_ends(list, what)
struct obj *list;
int what;
{
    struct obj *otmp;

    for (otmp = list; otmp; otmp = otmp->nobj) {
        switch (what) {
        case CAT_CHECK: /* Schroedinger's Cat */
            /* Ascending is deterministic */
            if (SchroedingersBox(otmp))
                return rn2(2);
            break;
        }
        if (Has_contents(otmp))
            return odds_and_ends(otmp->cobj, what);
    }
    return FALSE;
}
#endif


#if 0
/* called twice; first to calculate total, then to list relevant items */
STATIC_OVL void
artifact_score(list, counting, endwin)
struct obj *list;
boolean counting; /* true => add up points; false => display them */
winid endwin;
{
    char pbuf[BUFSZ];
    struct obj *otmp;
    int64_t value, points;
    short dummy; /* object type returned by artifact_name() */

    for (otmp = list; otmp; otmp = otmp->nobj) {
        if (otmp->oartifact || otmp->otyp == BELL_OF_OPENING
            || otmp->otyp == SPE_BOOK_OF_THE_DEAD
            || otmp->otyp == CANDELABRUM_OF_INVOCATION) {
            value = arti_cost(otmp); /* zorkmid value */
            points = value * 5 / 2;  /* score value */
            if (counting) {
                nowrap_add(u.u_gamescore, points);
            } else {
                discover_object(otmp->otyp, TRUE, FALSE);
                otmp->known = otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
                /* assumes artifacts don't have quan > 1 */
                Sprintf(pbuf, "%s%s (worth %ld %s and %ld points)",
                        the_unique_obj(otmp) ? "The " : "",
                        otmp->oartifact ? artifact_name(xname(otmp), &dummy)
                                        : OBJ_NAME(objects[otmp->otyp]),
                        value, currency(value), points);
                putstr(endwin, 0, pbuf);
            }
        }
        if (Has_contents(otmp))
            artifact_score(otmp->cobj, counting, endwin);
    }
}
#endif

struct item_score_count_result
count_artifacts(list)
struct obj* list;
{
    struct obj* otmp;
    struct item_score_count_result cnt = { 0 };
    for (otmp = list; otmp; otmp = otmp->nobj) 
    {
        if (otmp->oartifact && (program_state.gameover || otmp->nknown || otmp->aknown))
        {
            cnt.quantity += otmp->quan;
            cnt.score += ARCHAEOLOGIST_PER_ARTIFACT_SCORE * otmp->quan;
        }
        if (Has_contents(otmp))
        {
            struct item_score_count_result cont_cnt = count_artifacts(otmp->cobj);
            cnt.quantity += cont_cnt.quantity;
            cnt.score += cont_cnt.score;
        }
    }
    return cnt;
}

struct item_score_count_result
count_historic_statues(list)
struct obj* list;
{
    struct obj* otmp;
    struct item_score_count_result cnt = { 0 };
    for (otmp = list; otmp; otmp = otmp->nobj)
    {
        if (otmp->otyp == STATUE && otmp->special_quality == SPEQUAL_STATUE_HISTORIC)
        {
            cnt.quantity += otmp->quan;
            cnt.score += ARCHAEOLOGIST_PER_HISTORIC_STATUE_SCORE * otmp->quan;
        }
        if (Has_contents(otmp))
        {
            struct item_score_count_result cont_cnt = count_historic_statues(otmp->cobj);
            cnt.quantity += cont_cnt.quantity;
            cnt.score += cont_cnt.score;
        }
    }
    return cnt;
}

struct item_score_count_result
count_valuable_art_objects(list)
struct obj* list;
{
    struct obj* otmp;
    struct item_score_count_result cnt = { 0 };
    for (otmp = list; otmp; otmp = otmp->nobj)
    {
        if (otmp->oclass == ART_CLASS)
        {
            cnt.quantity += otmp->quan;
            cnt.score += get_object_base_value(otmp) * otmp->quan;
        }
        if (Has_contents(otmp))
        {
            struct item_score_count_result cont_cnt = count_valuable_art_objects(otmp->cobj);
            cnt.quantity += cont_cnt.quantity;
            cnt.score += cont_cnt.score;
        }
    }
    return cnt;
}

struct item_score_count_result
count_powerful_melee_weapon_score(list)
struct obj* list;
{
    struct obj* otmp;
    struct item_score_count_result cnt = { 0 };
    for (otmp = list; otmp; otmp = otmp->nobj)
    {
        if (is_wieldable_weapon(otmp) && !is_appliable_pole_type_weapon(otmp)
            && !(is_launcher(otmp) || is_ammo(otmp) || is_missile(otmp))
            && ((otmp->oartifact && (program_state.gameover || otmp->nknown || otmp->aknown)) || (otmp->mythic_prefix && otmp->mythic_suffix)))
        {
            cnt.quantity += otmp->quan;
            cnt.score += BARBARIAN_PER_WEAPON_SCORE * otmp->quan;
        }
        if (Has_contents(otmp))
        {
            struct item_score_count_result cont_cnt = count_powerful_melee_weapon_score(otmp->cobj);
            cnt.quantity += cont_cnt.quantity;
            cnt.score += cont_cnt.score;
        }
    }
    return cnt;
}

struct item_score_count_result
count_powerful_ranged_weapon_score(list)
struct obj* list;
{
    struct obj* otmp;
    struct item_score_count_result cnt = { 0 };
    for (otmp = list; otmp; otmp = otmp->nobj)
    {
        if (is_wieldable_weapon(otmp)
            && (is_launcher(otmp) || is_ammo(otmp) || is_missile(otmp))
            && ((otmp->oartifact && (program_state.gameover || otmp->nknown || otmp->aknown)) || (otmp->mythic_prefix || otmp->mythic_suffix || otmp->exceptionality >= EXCEPTIONALITY_ELITE)))
        {
            cnt.quantity += otmp->quan;
            if (is_launcher(otmp))
            {
                cnt.quantity_nonammo += otmp->quan;
                cnt.score += RANGER_PER_LAUNCHER_SCORE * otmp->quan;
            }
            else
            {
                cnt.quantity_ammo += otmp->quan;
                cnt.score += RANGER_PER_AMMO_SCORE * otmp->quan;
            }
        }
        if (Has_contents(otmp))
        {
            struct item_score_count_result cont_cnt = count_powerful_ranged_weapon_score(otmp->cobj);
            cnt.quantity += cont_cnt.quantity;
            cnt.quantity_nonammo += cont_cnt.quantity_nonammo;
            cnt.quantity_ammo += cont_cnt.quantity_ammo;
            cnt.score += cont_cnt.score;
        }
    }
    return cnt;
}

struct item_score_count_result
count_powerful_Japanese_item_score(list)
struct obj* list;
{
    struct obj* otmp;
    struct item_score_count_result cnt = { 0 };
    for (otmp = list; otmp; otmp = otmp->nobj)
    {
        if (((otmp->oartifact && (program_state.gameover || otmp->nknown || otmp->aknown)) && (artilist[otmp->oartifact].aflags2 & AF2_JAPANESE) != 0)
            || ((otmp->mythic_prefix || otmp->mythic_suffix || otmp->exceptionality >= EXCEPTIONALITY_EXCEPTIONAL) && ((objects[otmp->otyp].oc_flags6 & O6_JAPANESE_ITEM) != 0 || Japanese_item_name(otmp->otyp) != 0))
           )
        {
            cnt.quantity += otmp->quan;
            if (is_ammo(otmp) || is_missile(otmp))
            {
                cnt.quantity_ammo += otmp->quan;
                cnt.score += SAMURAI_PER_AMMO_SCORE * otmp->quan;
            }
            else
            {
                cnt.quantity_nonammo += otmp->quan;
                cnt.score += SAMURAI_PER_ITEM_SCORE * otmp->quan;
            }
        }
        if (Has_contents(otmp))
        {
            struct item_score_count_result cont_cnt = count_powerful_Japanese_item_score(otmp->cobj);
            cnt.quantity += cont_cnt.quantity;
            cnt.quantity_nonammo += cont_cnt.quantity_nonammo;
            cnt.quantity_ammo += cont_cnt.quantity_ammo;
            cnt.score += cont_cnt.score;
        }
    }
    return cnt;
}

struct item_score_count_result
count_powerful_valkyrie_item_score(list)
struct obj* list;
{
    struct obj* otmp;
    struct item_score_count_result cnt = { 0 };
    for (otmp = list; otmp; otmp = otmp->nobj)
    {
        if (u.ualign.type == A_CHAOTIC ? otmp->exceptionality == EXCEPTIONALITY_INFERNAL : u.ualign.type == A_LAWFUL ? otmp->exceptionality == EXCEPTIONALITY_CELESTIAL : otmp->exceptionality == EXCEPTIONALITY_PRIMORDIAL)
        {
            cnt.quantity += otmp->quan;
            if (is_ammo(otmp) || is_missile(otmp))
            {
                cnt.quantity_ammo += otmp->quan;
                cnt.score += VALKYRIE_PER_AMMO_SCORE * otmp->quan;
            }
            else
            {
                cnt.quantity_nonammo += otmp->quan;
                cnt.score += VALKYRIE_PER_ITEM_SCORE * otmp->quan;
            }
        }
        if (Has_contents(otmp))
        {
            struct item_score_count_result cont_cnt = count_powerful_valkyrie_item_score(otmp->cobj);
            cnt.quantity += cont_cnt.quantity;
            cnt.quantity_nonammo += cont_cnt.quantity_nonammo;
            cnt.quantity_ammo += cont_cnt.quantity_ammo;
            cnt.score += cont_cnt.score;
        }
    }
    return cnt;
}

struct amulet_count_result
count_amulets(list)
struct obj* list;
{
    struct obj* otmp;
    struct amulet_count_result cnt = { 0, 0 };
    for (otmp = list; otmp; otmp = otmp->nobj)
    {
        if (otmp->otyp == AMULET_OF_LIFE_SAVING)
        {
            cnt.quantity += otmp->quan;
            cnt.amulets_of_life_saving += otmp->quan;
            cnt.score += CAVEMAN_PER_AMULET_OF_LIFE_SAVING_SCORE * otmp->quan;
        }
        else if (otmp->oclass == AMULET_CLASS && !objects[otmp->otyp].oc_pre_discovered && otmp->otyp != AMULET_OF_YENDOR && otmp->otyp != FAKE_AMULET_OF_YENDOR)
        {
            cnt.quantity += otmp->quan;
            cnt.other_amulets += otmp->quan;
            cnt.score += CAVEMAN_PER_OTHER_AMULET_SCORE * otmp->quan;
        }

        if (Has_contents(otmp))
        {
            struct amulet_count_result cont_cnt = count_amulets(otmp->cobj);
            cnt.score += cont_cnt.score;
            cnt.quantity += cont_cnt.quantity;
            cnt.amulets_of_life_saving += cont_cnt.amulets_of_life_saving;
            cnt.other_amulets += cont_cnt.other_amulets;
        }
    }
    return cnt;
}


/* Be careful not to call panic from here! */
void
done(how)
int how;
{
    boolean survive = FALSE;
    if (how == TRICKED) {
        if (killer.name[0]) {
            paniclog("trickery", killer.name);
            killer.name[0] = '\0';
            killer.hint_idx = 0;
        }
        (void)restore_backup_savefile(TRUE);
        if (wizard) {
            You1("are a very tricky wizard, it seems.");
            killer.format = KILLED_BY_AN; /* reset to 0 */
            return;
        }
    }
    if (program_state.panicking
#ifdef HANGUPHANDLING
        || program_state.done_hup
#endif
        ) {
        /* skip status update if panicking or disconnected */
        context.botl = context.botlx = iflags.time_botl = FALSE;
    } else {
        /* otherwise force full status update */
        context.botlx = TRUE;
        bot();
    }

#ifdef ANDROID
    if (how != TRICKED && how != QUIT && how != PANICKED && how != ESCAPED) {
        and_you_die();
    }
#endif

    if (iflags.debug_fuzzer)
    {
        if (!(program_state.panicking || how == PANICKED)) 
        {
            savelife(how);
            /* periodically restore characteristics and lost exp levels
               or cure lycanthropy */
            if (!rn2(10))
            {
                struct obj *potion = mksobj((u.ulycn > LOW_PM && !rn2(3))
                                            ? POT_WATER : POT_RESTORE_ABILITY,
                                            TRUE, FALSE, FALSE);

                bless(potion);
                (void) peffects(potion); /* always -1 for restore ability */
                /* not useup(); we haven't put this potion into inventory */
                Sprintf(priority_debug_buf_4, "done: %d", potion->otyp);
                obfree(potion, (struct obj *) 0);
            }
            killer.name[0] = '\0';
            killer.format = 0;
            killer.hint_idx = 0;
            return;
        }
    }
    else if (how == ASCENDED || (!killer.name[0] && how == GENOCIDED))
        killer.format = NO_KILLER_PREFIX;

    if (how < PANICKED)
    {
        add_glyph_buffer_layer_flags(u.ux, u.uy, 0UL, LMFLAGS_KILLED | (how == STONING ? LMFLAGS_STONED : 0UL));
        update_u_action(ACTION_TILE_DEATH);
        if(how != STONING && how != DISINTEGRATION)
            play_simple_monster_sound(&youmonst, MONSTER_SOUND_TYPE_DEATH);
        u_wait_until_action();
    }

    /* Avoid killed by "a" burning or "a" starvation */
    if (!killer.name[0] && (how == STARVING || how == BURNING))
        killer.format = KILLED_BY;
    if (!killer.name[0] || how >= PANICKED)
        Strcpy(killer.name, deaths[how]);

    if (how < PANICKED) {
        u.umortality++;
        /* in case caller hasn't already done this */
        if (u.uhp != 0 || (Upolyd && u.mh != 0)) {
            /* force HP to zero in case it is still positive (some
               deaths aren't triggered by loss of hit points), or
               negative (-1 is used as a flag in some circumstances
               which don't apply when actually dying due to HP loss) */
            u.uhp = u.mh = 0;
            context.botl = 1;
        }
    }
    if (Lifesaved && (how <= GENOCIDED)) 
    {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "But wait...");
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_LIFE_SAVED);
        special_effect_wait_until_action(0);
        if (HLifesaved)
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel the invisible hand of %s around you.", u_gname());
            if (how == CHOKING)
                You1("vomit ...");
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "much better!");
            u.uprops[LIFESAVED].intrinsic = u.uprops[LIFESAVED].intrinsic & ~TIMEOUT; //Set timeout to 0
        }
        else
        {
            if(uamul && uamul->otyp == AMULET_OF_LIFE_SAVING)
            {
                makeknown(AMULET_OF_LIFE_SAVING);
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "medallion %s!", !Blind ? "begins to glow" : "feels warm");
                if (how == CHOKING)
                    You1("vomit ...");
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "much better!");
                play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "medallion crumbles to dust!");
                if (uamul)
                {
                    Sprintf(priority_debug_buf_2, "done: %d", uamul->otyp);
                    useup(uamul);
                }
            }
            else
            {
                struct obj* lifesaver = what_gives(LIFESAVED);
                if (lifesaver)
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s!", Yname2(lifesaver), !Blind ? "begins to glow" : "feels warm");
                    if (how == CHOKING)
                        You1("vomit ...");
                    You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "much better!");
                    play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s crumbles to dust!", cxname(lifesaver));
                    if (lifesaver)
                    {
                        Sprintf(priority_debug_buf_2, "done2: %d", lifesaver->otyp);
                        useup(lifesaver);
                    }
                }
            }
            special_effect_wait_until_end(0);
        }
        (void) adjattrib(A_CON, -1, TRUE);
        savelife(how);
        if (how == GENOCIDED) {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Unfortunately you are still genocided...");
        } else {
            char killbuf[BUFSZ];
            formatkiller(killbuf, BUFSZ, how, FALSE);
            livelog_printf(LL_LIFESAVE, "averted death (%s)", killbuf);
            survive = TRUE;
        }
    }
    /* explore, modern, and wizard modes offer player the option to keep playing */
    if (!survive && how <= GENOCIDED)
    {
        u.utruemortality++;
        if ((wizard || discover) && !paranoid_query_ex(ATR_NONE, CLR_MSG_GOD, ParanoidDie, (char*)0, "Die?"))
        {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "OK, so you don't %s.", (how == CHOKING) ? "choke" : "die");
            savelife(how);
            survive = TRUE;
        }
        else if (ModernMode)
        {
            savelife(how);
            survive = TRUE;
            boolean teleinstead = FALSE;
            incr_itimeout(&HInvulnerable, 2);
            refresh_u_tile_gui_info(TRUE);
            if (!In_endgame(&u.uz))
            {
                d_level dl = { 0 };
                if (u.uachieve.amulet)
                {
                    dl = sanctum_level;
                }
                else
                {
                    dl.dlevel = 1;
                    dl.dnum = main_dungeon_dnum;
                }
                if (on_level(&u.uz, &dl))
                    teleinstead = TRUE;
                else
                    schedule_goto(&dl, 2, FALSE, FALSE, FALSE, 0, (char*)0, (char*)0);
            }
            else
            {
                teleinstead = TRUE;
            }

            if (teleinstead)
            {
                char wakeupbuf[BUFSZ] = "";
                context.reviving = TRUE;
                revival_at_altar(wakeupbuf);
                char* ptr;
                for (ptr = &u.urooms[0]; *ptr; ptr++) {
                    int roomno = *ptr - ROOMOFFSET, rt = rooms[roomno].rtype;
                    if (rt == NPCROOM)
                    {
                        in_npc_room(roomno + ROOMOFFSET, TRUE);
                        break;
                    }
                }
                revival_popup_message(wakeupbuf);
                context.reviving = FALSE;
            }
            death_hint();
        }
    }

    if (survive)
    {
        killer.name[0] = '\0';
        killer.format = KILLED_BY_AN; /* reset to 0 */
        killer.hint_idx = 0;
        if (how < PANICKED)
        {
            remove_glyph_buffer_layer_flags(u.ux, u.uy, 0UL, LMFLAGS_KILLED | LMFLAGS_STONED);
            update_u_action_revert(ACTION_TILE_NO_ACTION);
        }
        return;
    }

    /* Just in case revert to normal without updating screen */
    remove_glyph_buffer_layer_flags(u.ux, u.uy, 0UL, LMFLAGS_KILLED | LMFLAGS_STONED);
    u.action = ACTION_TILE_NO_ACTION;

    really_done(how);
    /*NOTREACHED*/
}

/* separated from done() in order to specify the __noreturn__ attribute */
STATIC_OVL void
really_done(how)
int how;
{
    boolean taken;
    char pbuf[BUFSZ];
    winid endwin = WIN_ERR;
    boolean bones_ok, have_windows = iflags.window_inited;
    struct obj *corpse = (struct obj *) 0;
    time_t endtime;
    int64_t umoney;
    boolean has_existing_save_file = (wizard || discover || CasualMode) && check_existing_save_file();
    boolean disclose_and_dumplog_ok = !(how < ASCENDED && CasualMode && has_existing_save_file);
    //int64_t tmp;

    /*
     *  The game is now over...
     */
    program_state.gameover = (how == ASCENDED ? 2 : 1);
    /* in case of a subsequent panic(), there's no point trying to save */
    program_state.something_worth_saving = 0;
#ifdef HANGUPHANDLING
    if (program_state.done_hup)
        done_stopprint++;
#endif
    /* render vision subsystem inoperative */
    iflags.vision_inited = 0;
    issue_simple_gui_command(GUI_CMD_GAME_ENDED);

    if (stop_all_sounds && how != ASCENDED)
    {
        struct stop_all_info sainfo = { 0 };
        stop_all_sounds(sainfo);
    }
    update_game_music();

    const char* endtext = 0;
    const char* endinfotext = 0;
    int screentextstyle = 0;
    int clr = NO_COLOR;

    switch (how)
    {
    case DIED:
    case CHOKING:
    case POISONING:
    case STARVING:
    case DROWNING:
    case DROWNED:
    case BURNING:
    case DISSOLVED:
    case CRUSHING:
    case STRANGULATION:
    case SUFFOCATION:
    case STONING:
    case DISINTEGRATION:
    case ILLNESS:
    case ROTTED:
    case GENOCIDED:
        endtext = "You are dead.";
        screentextstyle = SCREEN_TEXT_DEAD;
        clr = CLR_RED;
        break;
    case TURNED_SLIME:
        endtext = "You have turned into slime.";
        screentextstyle = SCREEN_TEXT_DEAD;
        clr = CLR_BRIGHT_GREEN;
        break;
    case PANICKED:
        endtext = "Panic!";
        endinfotext = "The cause has been recorded in the paniclog.";
        screentextstyle = SCREEN_TEXT_SPECIAL_END;
        clr = CLR_RED;
        break;
    case TRICKED:
        endtext = "Trickery!";
        endinfotext = "Your game may have become corrupted.";
        screentextstyle = SCREEN_TEXT_SPECIAL_END;
        clr = CLR_RED;
        break;
    case ESCAPED:
        endtext = "You escaped the dungeon.";
        screentextstyle = SCREEN_TEXT_ESCAPED;
        clr = CLR_YELLOW;
        break;
    case ASCENDED:
        endtext = "Ascended!";
        screentextstyle = SCREEN_TEXT_ASCENDED;
        clr = CLR_MAGENTA;
        break;
    case QUIT:
        if (!has_existing_save_file)
        {
            endtext = "You have quit.";
            screentextstyle = SCREEN_TEXT_QUIT;
            clr = CLR_RED;
        }
        break;
    default:
        break;
    }

    issue_simple_gui_command(GUI_CMD_CLEAR_CONDITION_TEXTS);
 
    if(endtext)
        display_screen_text(endtext, (const char*)0, (const char*)0, screentextstyle, ATR_NONE, clr, 0UL);

    if (how != PANICKED && how != TRICKED)
    {
        delete_tmp_backup_savefile();
        delete_backup_savefile();
        delete_error_savefile(); /* Normal end from a loaded game, so do�not leave any error save files behind, either */
    }

    /* might have been killed while using a disposable item, so make sure
       it's gone prior to inventory disclosure and creation of bones data */
    inven_inuse(TRUE);
    /* maybe not on object lists; if an active light source, would cause
       big trouble (`obj_is_local' panic) for savebones() -> savelev() */
    if (thrownobj && thrownobj->where == OBJ_FREE)
    {
        Sprintf(priority_debug_buf_4, "really_done: %d", thrownobj->otyp);
        obfree(thrownobj, (struct obj*)0);
    }
    if (kickedobj && kickedobj->where == OBJ_FREE)
    {
        Sprintf(priority_debug_buf_4, "really_done2: %d", kickedobj->otyp);
        obfree(kickedobj, (struct obj*)0);
    }

    /* remember time of death here instead of having bones, rip, and
       topten figure it out separately and possibly getting different
       time or even day if player is slow responding to --More-- */
    int64_t realtime;
    endtime = getnow();
    urealtime.finish_time = (int64_t)endtime;
    urealtime.realtime += ((int64_t)endtime - urealtime.start_timing);
    realtime = urealtime.realtime;
    issue_simple_gui_command(GUI_CMD_REPORT_PLAY_TIME);

    fixup_death(how); /* actually, fixup multi_reason */

    /* Write dumplog */
    if (disclose_and_dumplog_ok)
        dump_open_log(endtime, FALSE);

    char postbuf[BUFSZ * 3];
    Strcpy(postbuf, "");

    /* Write forum post and livelog */
    if (how < PANICKED || how == ASCENDED || how == ESCAPED)
    {
        if (how < PANICKED)
        {
            char killerbuf[BUFSZ * 2];
            formatkiller(killerbuf, sizeof killerbuf, how, TRUE);
            if (!*killerbuf)
                Strcpy(killerbuf, deaths[how]);
            Strcpy(postbuf, killerbuf);
        }
        else
        {
            if (how == ASCENDED)
                Strcpy(postbuf, "ascended to demigodhood");
            else if (how == ESCAPED)
                Strcpy(postbuf, "escaped the dungeon");
        }

        livelog_printf(LL_DUMP, "%s", postbuf);
    }

    You("were playing on %s difficulty in %s mode.", get_game_difficulty_text(context.game_difficulty),
        get_game_mode_text(TRUE));

    /* Sometimes you die on the first move.  Life's not fair.
     * On those rare occasions you get hosed immediately, go out
     * smiling... :-)  -3.
     */
    if (moves <= 1 && how < PANICKED && !done_stopprint)
        pline("Do not pass Go.  Do not collect 200 %s.", currency(200L));

    if (have_windows)
        wait_synch(); /* flush screen output */
#ifndef NO_SIGNAL
    (void) signal(SIGINT, (SIG_RET_TYPE) done_intr);
#if (defined(UNIX) || defined(VMS) || defined(__EMX__)) && !defined(GNH_MOBILE)
    (void) signal(SIGQUIT, (SIG_RET_TYPE) done_intr);
    sethanguphandler(done_hangup);
#endif
#endif /* NO_SIGNAL */

    bones_ok = (how < GENOCIDED) && can_make_bones();

    if (bones_ok && launch_in_progress())
        force_launch_placement();

    /* maintain ugrave_arise even for !bones_ok */
    if (how == PANICKED || how == TRICKED)
        u.ugrave_arise = (NON_PM - 3); /* no corpse, no grave */
    else if (how == BURNING || how == DISSOLVED || how == DISINTEGRATION) /* corpse burns up too */
        u.ugrave_arise = (NON_PM - 2); /* leave no corpse */
    else if (how == STONING)
        u.ugrave_arise = (NON_PM - 1); /* statue instead of corpse */
    else if (how == TURNED_SLIME
             /* it's possible to turn into slime even though green slimes
                have been genocided:  genocide could occur after hero is
                already infected or hero could eat a glob of one created
                before genocide; don't try to arise as one if they're gone */
             && !(mvitals[PM_GREEN_SLIME].mvflags & MV_GENOCIDED))
        u.ugrave_arise = PM_GREEN_SLIME;
    else if (how == ROTTED)
    {
        int montype = urace.mummynum;
        if(montype > NON_PM && !(mvitals[montype].mvflags & MV_GENOCIDED))
            u.ugrave_arise = montype;
    }

    if (how == QUIT) 
    {
        killer.format = NO_KILLER_PREFIX;
        if (u.uhp < 1) 
        {
            how = DIED;
            u.umortality++; /* skipped above when how==QUIT */
            u.utruemortality++; /* skipped above when how==QUIT */
            Strcpy(killer.name, "quit while already on Charon's boat");
        }
    }

    if (how == ESCAPED || how == PANICKED)
        killer.format = NO_KILLER_PREFIX;

    if (how != PANICKED && how != TRICKED)
    {
        boolean silently = done_stopprint ? TRUE : FALSE;

        /* these affect score and/or bones, but avoid them during panic */
        taken = paybill((how == ESCAPED) ? -1 : (how != QUIT), silently);
        paygd(silently);
        clearpriests();
        clearsmiths();
        clearnpcs();
    } 
    else
        taken = FALSE; /* lint; assert( !bones_ok ); */

    clearlocks();
    death_hint();

    if (have_windows)
        display_nhwindow(WIN_MESSAGE, FALSE);

    if (how != PANICKED && how != TRICKED && disclose_and_dumplog_ok)
    {
        struct obj *obj;

        /*
         * This is needed for both inventory disclosure and dumplog.
         * Both are optional, so do it once here instead of duplicating
         * it in both of those places.
         */
        for (obj = invent; obj; obj = obj->nobj) 
        {
            discover_object(obj->otyp, TRUE, FALSE);
            obj->known = obj->bknown = obj->dknown = obj->rknown = obj->nknown = obj->aknown = obj->mknown = 1;
            if (Is_container(obj) || obj->otyp == STATUE)
                obj->cknown = obj->lknown = obj->tknown = 1;
            /* we resolve Schroedinger's cat now in case of both
               disclosure and dumplog, where the 50:50 chance for
               live cat has to be the same both times */
            if (SchroedingersBox(obj))
            {
                if (!Schroedingers_cat)
                {
                    /* tell observe_quantum_cat() not to create a cat; if it
                       chooses live cat in this situation, it will leave the
                       SchroedingersBox flag set (for container_contents()) */
                    observe_quantum_cat(obj, FALSE, FALSE);
                    if (SchroedingersBox(obj))
                        Schroedingers_cat = TRUE;
                } 
                else
                    obj->speflags &= ~SPEFLAGS_SCHROEDINGERS_BOX; /* ordinary box with cat corpse in it */
            }
        }

        if (strcmp(flags.end_disclose, "none"))
            disclose(how, taken);

        dump_everything(how, endtime);
    }
    else if (how != QUIT)
    {
        char ebuf[BUFSZ];
        Sprintf(ebuf, "%s%s%s%s%s", 
            endtext ? endtext : "Your game is over.", 
            endinfotext ? " " : "", 
            endinfotext ? endinfotext : "", 
            has_existing_save_file ? " " : "", 
            has_existing_save_file ? "You can load the game from the point at which you last saved the game." : "");
        display_popup_text(ebuf, "Game Over", POPUP_TEXT_MESSAGE, ATR_NONE, clr, NO_GLYPH, POPUP_FLAGS_NONE);
    }

    /* if pets will contribute to score, populate mydogs list now
       (bones creation isn't a factor, but pline() messaging is; used to
       be done even sooner, but we need it to come after dump_everything()
       so that any accompanying pets are still on the map during dump) */
    if (how == ESCAPED)
        keepdogs(TRUE, TRUE); /* Just nearby pets following to the ground level */
    else if (how == ASCENDED)
        keepdogs(TRUE, FALSE); /* All pets surviving to the point of ascension */

    /* calculate score, before creating bones [container gold] */
    {
        umoney = money_cnt(invent);
        umoney += hidden_gold() + magic_gold(); /* accumulate gold from containers */
#if 0
        int deepest = deepest_lev_reached(FALSE);

        umoney = money_cnt(invent);
        tmp = u.umoney0;
        umoney += hidden_gold(); /* accumulate gold from containers */
        tmp = umoney - tmp;      /* net gain */

        if (tmp < 0L)
            tmp = 0L;
        if (how < PANICKED)
            tmp -= tmp / 10L;
        tmp += 50L * (int64_t) (deepest - 1);
        if (deepest > 20)
            tmp += 1000L * (int64_t) ((deepest > 30) ? 10 : deepest - 20);
#endif
        u.u_gamescore = get_current_game_score();

#if 0
        nowrap_add(u.u_gamescore, tmp);

        /* ascension gives a score bonus iff offering to original deity */
        if (how == ASCENDED && u.ualign.type == u.ualignbase[A_ORIGINAL]) 
        {
            /* retaining original alignment: score *= 2;
               converting, then using helm-of-OA to switch back: *= 1.5 */
            tmp = (u.ualignbase[A_CURRENT] == u.ualignbase[A_ORIGINAL])
                      ? u.u_gamescore
                      : (u.u_gamescore / 2L);
            nowrap_add(u.u_gamescore, tmp);
        }
#endif
    }

    if (u.ugrave_arise >= LOW_PM && !done_stopprint)
    {
        /* give this feedback even if bones aren't going to be created,
           so that its presence or absence doesn't tip off the player to
           new bones or their lack; it might be a lie if makemon fails */
        Your("%s as %s...",
             (u.ugrave_arise != PM_GREEN_SLIME)
                 ? "body rises from the dead"
                 : "revenant persists",
             an(pm_monster_name(&mons[u.ugrave_arise], flags.female)));
        display_nhwindow(WIN_MESSAGE, FALSE);
    }

    /* update gold for the rip output, which can't use hidden_gold()
       (containers will be gone by then if bones just got saved...) */
    done_money = umoney;

    /* clean up unneeded windows */
    if (have_windows) 
    {
        wait_synch();
        free_pickinv_cache(); /* extra persistent window if perm_invent */
        if (WIN_INVEN != WIN_ERR) 
        {
            destroy_nhwindow(WIN_INVEN),  WIN_INVEN = WIN_ERR;
            /* precaution in case any late update_inventory() calls occur */
            iflags.perm_invent = 0;
        }
        display_nhwindow(WIN_MESSAGE, TRUE);
        destroy_nhwindow(WIN_MAP),  WIN_MAP = WIN_ERR;
        if (WIN_STATUS != WIN_ERR)
            destroy_nhwindow(WIN_STATUS),  WIN_STATUS = WIN_ERR;
        destroy_nhwindow(WIN_MESSAGE),  WIN_MESSAGE = WIN_ERR;

        if (!done_stopprint || flags.tombstone)
        {
            struct extended_create_window_info info = extended_create_window_info_from_mon(&youmonst);
            if(how == ASCENDED)
                info.create_flags |= WINDOW_CREATE_FLAGS_ASCENDED;
            endwin = create_nhwindow_ex(NHW_TEXT, GHWINDOW_STYLE_OUTRIP, u_to_glyph(), info);
        }
        if (how < GENOCIDED && flags.tombstone && endwin != WIN_ERR)
            outrip(endwin, how, endtime);
    } 
    else
        done_stopprint = 1; /* just avoid any more output */

    if (disclose_and_dumplog_ok)
    {
        int dumpwin = endwin;
#if defined (DUMPLOG) || defined (DUMPHTML)
        /* 'how' reasons beyond genocide shouldn't show tombstone;
           for normal end of game, genocide doesn't either */
        dumpwin = NHW_DUMPTXT;
        if (how <= GENOCIDED)
        {
            dump_redirect(TRUE);
            if (iflags.in_dumplog)
                outrip(0, how, endtime);
            dump_redirect(FALSE);
        }
#endif
        if (u.uhave.amulet)
        {
            Strcat(killer.name, " (with the Amulet)");
        }
        else if (how == ESCAPED)
        {
            if (Is_astralevel(&u.uz)) /* offered Amulet to wrong deity */
                Strcat(killer.name, " (in celestial disgrace)");
            else if (carrying(FAKE_AMULET_OF_YENDOR))
                Strcat(killer.name, " (with a fake Amulet)");
            /* don't bother counting to see whether it should be plural */
        }

        Sprintf(pbuf, "%s %s the %s...", Goodbye(), plname,
            (how != ASCENDED)
            ? (const char*)((flags.female && urole.name.f)
                ? urole.name.f
                : urole.name.m)
            : (const char*)(flags.female ? "Demigoddess" : "Demigod"));
        dump_forward_putstr(endwin, ATR_HEADING, pbuf, done_stopprint, 0);
        dump_forward_putstr(dumpwin, 0, "", done_stopprint, 0);

        if (how == ESCAPED || how == ASCENDED)
        {
            struct monst* mtmp;

#if 0
            struct obj* otmp;
            register struct val_list* val;
            register int i;

            for (val = valuables; val->list; val++)
                for (i = 0; i < val->size; i++)
                {
                    val->list[i].count = 0L;
                }

            get_valuables(invent);

            /* add points for collected valuables */
            for (val = valuables; val->list; val++)
                for (i = 0; i < val->size; i++)
                    if (val->list[i].count != 0L)
                    {
                        tmp = val->list[i].count
                            * objects[val->list[i].typ].oc_cost;
                        nowrap_add(u.u_gamescore, tmp);
                    }

            /* count the points for artifacts */
            artifact_score(invent, TRUE, endwin);
#endif

            viz_array[0][0] |= IN_SIGHT; /* need visibility for naming */

            mtmp = mydogs;
            int petcount = 0;
            while (mtmp)
            {
                petcount++;
                mtmp = mtmp->nmon;
            }
            if (Schroedingers_cat)
                petcount++;

            mtmp = mydogs;
            Strcpy(pbuf, "You");
            if (mtmp || Schroedingers_cat)
            {
                dump_forward_putstr(endwin, ATR_NONE, pbuf, done_stopprint, 1);
                int petindex = 0;
                while (mtmp)
                {
                    Sprintf(pbuf, "%s %s",
                        petcount == 1 ? " and" : petindex < petcount - 1 ? "," : ", and",
                       mon_nam(mtmp));
                    mtmp = mtmp->nmon;
                    petindex++;
                    dump_forward_putstr(endwin, ATR_NONE, pbuf, done_stopprint, 1);
                }

                /* [it might be more robust to create a housecat and add it to
                   mydogs; it doesn't have to be placed on the map for that] */
                pbuf[0] = '\0';
                if (Schroedingers_cat)
                {
                    Sprintf(pbuf, "%s Schroedinger's cat", petcount == 1 ? " and" : ", and");
                }
                Strcat(pbuf, " ");
            }
            else
            {
                Strcat(pbuf, " ");
            }
            Sprintf(eos(pbuf), "%s with %lld point%s, ",
                how == ASCENDED ? "went to your reward"
                : "escaped from the dungeon",
                (long long)u.u_gamescore, plur(u.u_gamescore));
            dump_forward_putstr(endwin, ATR_NONE, pbuf, done_stopprint, 1);

#if 0
            if (!done_stopprint)
                artifact_score(invent, FALSE, endwin); /* list artifacts */

#if defined (DUMPLOG) || defined (DUMPHTML)
            dump_redirect(TRUE);
            if (iflags.in_dumplog)
                artifact_score(invent, FALSE, 0);
            dump_redirect(FALSE);
#endif

            /* list valuables here */
            for (val = valuables; val->list; val++)
            {
                sort_valuables(val->list, val->size);
                for (i = 0; i < val->size && !done_stopprint; i++)
                {
                    int typ = val->list[i].typ;
                    int64_t count = val->list[i].count;

                    if (count == 0L)
                        continue;

                    if (objects[typ].oc_class != GEM_CLASS || typ <= LAST_GEM)
                    {
                        otmp = mksobj(typ, FALSE, FALSE, FALSE);
                        discover_object(otmp->otyp, TRUE, FALSE);
                        otmp->known = 1;  /* for fake amulets */
                        otmp->dknown = 1; /* seen it (blindness fix) */
                        if (has_oname(otmp))
                            free_oname(otmp);
                        otmp->quan = count;
                        Sprintf(pbuf, "%8ld %s (worth %ld %s),", count,
                            xname(otmp), count * objects[typ].oc_cost,
                            currency(2L));
                        obfree(otmp, (struct obj*)0);
                    }
                    else
                    {
                        Sprintf(pbuf, "%8ld worthless piece%s of colored glass, ",
                            count, plur(count));
                    }
                    dump_forward_putstr(endwin, ATR_NONE, pbuf, 0);
                }
            }
#endif

        }
        else
        {
            /* did not escape or ascend */
            if (u.uz.dnum == 0 && u.uz.dlevel <= 0)
            {
                /* level teleported out of the dungeon; `how' is DIED,
                   due to falling or to "arriving at heaven prematurely" */
                Sprintf(pbuf, "You %s beyond the confines of the dungeon",
                    (u.uz.dlevel < 0) ? "passed away" : ends[how]);
            }
            else
            {
                /* more conventional demise */
                const char* where = dungeons[u.uz.dnum].dname;

                if (Is_astralevel(&u.uz))
                    where = "The Astral Plane";
                Sprintf(pbuf, "You %s in %s", ends[how], where);
                if (!In_endgame(&u.uz) && !Is_knox(&u.uz))
                    Sprintf(eos(pbuf), " on dungeon level %d",
                        In_quest(&u.uz) ? dunlev(&u.uz) : depth(&u.uz));
            }

            Sprintf(eos(pbuf), " with %lld point%s, ", (long long)u.u_gamescore, plur(u.u_gamescore));
            dump_forward_putstr(endwin, ATR_NONE, pbuf, done_stopprint, 1);
        }

        Sprintf(pbuf, "and %lld piece%s of gold, after %lld move%s.", (long long)umoney,
            plur(umoney), (long long)moves, plur(moves));
        dump_forward_putstr(endwin, ATR_NONE, pbuf, done_stopprint, 0);

        char realtimebuf[BUFSZ] = "";
        print_realtime(realtimebuf, realtime);
        Sprintf(pbuf, "You played on %s difficulty in %s mode for %s.", get_game_difficulty_text(context.game_difficulty),
            get_game_mode_text(TRUE), realtimebuf);
        dump_forward_putstr(endwin, ATR_NONE, pbuf, done_stopprint, 0);
        if (!n_game_recoveries)
            Strcpy(pbuf, "The dungeon never collapsed on you.");
        else
            Sprintf(pbuf, "The dungeon collapsed on you %llu time%s.", (unsigned long long)n_game_recoveries, plur(n_game_recoveries));
        dump_forward_putstr(endwin, ATR_NONE, pbuf, done_stopprint, 0);
        Sprintf(pbuf,
            "You were level %d with a maximum of %d hit point%s when you %s.",
            u.ulevel, u.uhpmax, plur(u.uhpmax), ends[how]);
        dump_forward_putstr(endwin, ATR_NONE, pbuf, done_stopprint, 0);
        dump_forward_putstr(endwin, ATR_NONE, "", done_stopprint, 0);

        if (!done_stopprint)
            display_nhwindow(endwin, TRUE);
        if (endwin != WIN_ERR)
            destroy_nhwindow(endwin);

        dump_close_log();
    }

    /* Dumplog is closed so can post it now */
    if (*postbuf)
    {
        IfModeAllowsPostToForum
        {
    #if defined (DUMPLOG) || defined (DUMPHTML)
            char dlbuf[BUFSZ * 4];
            char* dlfilename = print_dumplog_filename_to_buffer(dlbuf);
            if (dlfilename)
            {
                issue_gui_command(GUI_CMD_POST_GAME_STATUS, GAME_STATUS_RESULT_ATTACHMENT, GAME_STATUS_ATTACHMENT_DUMPLOG_TEXT, dlfilename);
            }
    #if defined(DUMPHTML)
            dlfilename = print_dumphtml_filename_to_buffer(dlbuf);
            if (dlfilename)
            {
                issue_gui_command(GUI_CMD_POST_GAME_STATUS, GAME_STATUS_RESULT_ATTACHMENT, GAME_STATUS_ATTACHMENT_DUMPLOG_HTML, dlfilename);
            }
    #endif
    #endif
            char totalpostbuf[BUFSZ * 4];
            char mbuf[BUFSZ] = "";
            char cbuf[BUFSZ];
            (void)describe_mode(mbuf);
            Sprintf(cbuf, "%.3s %.3s %.3s %.3s XL:%d", urole.filecode,
                urace.filecode, genders[flags.female].filecode,
                aligns[1 - u.ualign.type].filecode, u.ulevel);
            char* duration = format_duration_with_units(realtime);
            Sprintf(totalpostbuf, "%s (%s), %lld point%s, T:%lld (%s), %s [%s]", plname, cbuf, (long long)u.u_gamescore, plur(u.u_gamescore), (long long)moves, duration, postbuf, mbuf);
            issue_gui_command(GUI_CMD_POST_GAME_STATUS, GAME_STATUS_RESULT, how, totalpostbuf);
        }
    }

    /* finish_paybill should be called after disclosure but before bones */
    if (bones_ok && taken)
        finish_paybill();

    /* grave creation should be after disclosure so it doesn't have
       this grave in the current level's features for #overview */
    if (bones_ok && u.ugrave_arise == NON_PM
        && !(mvitals[u.umonnum].mvflags & MV_NOCORPSE))
    {
        int mnum = u.umonnum;

        if (!Upolyd)
        {
            /* Base corpse on race when not poly'd since original
             * u.umonnum is based on role, and all role monsters
             * are human.
             */
            mnum = urace.monsternum;
        }
        corpse = mk_named_object(CORPSE, &mons[mnum], u.ux, u.uy, plname);
        corpse->nknown = 1;
        Sprintf(pbuf, "%s, ", plname);
        formatkiller(eos(pbuf), sizeof pbuf - strlen(pbuf), how, TRUE);
        make_grave(u.ux, u.uy, pbuf, TRUE);
        pbuf[0] = '\0'; /* clear grave text; also lint suppression */
    }

    if (bones_ok)
    {
        if (!wizard || paranoid_query_ex(ATR_NONE, NO_COLOR, ParanoidBones, (char*)0, "Save bones?"))
            savebones(how, endtime, corpse);
        /* corpse may be invalid pointer now so
            ensure that it isn't used again */
        corpse = (struct obj*)0;
    }

    /* "So when I die, the first thing I will see in Heaven is a
     * score list?" */
    if (have_windows && !iflags.toptenwin)
        exit_nhwindows((char*)0), have_windows = FALSE;

    //Should exclude games from imported files?
    if(((!discover && !CasualMode) || (CasualMode && how == ASCENDED)) && !program_state.panicking &&
        (!wizard || special_yn_query("Write Top Scores", "Write a top score entry?") == 'y'))
        topten(how, endtime);

    if (CasualMode && how == ASCENDED && has_existing_save_file)
        (void)delete_savefile(); /* The casual mode character gets deleted only upon ascension */

    if (have_windows)
        exit_nhwindows((char*)0);

    if (done_stopprint) 
    {
        raw_print("");
        raw_print("");
    }
    livelog_dump_url(LL_DUMP_ALL | (how == ASCENDED ? LL_DUMP_ASC : 0));
    nh_terminate(EXIT_SUCCESS);
}

void
container_contents(list, identified, all_containers, reportempty, show_weights)
struct obj *list;
boolean identified, all_containers, reportempty;
int show_weights;
{
    register struct obj *box, *obj;
    char buf[BUFSZ];
    boolean cat, dumping = iflags.in_dumplog;
    int count = 0;
    int totalweight = 0;
    boolean loadstonecorrectly = loadstone_weight_shown_correctly(show_weights);

    for (box = list; box; box = box->nobj) 
    {
        if (Is_container(box) || box->otyp == STATUE) 
        {
            if (dumping)
            {
                if (identified)
                    box->cknown = 1; /* we're looking at the contents now */
                else if (!box->cknown) /* Do not list containers whose contents we do not know */
                    continue;
            }
            else
                box->cknown = 1; /* we're looking at the contents now */
            if (identified)
                box->lknown = 1;
            if (Is_noncontainer(box) /*->otyp == BAG_OF_TRICKS*/) 
            {
                continue; /* wrong type of container */
            } 
            else if (contained_object_chain(box))
            {
                winid tmpwin = create_nhwindow(NHW_MENU);
                Loot *sortedcobj, *srtc;
                unsigned sortflags;

                /* at this stage, the SchroedingerBox() flag is only set
                   if the cat inside the box is alive; the box actually
                   contains a cat corpse that we'll pretend is not there;
                   for dead cat, the flag will be clear and there'll be
                   a cat corpse inside the box; either way, inventory
                   reports the box as containing "1 item" */
                cat = SchroedingersBox(box);
                count = 0;

                Sprintf(buf, "Contents of %s:", the(xname(box)));
                putstr(tmpwin, ATR_TITLE, buf);
                if (!dumping)
                    putstr(tmpwin, ATR_HALF_SIZE, " ");
                buf[0] = buf[1] = ' '; /* two leading spaces */
                if (contained_object_chain(box) && !cat)
                {
                    sortflags = (((flags.sortloot == 'l'
                                   || flags.sortloot == 'f')
                                     ? SORTLOOT_LOOT : 0)
                                 | (flags.sortpack ? SORTLOOT_PACK : 0));
                    sortedcobj = sortloot(contained_object_chain_ptr(box), sortflags, FALSE,
                                          (boolean FDECL((*), (OBJ_P))) 0);
                    totalweight = 0;
                    for (srtc = sortedcobj; ((obj = srtc->obj) != 0); ++srtc) 
                    {
                        if (identified) 
                        {
                            discover_object(obj->otyp, TRUE, FALSE);
                            obj->known = obj->bknown = obj->dknown
                                = obj->rknown = obj->nknown = obj->aknown = obj->mknown = 1;
                            if (Is_container(obj) || obj->otyp == STATUE)
                                obj->cknown = obj->lknown = obj->tknown = 1;
                        }
                        count++;

                        /* total sum here */
                        if (obj->otyp == LOADSTONE && !loadstonecorrectly)
                            totalweight += objects[LUCKSTONE].oc_weight;
                        else
                            totalweight += obj->owt;
    
                        Sprintf(&buf[2], "%2d - %s", count, show_weights > SHOWWEIGHTS_NONE ? (flags.inventory_weights_last ? doname_with_price_and_weight_last(obj, loadstonecorrectly) : doname_with_price_and_weight_first(obj, loadstonecorrectly)) : doname_with_price(obj));
                        //Strcpy(&buf[2], doname_with_price_and_weight_first(obj));
                        putstr(tmpwin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                    }
                    if (flags.show_weight_summary)
                    {
                        if (flags.inventory_weights_last)
                            putstr(tmpwin, ATR_HALF_SIZE, " ");
                        add_weight_summary_putstr(tmpwin, totalweight, show_weights);
                    }

                    unsortloot(&sortedcobj);
                } 
                else if (cat) 
                {
                    Strcpy(&buf[2], "Schroedinger's cat!");
                    putstr(tmpwin, 0, buf);
                }
                if (dumping)
                    putstr(0, ATR_HALF_SIZE, " ");
                display_nhwindow(tmpwin, TRUE);
                destroy_nhwindow(tmpwin);
                if (all_containers)
                    container_contents(contained_object_chain(box), identified, TRUE,
                                       reportempty, show_weights);
            } 
            else if (reportempty) 
            {
                pline("%s is empty.", upstart(thesimpleoname(box)));
                display_nhwindow(WIN_MESSAGE, FALSE);
            }
        }
        if (!all_containers)
            break;
    }
}

void
magic_chest_contents(identified, all_containers, reportempty, show_weights)
boolean identified, all_containers, reportempty;
int show_weights;
{
    register struct obj* obj;
    char buf[BUFSZ];
    boolean dumping = iflags.in_dumplog;
    int count = 0;
    int totalweight = 0;
    boolean loadstonecorrectly = loadstone_weight_shown_correctly(show_weights);
    const char* chest_name = objects[MAGIC_CHEST].oc_name_known || identified ? OBJ_NAME(objects[MAGIC_CHEST]) : OBJ_DESCR(objects[MAGIC_CHEST]);

    if (magic_objs)
    {
        winid tmpwin = create_nhwindow(NHW_MENU);
        Loot* sortedcobj, * srtc;
        unsigned sortflags;

        count = 0;

        Sprintf(buf, "Contents of your %s:", chest_name);
        putstr(tmpwin, ATR_TITLE, buf);
        if (!dumping)
            putstr(tmpwin, ATR_HALF_SIZE, " ");
        buf[0] = buf[1] = ' '; /* two leading spaces */
        if (magic_objs)
        {
            sortflags = (((flags.sortloot == 'l'
                || flags.sortloot == 'f')
                ? SORTLOOT_LOOT : 0)
                | (flags.sortpack ? SORTLOOT_PACK : 0));
            sortedcobj = sortloot(&magic_objs, sortflags, FALSE,
                (boolean FDECL((*), (OBJ_P))) 0);
            totalweight = 0;
            for (srtc = sortedcobj; ((obj = srtc->obj) != 0); ++srtc)
            {
                if (identified)
                {
                    discover_object(obj->otyp, TRUE, FALSE);
                    obj->known = obj->bknown = obj->dknown
                        = obj->rknown = obj->nknown = obj->aknown = obj->mknown = 1;
                    if (Is_container(obj) || obj->otyp == STATUE)
                        obj->cknown = obj->lknown = obj->tknown = 1;
                }
                count++;

                /* total sum here */
                if (obj->otyp == LOADSTONE && !loadstonecorrectly)
                    totalweight += objects[LUCKSTONE].oc_weight;
                else
                    totalweight += obj->owt;

                Sprintf(&buf[2], "%2d - %s", count, show_weights > SHOWWEIGHTS_NONE ? (flags.inventory_weights_last ? doname_with_price_and_weight_last(obj, loadstonecorrectly) : doname_with_price_and_weight_first(obj, loadstonecorrectly)) : doname_with_price(obj));
                //Strcpy(&buf[2], doname_with_price_and_weight_first(obj));
                putstr(tmpwin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (flags.show_weight_summary)
            {
                if (flags.inventory_weights_last)
                    putstr(tmpwin, ATR_HALF_SIZE, " ");
                add_weight_summary_putstr(tmpwin, totalweight, show_weights);
            }

            unsortloot(&sortedcobj);
        }
        if (dumping)
            putstr(0, ATR_HALF_SIZE, " ");
        display_nhwindow(tmpwin, TRUE);
        destroy_nhwindow(tmpwin);
        if (all_containers)
            container_contents(magic_objs, identified, TRUE, reportempty, show_weights);
    }
    else if (reportempty)
    {
        pline("Your %s is empty.", chest_name);
        display_nhwindow(WIN_MESSAGE, FALSE);
    }
}


/* should be called with either EXIT_SUCCESS or EXIT_FAILURE */
/* called between displaying gamewindows and before newgame / restore, after getlock doclearlocks must be set to TRUE */
void
nh_bail(status, mesg, fullterminate)
int status;
const char* mesg;
boolean fullterminate;
{
    clearlocks();
    exit_nhwindows(mesg);
    if (fullterminate)
    {
        //nh_terminate resets blstats
        nh_terminate(status);
    }
    else
    {
        if (VIA_WINDOWPORT()) {
            reset_blstats();
        }
        gnollhack_exit(status);
    }
}

/* should be called with either EXIT_SUCCESS or EXIT_FAILURE */
void
nh_terminate(status)
int status;
{
    program_state.in_moveloop = 0; /* won't be returning to normal play */
    program_state.freeing_dynamic_data = 1;
    stop_animations();

#ifdef MAC
    getreturn("to exit");
#endif
    /* don't bother to try to release memory if we're in panic mode, to
       avoid trouble in case that happens to be due to memory problems */
    //if (!program_state.panicking) {
    //    freedynamicdata();
    //    dlb_cleanup();
    //}

    reset_game();

    /*
     *  This is liable to draw a warning if compiled with gcc, but it's
     *  more important to flag panic() -> really_done() -> nh_terminate()
     *  as __noreturn__ then to avoid the warning.
     */
    /* don't call exit() if already executing within an exit handler;
       that would cancel any other pending user-mode handlers */
#ifdef VMS
    if (!program_state.exiting)
#endif
    {
        program_state.exiting = 1;
        program_state.exit_status = status;

        gnollhack_exit(status);
    }
}

enum vanq_order_modes {
    VANQ_MLVL_MNDX = 0,
    VANQ_MSTR_MNDX,
    VANQ_ALPHA_SEP,
    VANQ_ALPHA_MIX,
    VANQ_MCLS_HTOL,
    VANQ_MCLS_LTOH,
    VANQ_COUNT_H_L,
    VANQ_COUNT_L_H,

    NUM_VANQ_ORDER_MODES
};


STATIC_VAR const char *vanqorders[NUM_VANQ_ORDER_MODES] = {
    "traditional: by monster level, by internal monster index",
    "by monster toughness, by internal monster index",
    "alphabetically, first unique monsters, then others",
    "alphabetically, unique monsters and others intermixed",
    "by monster class, high to low level within class",
    "by monster class, low to high level within class",
    "by count, high to low, by internal index within tied count",
    "by count, low to high, by internal index within tied count",
};
STATIC_VAR int vanq_sortmode = VANQ_MLVL_MNDX;

STATIC_PTR int CFDECLSPEC
vanqsort_cmp(vptr1, vptr2)
const genericptr vptr1;
const genericptr vptr2;
{
    int indx1 = *(short *) vptr1, indx2 = *(short *) vptr2,
        mlev1, mlev2, mstr1, mstr2, uniq1, uniq2, died1, died2, res;
    const char *name1, *name2, *punct;
    schar mcls1, mcls2;

    switch (vanq_sortmode) {
    default:
    case VANQ_MLVL_MNDX:
        /* sort by monster level */
        mlev1 = mons[indx1].mlevel, mlev2 = mons[indx2].mlevel;
        res = mlev2 - mlev1; /* mlevel high to low */
        break;
    case VANQ_MSTR_MNDX:
        /* sort by monster toughness */
        mstr1 = mons[indx1].difficulty, mstr2 = mons[indx2].difficulty;
        res = mstr2 - mstr1; /* monstr high to low */
        break;
    case VANQ_ALPHA_SEP:
        uniq1 = UniqCritterIndx(indx1);
        uniq2 = UniqCritterIndx(indx2);
        if (uniq1 ^ uniq2) { /* one or other uniq, but not both */
            res = uniq2 - uniq1;
            break;
        } /* else both unique or neither unique */
        /*FALLTHRU*/
    case VANQ_ALPHA_MIX:
        name1 = pm_common_name(&mons[indx1]), name2 = pm_common_name(&mons[indx2]);
        res = strcmpi(name1, name2); /* caseblind alhpa, low to high */
        break;
    case VANQ_MCLS_HTOL:
    case VANQ_MCLS_LTOH:
        /* mons[].mlet is a small integer, 1..N, of type plain char;
           if 'char' happens to be unsigned, (mlet1 - mlet2) would yield
           an inappropriate result when mlet2 is greater than mlet1,
           so force our copies (mcls1, mcls2) to be signed */
        mcls1 = (schar) mons[indx1].mlet, mcls2 = (schar) mons[indx2].mlet;
        /* S_ANT through S_LESSER_UNDEAD correspond to lowercase monster classes,
           S_ANGEL through S_GREATER_UNDEAD correspond to uppercase, and various
           punctuation characters are used for classes beyond those */
        if (mcls1 > S_GREATER_UNDEAD && mcls2 > S_GREATER_UNDEAD) {
            /* force a specific order to the punctuation classes that's
               different from the internal order;
               internal order is ok if neither or just one is punctuation
               since letters have lower values so come out before punct */
            static const char punctclasses[] = {
                S_LIZARD, S_EEL, S_GOLEM, S_TREANT, S_GHOST, S_DEMON, S_HUMAN, '\0'
            };

            if ((punct = index(punctclasses, mcls1)) != 0)
                mcls1 = (schar) (S_LESSER_UNDEAD + 1 + (int) (punct - punctclasses));
            if ((punct = index(punctclasses, mcls2)) != 0)
                mcls2 = (schar) (S_GREATER_UNDEAD + 1 + (int) (punct - punctclasses));
        }
        res = mcls1 - mcls2; /* class */
        if (res == 0) {
            mlev1 = mons[indx1].mlevel, mlev2 = mons[indx2].mlevel;
            res = mlev1 - mlev2; /* mlevel low to high */
            if (vanq_sortmode == VANQ_MCLS_HTOL)
                res = -res; /* mlevel high to low */
        }
        break;
    case VANQ_COUNT_H_L:
    case VANQ_COUNT_L_H:
        died1 = mvitals[indx1].died, died2 = mvitals[indx2].died;
        res = died2 - died1; /* dead count high to low */
        if (vanq_sortmode == VANQ_COUNT_L_H)
            res = -res; /* dead count low to high */
        break;
    }
    /* tiebreaker: internal mons[] index */
    if (res == 0)
        res = indx1 - indx2; /* mndx low to high */
    return res;
}

/* returns -1 if cancelled via ESC */
STATIC_OVL int
set_vanq_order()
{
    winid tmpwin;
    menu_item *selected;
    anything any;
    int i, n, choice;

    tmpwin = create_nhwindow(NHW_MENU);
    start_menu_ex(tmpwin, GHMENU_STYLE_CHOOSE_SIMPLE);
    any = zeroany; /* zero out all bits */
    for (i = 0; i < SIZE(vanqorders); i++) {
        if (i == VANQ_ALPHA_MIX || i == VANQ_MCLS_HTOL) /* skip these */
            continue;
        any.a_int = i + 1;
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, vanqorders[i],
                 (i == vanq_sortmode) ? MENU_SELECTED : MENU_UNSELECTED);
    }
    end_menu(tmpwin, "Sort order for vanquished monster counts");

    n = select_menu(tmpwin, PICK_ONE, &selected);
    destroy_nhwindow(tmpwin);
    if (n > 0) {
        choice = selected[0].item.a_int - 1;
        /* skip preselected entry if we have more than one item chosen */
        if (n > 1 && choice == vanq_sortmode)
            choice = selected[1].item.a_int - 1;
        free((genericptr_t) selected);
        vanq_sortmode = choice;
    }
    return (n < 0) ? -1 : vanq_sortmode;
}

/* #vanquished command */
int
dovanquished()
{
    list_vanquished('a', FALSE, FALSE);
    return 0;
}

/* #killed command */
int
dokilledmonsters()
{
    list_vanquished('b', FALSE, FALSE);
    return 0;
}

/* #genocided command */
int
dogenocidedmonsters()
{
    list_genocided('a', FALSE, FALSE);
    return 0;
}

STATIC_OVL void
list_vanquished(defquery, ask, isend)
char defquery;
boolean ask, isend;
{
    register int i;
    int pfx, nkilled, fkilled;
    boolean all_female, no_female;
    unsigned ntypes, ni;
    int64_t total_killed = 0L;
    winid klwin;
    short mindx[NUM_MONSTERS];
    char c, buf[BUFSZ], buftoo[BUFSZ];
    boolean dumping; /* for DUMPLOG; doesn't need to be conditional */

    dumping = (defquery == 'd');
    if (dumping)
        defquery = 'y';

    /* get totals first */
    ntypes = 0;
    for (i = LOW_PM; i < NUM_MONSTERS; i++) 
    {
        nkilled = (int)mvitals[i].died;
        if (nkilled == 0)
            continue;
        mindx[ntypes++] = i;
        total_killed += (int64_t) nkilled;
    }

    /* vanquished creatures list;
     * includes all dead monsters, not just those killed by the player
     */
    if (ntypes != 0) 
    {
        char mlet, prev_mlet = 0; /* used as small integer, not character */
        boolean class_header, uniq_header, was_uniq = FALSE;

        c = ask ? yn_function_end("Do you want an account of creatures vanquished?",
                              ynaqchars, defquery, ynaq2descs)
                : defquery;
        if (c == 'q')
            done_stopprint++;
        if (c == 'y' || c == 'a' || c == 'b') 
        {
            if (c == 'a') 
            { /* ask player to choose sort order */
                /* choose value for vanq_sortmode via menu; ESC cancels list
                   of vanquished monsters but does not set 'done_stopprint' */
                if (set_vanq_order() < 0)
                    return;
            }
            uniq_header = (vanq_sortmode == VANQ_ALPHA_SEP);
            class_header = (vanq_sortmode == VANQ_MCLS_LTOH
                            || vanq_sortmode == VANQ_MCLS_HTOL);

            klwin = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_NARROW_LIST, NO_GLYPH, zerocreatewindowinfo);
            putstr(klwin, ATR_TITLE, "Vanquished creatures:");
            if (!dumping)
                putstr(klwin, ATR_NONE, "");

            qsort((genericptr_t) mindx, ntypes, sizeof *mindx, vanqsort_cmp);
            for (ni = 0; ni < ntypes; ni++) 
            {
                i = mindx[ni];
                nkilled = mvitals[i].died;
                fkilled = mvitals[i].died_female;
                no_female = (fkilled == 0);
                all_female = (fkilled == nkilled);
                mlet = mons[i].mlet;
                if (class_header && mlet != prev_mlet) 
                {
                    Strcpy(buf, def_monsyms[(int) mlet].name);
                    putstr(klwin, ask ? ATR_NONE : iflags.menu_headings,
                           upstart(buf));
                    prev_mlet = mlet;
                }
                if (UniqCritterIndx(i)) 
                {
                    Sprintf(buf, "%s%s",
                            !is_mname_proper_name(&mons[i]) ? "the " : "",
                            pm_common_name(&mons[i]));
                    if (nkilled > 1) 
                    {
                        switch (nkilled) 
                        {
                        case 2:
                            Sprintf(eos(buf), " (twice)");
                            break;
                        case 3:
                            Sprintf(eos(buf), " (thrice)");
                            break;
                        default:
                            Sprintf(eos(buf), " (%d times)", nkilled);
                            break;
                        }
                    }
                    was_uniq = TRUE;
                } 
                else
                {
                    if (uniq_header && was_uniq)
                    {
                        putstr(klwin, ATR_NONE, "");
                        was_uniq = FALSE;
                    }
                    /* trolls or undead might have come back,
                       but we don't keep track of that */
                    if (nkilled == 1)
                        Strcpy(buf, an(all_female ? pm_female_name(&mons[i]) : no_female ? mons[i].mname : pm_common_name(&mons[i])));
                    else
                    {
                        const char* plural_name = all_female ? makeplural(pm_female_name(&mons[i])) : no_female ? makeplural(mons[i].mname) : pm_plural_name(&mons[i], 3);
                        Sprintf(buf, "%3d %s", nkilled, plural_name);
                    }
                }
                /* number of leading spaces to match 3 digit prefix */
                pfx = !strncmpi(buf, "the ", 4) ? 0
                      : !strncmpi(buf, "an ", 3) ? 1
                        : !strncmpi(buf, "a ", 2) ? 2
                          : !digit(buf[2]) ? 4 : 0;
                if (class_header)
                    ++pfx;
                Sprintf(buftoo, "%*s%s", pfx, "", buf);
                putstr(klwin, ATR_NONE, buftoo);
            }
            /*
             * if (Hallucination)
             *     putstr(klwin, ATR_NONE, "and a partridge in a pear tree");
             */
            if (ntypes > 1) 
            {
                if (!dumping)
                    putstr(klwin, ATR_NONE, "");
                Sprintf(buf, "%lld creatures vanquished.", (long long)total_killed);
                putstr(klwin, ATR_PARAGRAPH_LINE, buf);
            }
            display_nhwindow(klwin, TRUE);
            destroy_nhwindow(klwin);
        }
    } 
    else if (defquery == 'a' || defquery == 'b') 
    {
        /* #dovanquished or #killed rather than final disclosure, so pline() is ok */
        if (!isend)
            play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        const char* nomsg = "No creatures have been vanquished.";
        pline_ex1(ATR_NONE, CLR_MSG_FAIL, nomsg);
        if (!isend)
            display_popup_text(nomsg, "No Vanquished Monsters", POPUP_TEXT_NO_MONSTERS_IN_LIST, ATR_NONE, NO_COLOR, NO_GLYPH, POPUP_FLAGS_NONE);
#if defined (DUMPLOG) || defined (DUMPHTML)
    }
    else if (dumping)
    {
        putstr(0, ATR_HEADING, "No creatures were vanquished."); /* not pline() */
#endif
    }
}

void
print_selfies(enwin, final)
winid enwin;
int final;
{
    short mindx[NUM_MONSTERS] = { 0 };
    int ntypes = 0;
    int i;
    int pfx;
    for (i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        if (mvitals[i].mvflags & MV_SELFIE_TAKEN)
        {
            mindx[ntypes] = i;
            ntypes++;
        }
    }

    if (!ntypes)
    {
        putstr(enwin, 0, "  (No selfies taken with monsters)");
    }
    else
    {
        qsort((genericptr_t)mindx, ntypes, sizeof * mindx, vanqsort_cmp);

        char buf[BUFSZ], buftoo[BUFSZ];
        int ni;
        int64_t selfiescore = 0L;
        for (ni = 0; ni < ntypes; ni++)
        {
            i = mindx[ni];
            if (UniqCritterIndx(i))
            {
                Sprintf(buf, "%s%s",
                    !is_mname_proper_name(&mons[i]) ? "the " : "",
                    pm_common_name(&mons[i]));
            }
            else
            {
                Strcpy(buf, an(pm_common_name(&mons[i])));
            }
            selfiescore += TOURIST_SELFIE_PER_LEVEL_SCORE * (mons[i].difficulty + 1);

            /* number of leading spaces to match 3 digit prefix */
            pfx = !strncmpi(buf, "the ", 4) ? 0
                : !strncmpi(buf, "an ", 3) ? 1
                : !strncmpi(buf, "a ", 2) ? 2
                : !digit(buf[2]) ? 4 : 0;

            Sprintf(buftoo, " %*s%s", pfx, "", buf);
            putstr(enwin, 0, buftoo);
        }
        if (!final)
            putstr(enwin, ATR_HALF_SIZE, " ");
        if (selfiescore != context.role_score)
        {
            char dbuf[BUFSZ];
            Sprintf(dbuf, "print_selfies: selfiescore of %lld does not match context.role_score of %lld.", (long long)selfiescore, (long long)context.role_score);
            issue_debuglog(0, dbuf);
            context.role_score = selfiescore;
        }
        int64_t score_percentage = ((selfiescore + (int64_t)u.uachieve.role_achievement * TOURIST_ROLE_ACHIEVEMENT_SCORE) * 100) / MAXIMUM_ROLE_SCORE;
        score_percentage = min(100, score_percentage);
        Sprintf(buf, " You %s gained %lld%% of your maximum role score.", final ? "had" : "have", (long long)score_percentage);
        putstr(enwin, ATR_PARAGRAPH_LINE, buf);
    }
}

void
print_knight_slayings(enwin, final)
winid enwin;
int final;
{
    short mindx[NUM_MONSTERS] = { 0 };
    int ntypes = 0;
    int i;
    int pfx;
    for (i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        if (mvitals[i].died > 0 && is_knight_bounty(&mons[i]))
        {
            mindx[ntypes] = i;
            ntypes++;
        }
    }

    if (!ntypes)
    {
        putstr(enwin, 0, "  (No creatures slain)");
    }
    else
    {
        qsort((genericptr_t)mindx, ntypes, sizeof * mindx, vanqsort_cmp);

        char buf[BUFSZ], buftoo[BUFSZ];
        int ni;
        boolean no_female, all_female;
        uchar nkilled, fkilled;
        int64_t killscore = 0L;
        for (ni = 0; ni < ntypes; ni++)
        {
            i = mindx[ni];
            nkilled = mvitals[i].died;
            fkilled = mvitals[i].died_female;
            no_female = (fkilled == 0);
            all_female = (fkilled == nkilled);
            if (nkilled > 0)
            {
                if (UniqCritterIndx(i))
                {
                    killscore += KNIGHT_UNIQUE_MONSTER_PER_LEVEL_SCORE * (mons[i].difficulty + 1);
                    Sprintf(buf, "%s%s",
                        !is_mname_proper_name(&mons[i]) ? "the " : "",
                        pm_common_name(&mons[i]));
                    if (nkilled > 1)
                    {
                        switch (nkilled)
                        {
                        case 2:
                            Sprintf(eos(buf), " (twice)");
                            break;
                        case 3:
                            Sprintf(eos(buf), " (thrice)");
                            break;
                        default:
                            Sprintf(eos(buf), " (%d times)", nkilled);
                            break;
                        }
                    }
                }
                else
                {
                    killscore += KNIGHT_NORMAL_MONSTER_PER_LEVEL_SCORE * (mons[i].difficulty + 1) * nkilled;
                    /* trolls or undead might have come back,
                       but we don't keep track of that */
                    if (nkilled == 1)
                        Strcpy(buf, an(all_female ? pm_female_name(&mons[i]) : no_female ? mons[i].mname : pm_common_name(&mons[i])));
                    else
                    {
                        const char* plural_name = all_female ? makeplural(pm_female_name(&mons[i])) : no_female ? makeplural(mons[i].mname) : pm_plural_name(&mons[i], 3);
                        Sprintf(buf, "%3d %s", nkilled, plural_name);
                    }
                }
                /* number of leading spaces to match 3 digit prefix */
                pfx = !strncmpi(buf, "the ", 4) ? 0
                    : !strncmpi(buf, "an ", 3) ? 1
                    : !strncmpi(buf, "a ", 2) ? 2
                    : !digit(buf[2]) ? 4 : 0;
                Sprintf(buftoo, " %*s%s", pfx, "", buf);
                putstr(enwin, ATR_NONE, buftoo);
            }
        }
        if (!final)
            putstr(enwin, ATR_HALF_SIZE, " ");
        if (killscore != context.role_score)
        {
            char dbuf[BUFSZ];
            Sprintf(dbuf, "print_knight_slayings: killscore of %lld does not match context.role_score of %lld.", (long long)killscore, (long long)context.role_score);
            issue_debuglog(0, dbuf);
            context.role_score = killscore;
        }
        int64_t score_percentage = ((killscore + (int64_t)u.uachieve.role_achievement * KNIGHT_ROLE_ACHIEVEMENT_SCORE) * 100) / MAXIMUM_ROLE_SCORE;
        score_percentage = min(100, score_percentage);
        Sprintf(buf, " You %s gained %lld%% of your maximum role score.", final ? "had" : "have", (long long)score_percentage);
        putstr(enwin, ATR_PARAGRAPH_LINE, buf);
    }
}

void
recalculate_knight_slaying_score(VOID_ARGS)
{
    int i;
    int64_t score = 0L;
    for (i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        if (mvitals[i].died > 0 && is_knight_bounty(&mons[i]))
        {
            if (UniqCritterIndx(i))
            {
                score += KNIGHT_UNIQUE_MONSTER_PER_LEVEL_SCORE * (mons[i].difficulty + 1);
            }
            else
            {
                score += (int64_t)mvitals[i].died * KNIGHT_NORMAL_MONSTER_PER_LEVEL_SCORE * (mons[i].difficulty + 1);
            }
        }
    }
    context.role_score = score;

    struct monst* mtmp;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
    {
        if (!DEADMONSTER(mtmp) && isok(mtmp->mx, mtmp->my) && canspotmon(mtmp))
        {
            refresh_m_tile_gui_info(mtmp, FALSE);
        }
    }
}

/* number of monster species which have been genocided */
int
num_genocides()
{
    int i, n = 0;

    for (i = LOW_PM; i < NUM_MONSTERS; ++i) {
        if (mvitals[i].mvflags & MV_GENOCIDED) {
            ++n;
            if (UniqCritterIndx(i))
                impossible("unique creature '%d: %s' genocided?",
                           i, mons[i].mname);
        }
    }
    return n;
}

STATIC_OVL int
num_extinct()
{
    int i, n = 0;

    for (i = LOW_PM; i < NUM_MONSTERS; ++i) {
        if (UniqCritterIndx(i))
            continue;
        if ((mvitals[i].mvflags & MV_GONE) == MV_EXTINCT)
            ++n;
    }
    return n;
}

STATIC_OVL void
list_genocided(defquery, ask, isend)
char defquery;
boolean ask, isend;
{
    register int i;
    int ngenocided, nextinct;
    char c;
    winid klwin;
    char buf[BUFSZ];
    boolean dumping; /* for DUMPLOG; doesn't need to be conditional */

    dumping = (defquery == 'd');
    if (dumping)
        defquery = 'y';

    ngenocided = num_genocides();
    nextinct = num_extinct();

    /* genocided or extinct species list */
    if (ngenocided != 0 || nextinct != 0) 
    {
        Sprintf(buf, "Do you want a list of %sspecies%s%s?",
            (nextinct && !ngenocided) ? "extinct " : "",
            (ngenocided) ? " genocided" : "",
            (nextinct && ngenocided) ? " and extinct" : "");
        
        c = ask ? yn_function_end(buf, ynqchars, defquery, ynq2descs) : defquery;
        
        if (c == 'q')
            done_stopprint++;

        if (c == 'y' || c == 'a') 
        {
            klwin = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_NARROW_LIST, NO_GLYPH, zerocreatewindowinfo);
            Sprintf(buf, "%s%s species:",
                (ngenocided) ? "Genocided" : "Extinct",
                (nextinct && ngenocided) ? " or extinct" : "");
            putstr(klwin, ATR_TITLE, buf);
            if (!dumping)
                putstr(klwin, ATR_NONE, "");

            for (i = LOW_PM; i < NUM_MONSTERS; i++) 
            {
                /* uniques can't be genocided but can become extinct;
                   however, they're never reported as extinct, so skip them */
                if (UniqCritterIndx(i))
                    continue;

                if (mvitals[i].mvflags & MV_GONE) 
                {
                    Sprintf(buf, " %s", pm_plural_name(&mons[i], 1));
                    /*
                     * "Extinct" is unfortunate terminology.  A species
                     * is marked extinct when its birth limit is reached,
                     * but there might be members of the species still
                     * alive, contradicting the meaning of the word.
                     */
                    if ((mvitals[i].mvflags & MV_GONE) == MV_EXTINCT)
                        Strcat(buf, " (extinct)");

                    putstr(klwin, ATR_NONE, buf);
                }
            }

            if (!dumping)
                putstr(klwin, 0, "");

            if (ngenocided > 0) 
            {
                Sprintf(buf, "%d species genocided.", ngenocided);
                putstr(klwin, ATR_PARAGRAPH_LINE, buf);
            }

            if (nextinct > 0) 
            {
                Sprintf(buf, "%d species extinct.", nextinct);
                putstr(klwin, ATR_PARAGRAPH_LINE, buf);
            }

            display_nhwindow(klwin, TRUE);
            destroy_nhwindow(klwin);
        }
    }
    else if (defquery == 'a')
    {
        if (!isend)
            play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        const char* nomsg = "No species have been genocided or become extinct.";
        pline_ex1(ATR_NONE, CLR_MSG_FAIL, nomsg); /* Game is still ongoing, so pline is ok */
        if(!isend)
            display_popup_text(nomsg, "No Genocided Monsters", POPUP_TEXT_NO_MONSTERS_IN_LIST, ATR_NONE, NO_COLOR, NO_GLYPH, POPUP_FLAGS_NONE);
#if defined (DUMPLOG) || defined (DUMPHTML)
    } 
    else if (dumping) 
    {
        putstr(0, ATR_HEADING, "No species were genocided or became extinct.");
#endif
    }
}

/* set a delayed killer, ensure non-delayed killer is cleared out */
void
delayed_killer(id, format, killername, killerhintidx)
int id;
int format;
const char *killername;
int killerhintidx;
{
    struct kinfo *k = find_delayed_killer(id);

    if (!k) {
        /* no match, add a new delayed killer to the list */
        k = (struct kinfo *) alloc(sizeof (struct kinfo));
        (void) memset((genericptr_t) k, 0, sizeof (struct kinfo));
        k->id = id;
        k->next = killer.next;
        killer.next = k;
    }

    k->hint_idx = killerhintidx;
    k->format = format;
    Strcpy(k->name, killername ? killername : "");
    killer.name[0] = 0;
}

struct kinfo *
find_delayed_killer(id)
int id;
{
    struct kinfo *k;

    for (k = killer.next; k != (struct kinfo *) 0; k = k->next) {
        if (k->id == id)
            break;
    }
    return k;
}

void
dealloc_killer(kptr)
struct kinfo *kptr;
{
    struct kinfo *prev = &killer, *k;

    if (kptr == (struct kinfo *) 0)
        return;
    for (k = killer.next; k != (struct kinfo *) 0; k = k->next) {
        if (k == kptr)
            break;
        prev = k;
    }

    if (k == (struct kinfo *) 0) {
        impossible("dealloc_killer (#%d) not on list", kptr->id);
    } else {
        prev->next = k->next;
        free((genericptr_t) k);
        debugpline1("freed delayed killer #%d", kptr->id);
    }
}

void
save_killers(fd, mode)
int fd;
int mode;
{
    struct kinfo *kptr;

    if (perform_bwrite(mode)) {
        for (kptr = &killer; kptr != (struct kinfo *) 0; kptr = kptr->next) {
            bwrite(fd, (genericptr_t) kptr, sizeof (struct kinfo));
        }
    }
    if (release_data(mode)) {
        while (killer.next) {
            kptr = killer.next->next;
            free((genericptr_t) killer.next);
            killer.next = kptr;
        }
    }
}

void
reset_killers(VOID_ARGS)
{
    struct kinfo* kptr;
    while (killer.next) {
        kptr = killer.next->next;
        free((genericptr_t)killer.next);
        killer.next = kptr;
    }
    memset((genericptr_t)&killer, 0, sizeof(killer));
}

void
restore_killers(fd)
int fd;
{
    struct kinfo *kptr;
    Strcpy(debug_buf_4, "restore_killers");

    for (kptr = &killer; kptr != (struct kinfo *) 0; kptr = kptr->next) {
        mread(fd, (genericptr_t) kptr, sizeof (struct kinfo));
        if (kptr->next) {
            kptr->next = (struct kinfo *) alloc(sizeof (struct kinfo));
        }
    }
}

STATIC_OVL int
wordcount(p)
char *p;
{
    int words = 0;

    while (*p) {
        while (*p && isspace((uchar) *p))
            p++;
        if (*p)
            words++;
        while (*p && !isspace((uchar) *p))
            p++;
    }
    return words;
}

STATIC_OVL void
bel_copy1(inp, out)
char **inp, *out;
{
    char *in = *inp;

    out += strlen(out); /* eos() */
    while (*in && isspace((uchar) *in))
        in++;
    while (*in && !isspace((uchar) *in))
        *out++ = *in++;
    *out = '\0';
    *inp = in;
}

char *
build_english_list(in)
char *in;
{
    char *out, *p = in;
    size_t len = strlen(p), words = (size_t)wordcount(p);

    /* +3: " or " - " "; +(words - 1): (N-1)*(", " - " ") */
    if (words > 1)
        len += 3 + (words - 1);
    out = (char *) alloc(len + 1);
    *out = '\0'; /* bel_copy1() appends */

    switch (words) {
    case 0:
        impossible("no words in list");
        break;
    case 1:
        /* "single" */
        bel_copy1(&p, out);
        break;
    default:
        if (words == 2) {
            /* "first or second" */
            bel_copy1(&p, out);
            Strcat(out, " ");
        } else {
            /* "first, second, or third */
            do {
                bel_copy1(&p, out);
                Strcat(out, ", ");
            } while (--words > 1);
        }
        Strcat(out, "or ");
        bel_copy1(&p, out);
        break;
    }
    return out;
}

int64_t
get_conduct_score_upon_ascension(VOID_ARGS)
{
    return (int64_t)(
        50 * (u.uconduct.food == 0)
        + 15 * (u.uconduct.gnostic == 0)
        + 60 * (u.uconduct.killer == 0)
        + 30 * (u.uconduct.literate == 0)
        + 2 * (u.uconduct.polypiles == 0)
        + 2 * (u.uconduct.polyselfs == 0)
        + 10 * (u.uconduct.unvegan == 0)
        + 10 * (u.uconduct.unvegetarian == 0)
        + 5 * (u.uconduct.weaphit == 0)
        + 5 * (u.uconduct.wisharti == 0)
        + 15 * (u.uconduct.wishes == 0)
        + 15 * (u.uconduct.genocides == 0)
        + 10 * (u.uconduct.elbereths == 0)
        + 15 * (u.uconduct.conflicts == 0)
        + 2 * (u.uconduct.pets == 0)
        + 10 * (u.umortality == 0)
        + 80 * (u.uroleplay.blind)
        + 60 * (u.uroleplay.nudist)
        + 2 * (u.uroleplay.numbones == 0)
        - 20 * (u.ualign.type != u.ualignbase[A_ORIGINAL])
        - 20 * (u.ualignbase[A_CURRENT] != u.ualignbase[A_ORIGINAL])
        );
}

int64_t
get_current_game_score(VOID_ARGS)
{
#if 0
    /* Old NetHack score */
    int64_t deepest = deepest_lev_reached(FALSE);
    int64_t utotal;

    utotal = money_cnt(invent) + hidden_gold();
    if ((utotal -= u.umoney0) < 0L)
        utotal = 0L;
    utotal += u.u_gamescore + (50 * (deepest - 1))
        + (deepest > 30 ? 10000 : deepest > 20 ? 1000 * (deepest - 20) : 0);
    if (utotal < u.u_gamescore)
        utotal = LONG_MAX; /* wrap around */
    return utotal;
#endif
    if (discover || CasualMode || flags.non_scoring)
        return 0L;

    int64_t utotal = 0;
    int64_t Deepest_Dungeon_Level = deepest_lev_reached(FALSE);
    int64_t Achievements_Score = (int64_t)(u.uachieve.amulet + u.uachieve.ascended + u.uachieve.bell + u.uachieve.book + u.uachieve.enter_gehennom + u.uachieve.finish_sokoban +
        u.uachieve.killed_medusa + u.uachieve.killed_yacc + u.uachieve.killed_demogorgon + u.uachieve.menorah + u.uachieve.prime_codex + u.uachieve.mines_luckstone +
        + u.uachieve.entered_astral_plane + u.uachieve.entered_elemental_planes + u.uevent.invoked + u.uachieve.crowned + u.uachieve.learned_castle_tune
        );

    int64_t Small_Achievements_Score = (int64_t)(u.uachieve.consulted_oracle + u.uachieve.read_discworld_novel
        + u.uachieve.entered_gnomish_mines + u.uachieve.entered_mine_town 
        + u.uachieve.entered_shop + u.uachieve.entered_temple
        + u.uachieve.entered_sokoban + u.uachieve.entered_bigroom
        + u.uachieve.entered_large_circular_dungeon + u.uachieve.entered_plane_of_modron 
        + u.uachieve.entered_hellish_pastures
        );

    int64_t Conduct_Score = (int64_t)(u.uachieve.ascended) * get_conduct_score_upon_ascension();

    int64_t Role_Specific_Score = 0L;
    int64_t Role_Achievement_Score = 0L;  /* Special role-specific achievement */

    switch (Role_switch)
    {
    case PM_ARCHAEOLOGIST:
    {
        struct item_score_count_result cnt = count_artifacts(invent);
        struct item_score_count_result cnt2 = count_artifacts(magic_objs);
        struct item_score_count_result cnt3 = count_historic_statues(invent);
        struct item_score_count_result cnt4 = count_historic_statues(magic_objs);
        struct item_score_count_result cnt5 = count_valuable_art_objects(invent);
        struct item_score_count_result cnt6 = count_valuable_art_objects(magic_objs);
        Role_Specific_Score = cnt.score + cnt2.score + cnt3.score + cnt4.score + (cnt5.score + cnt6.score) * ARCHAEOLOGIST_ART_OBJECT_SCORE_MULTIPLIER;
        Role_Achievement_Score = ARCHAEOLOGIST_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_BARBARIAN:
    {
        struct item_score_count_result cnt = count_powerful_melee_weapon_score(invent);
        struct item_score_count_result cnt2 = count_powerful_melee_weapon_score(magic_objs);
        Role_Specific_Score = cnt.score + cnt2.score;
        Role_Achievement_Score = BARBARIAN_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_CAVEMAN:
    {
        struct amulet_count_result cnt = count_amulets(invent);
        struct amulet_count_result cnt2 = count_amulets(magic_objs);
        Role_Specific_Score = CAVEMAN_PER_AMULET_OF_LIFE_SAVING_SCORE * cnt.amulets_of_life_saving + CAVEMAN_PER_OTHER_AMULET_SCORE * cnt.other_amulets;
        Role_Specific_Score += CAVEMAN_PER_AMULET_OF_LIFE_SAVING_SCORE * cnt2.amulets_of_life_saving + CAVEMAN_PER_OTHER_AMULET_SCORE * cnt2.other_amulets;
        Role_Achievement_Score = CAVEMAN_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_HEALER:
    {
        int i;
        for (i = 0; i < MAXSPELL; i++)
        {
            if (spl_book[i].sp_id == NO_SPELL)
                break;
            if (!P_RESTRICTED(objects[spl_book[i].sp_id].oc_skill) && !objects[spl_book[i].sp_id].oc_pre_discovered)
                Role_Specific_Score += HEALER_PER_SPELL_LEVEL_SCORE * (int64_t)(spl_book[i].sp_lev + 2); /* Healer has the fewest spells */
        }
        Role_Achievement_Score = HEALER_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_KNIGHT:
    {
        //int i;
        //for (i = LOW_PM; i < NUM_MONSTERS; i++)
        //{
        //    if (mvitals[i].died > 0 && ((u.ualign.type == A_LAWFUL ? is_demon(&mons[i]) || mons[i].mlet == S_IMP : u.ualign.type == A_CHAOTIC ? is_angel(&mons[i]) : FALSE) || (is_dragon(&mons[i]) && u.ualign.type * mons[i].maligntyp < 0)))
        //    {
        //        if (UniqCritterIndx(i))
        //        {
        //            Role_Specific_Score += KNIGHT_UNIQUE_MONSTER_PER_LEVEL_SCORE * (mons[i].difficulty + 1);
        //        }
        //        else
        //        {
        //            Role_Specific_Score += (int64_t)mvitals[i].died * KNIGHT_NORMAL_MONSTER_PER_LEVEL_SCORE * (mons[i].difficulty + 1);
        //        }
        //    }
        //}
        Role_Specific_Score = context.role_score;
        Role_Achievement_Score = KNIGHT_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_MONK:
    {
        Role_Specific_Score = Conduct_Score * MONK_EXTRA_CONDUCT_SCORE_MULTIPLIER;
        Role_Achievement_Score = MONK_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_PRIEST:
    {
        int i;
        for (i = 0; i < MAXSPELL; i++)
        {
            if (spl_book[i].sp_id == NO_SPELL)
                break;
            if (!P_RESTRICTED(objects[spl_book[i].sp_id].oc_skill) && !objects[spl_book[i].sp_id].oc_pre_discovered)
                Role_Specific_Score += PRIEST_PER_SPELL_LEVEL_SCORE * (int64_t)(spl_book[i].sp_lev + 2); /* Priest has the fewer spell than wizard */
        }
        Role_Achievement_Score = PRIEST_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_RANGER:
    {
        struct item_score_count_result cnt = count_powerful_ranged_weapon_score(invent);
        struct item_score_count_result cnt2 = count_powerful_ranged_weapon_score(magic_objs);
        Role_Specific_Score = cnt.score + cnt2.score;
        Role_Achievement_Score = RANGER_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_ROGUE:
    {
        int64_t lootvalue = 0L;
        lootvalue += money_cnt(invent);
        lootvalue += hidden_gold() + magic_gold(); /* accumulate gold from containers */
        lootvalue += carried_gem_value() + magic_gem_value();
        struct item_score_count_result cnt = count_valuable_art_objects(invent);
        struct item_score_count_result cnt2 = count_valuable_art_objects(magic_objs);
        lootvalue += cnt.score + cnt2.score;
        Role_Specific_Score = lootvalue;
        Role_Achievement_Score = ROGUE_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_SAMURAI:
    {
        struct item_score_count_result cnt = count_powerful_Japanese_item_score(invent);
        struct item_score_count_result cnt2 = count_powerful_Japanese_item_score(magic_objs);
        Role_Specific_Score = cnt.score + cnt2.score;
        Role_Achievement_Score = SAMURAI_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_TOURIST:
    {
        //int i;
        //for (i = LOW_PM; i < NUM_MONSTERS; i++)
        //{
        //    if (mvitals[i].mvflags & MV_SELFIE_TAKEN)
        //    {
        //        Role_Specific_Score += TOURIST_SELFIE_PER_LEVEL_SCORE * (mons[i].difficulty + 1);
        //    }
        //}
        Role_Specific_Score = context.role_score;
        Role_Achievement_Score = TOURIST_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_VALKYRIE:
    {
        struct item_score_count_result cnt = count_powerful_valkyrie_item_score(invent);
        struct item_score_count_result cnt2 = count_powerful_valkyrie_item_score(magic_objs);
        Role_Specific_Score = cnt.score + cnt2.score;
        Role_Achievement_Score = VALKYRIE_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    case PM_WIZARD:
    {
        int i;
        for (i = 0; i < MAXSPELL; i++)
        {
            if (spl_book[i].sp_id == NO_SPELL)
                break;
            if (!P_RESTRICTED(objects[spl_book[i].sp_id].oc_skill) && !objects[spl_book[i].sp_id].oc_pre_discovered)
                Role_Specific_Score += WIZARD_PER_SPELL_LEVEL_SCORE * (int64_t)(spl_book[i].sp_lev + 2);
        }
        Role_Achievement_Score = WIZARD_ROLE_ACHIEVEMENT_SCORE * (int64_t)u.uachieve.role_achievement;
        break;
    }
    default:
        break;
    }

    int64_t Total_Role_Score = Role_Specific_Score + Role_Achievement_Score;
    int64_t Base_Score = (int64_t)(Deepest_Dungeon_Level - 1) * SCORE_PER_DUNGEON_LEVEL + Small_Achievements_Score * SCORE_PER_MINOR_ACHIEVEMENT + Achievements_Score * SCORE_PER_MAJOR_ACHIEVEMENT 
        + Conduct_Score * CONDUCT_SCORE_MULTIPLIER + min(Total_Role_Score, MAXIMUM_ROLE_SCORE);
    
    Base_Score = max(0L, Base_Score);

    double Turn_Count_Multiplier = sqrt(50000.0) / sqrt((double)max(1L, moves));
    double Ascension_Multiplier = u.uachieve.ascended ? min(16.0, max(2.0, 4.0 * Turn_Count_Multiplier)) : 1.0;
    double Difficulty_Multiplier = pow(10.0, 0.5 * (double)context.game_difficulty);
    double mortexp = (double)(u.utruemortality > 2 ? 3 : u.utruemortality + 1);
    double mortmult = (double)(u.utruemortality > 2 ? u.utruemortality - 1 : 1);
    double Modern_Multiplier = ModernMode ? 1.0 / (pow(3, mortexp) * mortmult) : 1.0;

    utotal = (int64_t)(round((double)Base_Score * Ascension_Multiplier * Difficulty_Multiplier * Modern_Multiplier));
    return utotal;
}



STATIC_OVL void
reset_objchn(otmp)
struct obj* otmp;
{
    struct obj* otmp2;
    while (otmp) {
        otmp2 = otmp->nobj;
        if (Has_contents(otmp))
            reset_objchn(otmp->cobj);
        otmp->where = OBJ_FREE; /* set to free so dealloc will work */
        otmp->nobj = NULL;      /* nobj saved into otmp2 */
        otmp->cobj = NULL;      /* contents handled above */
        otmp->timed = 0;        /* not timed any more */
        otmp->lamplit = 0;      /* caller handled lights */
        otmp->makingsound = 0;  /* caller handled sounds */
        dealloc_obj(otmp);
        otmp = otmp2;
    }
}

STATIC_OVL void
reset_monchn(mtmp)
struct monst* mtmp;
{
    struct monst* mtmp2;
    while (mtmp)
    {
        mtmp2 = mtmp->nmon;
        if (mtmp->minvent)
            reset_objchn(mtmp->minvent);
        mtmp->nmon = NULL;  /* nmon saved into mtmp2 */
        dealloc_monst(mtmp);
        mtmp = mtmp2;
    }
}

void
reset_cemetery(cemeteryaddr)
struct cemetery** cemeteryaddr;
{
    struct cemetery* thisbones, * nextbones;

    nextbones = *cemeteryaddr;
    while ((thisbones = nextbones) != 0) {
        nextbones = thisbones->next;
        free((genericptr_t)thisbones);
    }
    *cemeteryaddr = 0;
}

/* save traps; ftrap is the only trap chain so the 2nd arg is superfluous */
STATIC_OVL void
reset_trapchn(trap)
struct trap* trap;
{
    struct trap* trap2;
    while (trap) {
        trap2 = trap->ntrap;
        dealloc_trap(trap);
        trap = trap2;
    }
}

STATIC_OVL void
reset_damage(VOID_ARGS)
{
    register struct damage* damageptr, * tmp_dam;
    unsigned int xl = 0;

    damageptr = level.damagelist;
    for (tmp_dam = damageptr; tmp_dam; tmp_dam = tmp_dam->next)
        xl++;

    while (xl--) {
        tmp_dam = damageptr;
        damageptr = damageptr->next;
        free((genericptr_t)tmp_dam);
    }
    level.damagelist = 0;
}


STATIC_OVL void
reset_lev(VOID_ARGS)
{
    reset_cemetery(&level.bonesinfo);
    memset((genericptr_t)levl, 0, sizeof levl);
    memset((genericptr_t)lastseentyp, 0, sizeof(lastseentyp));
    memset((genericptr_t)&monstermoves, 0, sizeof(monstermoves));
    memset((genericptr_t)&upstair, 0, sizeof(stairway));
    memset((genericptr_t)&dnstair, 0, sizeof(stairway));
    memset((genericptr_t)&upladder, 0, sizeof(stairway));
    memset((genericptr_t)&dnladder, 0, sizeof(stairway));
    memset((genericptr_t)&sstairs, 0, sizeof(stairway));
    memset((genericptr_t)&updest, 0, sizeof(dest_area));
    memset((genericptr_t)&dndest, 0, sizeof(dest_area));
    memset((genericptr_t)&noteledest, 0, sizeof(dest_area));
    memset((genericptr_t)&level.flags, 0, sizeof(level.flags));
    memset((genericptr_t)doors, 0, sizeof(doors));

    reset_rooms();
    doorindex = 0;

    //Already reset before
    //reset_timers();
    //reset_light_sources();
    //reset_sound_sources();

    reset_monchn(fmon);
    reset_worm();
    reset_trapchn(ftrap);
    reset_objchn(fobj);
    reset_objchn(level.buriedobjlist);
    reset_objchn(billobjs);
    reset_objchn(memoryobjs);

    int x, y;
    for (y = 0; y < ROWNO; y++)
        for (x = 0; x < COLNO; x++)
            level.monsters[x][y] = 0;
    fmon = 0;
    ftrap = 0;
    fobj = 0;
    level.buriedobjlist = 0;
    billobjs = 0;
    memoryobjs = 0;
    lastmemoryobj = 0;

    /* level.bonesinfo = 0; -- handled by savecemetery() */
    reset_engravings();
    reset_damage();
    clear_regions();
}

STATIC_OVL void
reset_levchn(VOID_ARGS)
{
    s_level* tmplev, * tmplev2;
    //int cnt = 0;

    //for (tmplev = sp_levchn; tmplev; tmplev = tmplev->next)
    //    cnt++;

    for (tmplev = sp_levchn; tmplev; tmplev = tmplev2) {
        tmplev2 = tmplev->next;
        free((genericptr_t)tmplev);
    }
    sp_levchn = 0;
}

STATIC_OVL void
reset_msghistory(VOID_ARGS)
{
    issue_simple_gui_command(GUI_CMD_CLEAR_MESSAGE_HISTORY);

    /* Let's clean something else, too, here just in case */
    issue_simple_gui_command(GUI_CMD_CLEAR_CONDITION_TEXTS);
    issue_simple_gui_command(GUI_CMD_CLEAR_FLOATING_TEXTS);
    issue_simple_gui_command(GUI_CMD_CLEAR_GUI_EFFECTS);
}


STATIC_OVL void
reset_gamestate(VOID_ARGS)
{
    memset((genericptr_t)&context, 0, sizeof(struct context_info));
    memset((genericptr_t)&urealtime.realtime, 0, sizeof urealtime.realtime);
    memset((genericptr_t)&flags, 0, sizeof(struct flag));
#ifdef SYSFLAGS
    memset((genericptr_t)&sysflags, 0, sizeof(struct sysflag));
#endif
    memset((genericptr_t)&spl_orderindx, 0, sizeof(spl_orderindx));
    memset((genericptr_t)&u, 0, sizeof(struct you));
    ubirthday = 0;
    reset_killers();

    /* must come before migrating_objs and migrating_mons are freed */
    reset_timers();
    reset_light_sources();
    reset_sound_sources();

    reset_objchn(invent);
    if (BALL_IN_MON) {
        /* prevent loss of ball & chain when swallowed */
        uball->nobj = uchain;
        uchain->nobj = (struct obj*)0;
        reset_objchn(uball);
    }

    reset_objchn(magic_objs);
    reset_objchn(migrating_objs);
    reset_monchn(migrating_mons);
    reset_monchn(mydogs);

    invent = 0;
    magic_objs = 0;
    migrating_objs = 0;
    migrating_mons = 0;
    mydogs = 0;
    uball = 0;
    uchain = 0;

    memset((genericptr_t)mvitals, 0, sizeof(mvitals));

    reset_dungeon();
    reset_levchn();
    memset((genericptr_t)&moves, 0, sizeof moves);
    memset((genericptr_t)&monstermoves, 0, sizeof monstermoves);
    memset((genericptr_t)&quest_status, 0, sizeof(struct q_score));
    memset((genericptr_t)spl_book, 0, sizeof(spl_book));
    reset_artifacts();
    reset_oracles();
    *pl_character = 0;
    *pl_fruit = 0;
    reset_fruitchn();
    reset_names();
    reset_waterlevel();
    reset_msghistory();
    reset_gamelog();
}

STATIC_DCL void
reset_remaining_static_variables(VOID_ARGS)
{
#ifdef PANICTRACE
    aborting = FALSE;
#endif
    reset_allmain();
    reset_hunger_status();
    reset_drawbridge();
    reset_dig();
    reset_display();
    reset_dogs();
    reset_dogmove();
    reset_doname();
    reset_dowear();
    reset_files();
    reset_hack();
    reset_inventory();
    reset_kick();
    reset_mhitm();
    reset_mhitu();
    reset_mon();
    reset_mthrowu();
    reset_pickup();
    reset_pline();
    reset_polyself();
    reset_potion();
    reset_read();
    reset_throw();
    reset_traps();
    reset_shk();
    reset_spells();
    reset_splev();
    reset_teleport();
    reest_track();
    reset_vision();
    reset_windows();
    reset_urolerace();
    reset_zap();
}

STATIC_DCL void
reset_remaining_dynamic_data(VOID_ARGS)
{
    free_dynamic_data_A();
    free_dynamic_data_B();
    free_dynamic_data_C();
}

void
reset_game(VOID_ARGS)
{
    dmonsfree();
    *plname = 0;
    *recovery_plname = 0;
    is_gui_in_debug_mode = FALSE;
    plname_from_error_savefile = FALSE;
    plname_from_imported_savefile = FALSE;
    reset_gamestate_ex();
    n_game_recoveries = 0;
    status_finish();
    reset_blstats();
    reset_occupations();
    reset_pick();
    reset_remaining_static_variables();
    reset_remaining_dynamic_data();
    dlb_cleanup();
}

void
reset_gamestate_ex(VOID_ARGS)
{
    reset_lev();
    reset_rooms(); /* no dynamic memory to reclaim */
    reset_gamestate();
    reset_makemon();
}

void
tally_realtime(VOID_ARGS)
{
    if (!context.game_started)
    {
        return;
    }
    urealtime.finish_time = getnow();
    urealtime.realtime += (urealtime.finish_time - urealtime.start_timing);
    issue_simple_gui_command(GUI_CMD_REPORT_PLAY_TIME);
    urealtime.start_timing = urealtime.finish_time;
}

/* yes/no question via GUI when the game windows may already have been closed */
STATIC_OVL char
special_yn_query(title, query)
const char* title;
const char* query;
{
    struct special_view_info info = { 0 };
    info.viewtype = SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_N;
    info.title = title;
    info.text = query;
    info.attr = ATR_NONE;
    info.color = NO_COLOR;
    int res = open_special_view(info);
    return (char)res;
}

/*end.c*/
