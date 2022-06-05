/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    were.c    $NHDT-Date: 1550524568 2019/02/18 21:16:08 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.23 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

void
were_change(mon)
register struct monst *mon;
{
    if (!mon)
        return;

    if (!is_were(mon->data))
        return;

    if (is_human(mon->data)) {
        if (!Protection_from_shape_changers
            && !rn2(night() ? (flags.moonphase == FULL_MOON ? 3 : 30)
                            : (flags.moonphase == FULL_MOON ? 10 : 50))) {
            new_were(mon); /* change into animal form */
            if (!Deaf && !canseemon(mon)) {
                const char *howler;

                switch (mon->mnum) {
                case PM_WEREWOLF:
                    howler = "wolf";
                    break;
                case PM_WEREBEAR:
                    howler = "bear";
                    break;
                case PM_WEREJACKAL:
                    howler = "jackal";
                    break;
                default:
                    howler = (char *) 0;
                    break;
                }
                if (howler)
                    You_hear("a %s howling at the moon.", howler);
            }
        }
    } else if (!rn2(30) || Protection_from_shape_changers) {
        new_were(mon); /* change back into human form */
    }

    if (mon == &youmonst)
    {
        /* update innate intrinsics (mainly Drain_resistance) */
        set_uasmon(); /* new_were() doesn't do this */
    }
}

int
counter_were(pm)
int pm;
{
    switch (pm) {
    case PM_WEREWOLF:
        return PM_HUMAN_WEREWOLF;
    case PM_HUMAN_WEREWOLF:
        return PM_WEREWOLF;
    case PM_WEREBEAR:
        return PM_HUMAN_WEREBEAR;
    case PM_HUMAN_WEREBEAR:
        return PM_WEREBEAR;
    case PM_WEREJACKAL:
        return PM_HUMAN_WEREJACKAL;
    case PM_HUMAN_WEREJACKAL:
        return PM_WEREJACKAL;
    case PM_WERERAT:
        return PM_HUMAN_WERERAT;
    case PM_HUMAN_WERERAT:
        return PM_WERERAT;
    default:
        return NON_PM;
    }
}

/* convert monsters similar to werecritters into appropriate werebeast */
int
were_beastie(pm)
int pm;
{
    switch (pm) {
    case PM_WERERAT:
    case PM_SEWER_RAT:
    case PM_GIANT_RAT:
    case PM_RABID_RAT:
        return PM_WERERAT;
    case PM_WEREJACKAL:
    case PM_JACKAL:
    case PM_FOX:
    case PM_COYOTE:
        return PM_WEREJACKAL;
    case PM_WEREWOLF:
    case PM_WOLF:
    case PM_WARG:
    case PM_WINTER_WOLF:
        return PM_WEREWOLF;
    case PM_WEREBEAR:
    case PM_GRIZZLY_BEAR:
        return PM_WEREBEAR;
    default:
        break;
    }
    return NON_PM;
}

void
new_were(mon)
register struct monst *mon;
{
    if (!mon)
        return;

    register int pm;

    pm = counter_were(mon->mnum);
    if (pm < LOW_PM) {
        impossible("unknown lycanthrope %s.", mon->data->mname);
        return;
    }

    if (canseemon(mon) && !Hallucination)
    {
        play_sfx_sound_at_location(SFX_POLYMORPH_SUCCESS, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s changes into a %s.", Monnam(mon),
            is_human(&mons[pm]) ? "human" : pm_monster_name(&mons[pm], mon->female) + 4);
    }

    set_mon_data(mon, &mons[pm]);
    if (!mon_can_move(mon)) 
    {
        /* transformation wakens and/or revitalizes */
        mon->msleeping = 0;
        mon->mfrozen = 0; /* not asleep or paralyzed */
        mon->mcanmove = 1;
        mon->mprops[SLEEPING] = 0;
        mon->mprops[PARALYZED] = 0;
    }
    /* regenerate by 1/4 of the lost hit points */
    mon->mhp += (mon->mhpmax - mon->mhp) / 4;
    newsym(mon->mx, mon->my);
    mon_break_armor(mon, FALSE);
    possibly_unwield(mon, FALSE);
}

/* a lycanthrope (even you) summons a horde */
int
were_summon(ptr, yours, visible, genbuf)
struct permonst *ptr;
boolean yours;
int *visible; /* number of visible helpers created */
char *genbuf;
{
    int i, typ, pm = monsndx(ptr);
    struct monst *mtmp;
    int total = 0;

    *visible = 0;
    if (Protection_from_shape_changers && !yours)
        return 0;
    context.makemon_spef_idx = 0;
    for (i = rnd(5); i > 0; i--) {
        switch (pm) {
        case PM_WERERAT:
        case PM_HUMAN_WERERAT:
            typ = rn2(3) ? PM_SEWER_RAT
                         : rn2(3) ? PM_GIANT_RAT : PM_RABID_RAT;
            if (genbuf)
                Strcpy(genbuf, "rat");
            break;
        case PM_WEREJACKAL:
        case PM_HUMAN_WEREJACKAL:
            typ = rn2(7) ? PM_JACKAL : PM_COYOTE;
            if (genbuf)
                Strcpy(genbuf, "jackal");
            break;
        case PM_WEREWOLF:
        case PM_HUMAN_WEREWOLF:
            typ = rn2(5) ? PM_WOLF : rn2(2) ? PM_WARG : PM_WINTER_WOLF;
            if (genbuf)
                Strcpy(genbuf, "wolf");
            break;
        case PM_WEREBEAR:
        case PM_HUMAN_WEREBEAR:
            typ = PM_GRIZZLY_BEAR;
            if (genbuf)
                Strcpy(genbuf, "bear");
            break;
        default:
            continue;
        }
        mtmp = makemon(&mons[typ], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL));
        if (mtmp)
        {
            context.makemon_spef_idx++;
            total++;
            mtmp->mprops[SUMMON_FORBIDDEN] = 30;
            if (canseemon(mtmp))
                *visible += 1;
        }
        if (yours && mtmp)
            (void) tamedog(mtmp, (struct obj *) 0, TAMEDOG_FORCE_NON_UNIQUE, FALSE, 0, FALSE, FALSE);
    }
    makemon_animation_wait_until_end();
    return total;
}

void
you_were()
{
    char qbuf[QBUFSZ];
    boolean controllable_poly = Polymorph_control && !(Stunned || Unaware);

    if (Unchanging || u.umonnum == u.ulycn)
        return;
    if (controllable_poly) {
        /* `+4' => skip "were" prefix to get name of beast */
        Sprintf(qbuf, "Do you want to change into %s?",
                an(pm_monster_name(&mons[u.ulycn], flags.female) + 4));
        if (!paranoid_query(ParanoidWerechange, qbuf))
            return;
    }
    (void) polymon(u.ulycn);
}

void
you_unwere(purify)
boolean purify;
{
    boolean controllable_poly = Polymorph_control && !(Stunned || Unaware);

    if (purify) {
        play_sfx_sound(SFX_CURE_DISEASE);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "purified.");
        set_ulycn(NON_PM); /* cure lycanthropy */
    }
    if (!Unchanging && is_were(youmonst.data)
        && (!controllable_poly
            || !paranoid_query(ParanoidWerechange, "Remain in beast form?")))
        rehumanize();
    else if (is_were(youmonst.data) && !u.mtimedone)
        u.mtimedone = rn1(200, 200); /* 40% of initial were change */

    refresh_u_tile_gui_info(TRUE);
    context.botl = context.botlx = 1;
}

/* lycanthropy is being caught or cured, but no shape change is involved */
void
set_ulycn(which)
int which;
{
    u.ulycn = which;
    /* add or remove lycanthrope's innate intrinsics (Drain_resistance) */
    set_uasmon();

    context.botl = context.botlx = 1;
}

/*were.c*/
