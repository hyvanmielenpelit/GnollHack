/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    wizard.c    $NHDT-Date: 1539804905 2018/10/17 19:35:05 $  $NHDT-Branch: keni-makedefsm $:$NHDT-Revision: 1.53 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2016. */
/* GnollHack may be freely redistributed.  See license for details. */

/* wizard code - inspired by rogue code from Merlyn Leroy (digi-g!brian) */
/*             - heavily modified to give the wiz balls.  (genat!mike)   */
/*             - dewimped and given some maledictions. -3. */
/*             - generalized for 3.1 (mike@bullns.on01.bull.ca) */

#include "hack.h"
#include "qtext.h"

STATIC_DCL short FDECL(which_arti, (unsigned long));
STATIC_DCL boolean FDECL(mon_has_arti, (struct monst *, SHORT_P));
STATIC_DCL struct monst *FDECL(other_mon_has_arti, (struct monst *, SHORT_P));
STATIC_DCL struct obj *FDECL(on_ground, (SHORT_P));
STATIC_DCL boolean FDECL(you_have_item, (unsigned long));
STATIC_DCL unsigned long FDECL(target_on, (unsigned long, struct monst *));
STATIC_DCL unsigned long FDECL(strategy, (struct monst *));

/* adding more neutral creatures will tend to reduce the number of monsters
   summoned by summon_nasties(); adding more lawful creatures will reduce the number
   of monsters summoned by lawfuls; adding more chaotic creatures will reduce
   the number of monsters summoned by chaotics; prior to 3.6.1, there were
   only four lawful candidates, so lawful summoners tended to summon more
   (trying to get lawful or neutral but obtaining chaotic instead) than
   their chaotic counterparts */
STATIC_VAR NEARDATA const int nasties[] = {
    /* neutral */
    PM_GARGANTUAN_COCKATRICE, PM_GIANT_COCKATRICE, PM_MINOTAUR,
    PM_OWLBEAR_PATRIARCH, PM_OWLBEAR_MATRIARCH, PM_PURPLE_WORM, 
    PM_LERNAEAN_HYDRA, PM_ROC, PM_NEO_OTYUGH, PM_MASTODON, PM_GORGON,
    PM_MUCILAGINOUS_CUBE, PM_GIANT_ANACONDA, PM_JABBERWOCK, PM_ELDER_TREANT,
    PM_SILVER_GOLEM, PM_SKELETON_KING, PM_GREEN_SLIME,
    /* chaotic */
    PM_ANCIENT_BLACK_DRAGON, PM_ANCIENT_RED_DRAGON,
    PM_BLACK_DRAGON, PM_RED_DRAGON, PM_VAMPIRE_LORD, PM_VAMPIRE_MAGE, PM_KING_WRAITH,
    PM_ELDER_TENTACLED_ONE, PM_DISENCHANTER, PM_WINGED_GARGOYLE, PM_FLIND_LORD,
    PM_STORM_GIANT, PM_WAR_TROLL, PM_ELDER_DRACOLICH, PM_DRACOLICH,
    PM_OGRE_KING, PM_DEATH_FLAYER, PM_GAZER, PM_ELDER_GAZER, PM_DEATH_GAZER,
    /* lawful */
    PM_ANCIENT_SILVER_DRAGON, PM_ANCIENT_ORANGE_DRAGON, PM_ANCIENT_GREEN_DRAGON,
    PM_ANCIENT_YELLOW_DRAGON, PM_SILVER_DRAGON, PM_ORANGE_DRAGON, PM_GREEN_DRAGON,
    PM_YELLOW_DRAGON, PM_GUARDIAN_NAGA, PM_FIRE_GIANT, PM_PHOENIX,
    PM_KI_RIN, PM_COUATL, PM_HORNED_DEVIL, PM_BARBED_DEVIL,
    /* (titans, ki-rin, and golden nagas are suitably nasty, but
       they're summoners so would aggravate excessive summoning) */
};

STATIC_VAR NEARDATA const unsigned wizapp[] = {
    PM_HUMAN,      PM_WATER_DEMON,  PM_VAMPIRE,       PM_RED_DRAGON,
    PM_TROLL,      PM_UMBRAL_HULK,   PM_XORN,          PM_XAN,
    PM_COCKATRICE, PM_FLOATING_EYE, PM_GUARDIAN_NAGA, PM_TRAPPER,
};

/* If you've found the Amulet, make the Wizard appear after some time */
/* Also, give hints about portal locations, if amulet is worn/wielded -dlc */
void
amulet()
{
    struct monst *mtmp;
    struct trap *ttmp;
    struct obj *amu;

#if 0 /* caller takes care of this check */
    if (!u.uhave.amulet)
        return;
#endif
    if ((((amu = uamul) != 0 && amu->otyp == AMULET_OF_YENDOR)
         || ((amu = uwep) != 0 && amu->otyp == AMULET_OF_YENDOR))
        && !rn2(15)) {
        for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap) {
            if (ttmp->ttyp == MAGIC_PORTAL) {
                int du = distu(ttmp->tx, ttmp->ty);
                if (du <= 9)
                    pline("%s hot!", Tobjnam(amu, "feel"));
                else if (du <= 64)
                    pline("%s very warm.", Tobjnam(amu, "feel"));
                else if (du <= 144)
                    pline("%s warm.", Tobjnam(amu, "feel"));
                /* else, the amulet feels normal */
                break;
            }
        }
    }

    if (!context.no_of_wizards)
        return;
    /* find Wizard, and wake him if necessary */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->iswiz && mtmp->msleeping && !rn2(40)) {
            if (mtmp->msleeping)
            {
                mtmp->msleeping = 0;
                refresh_m_tile_gui_info(mtmp, TRUE);
            }
            if (distu(mtmp->mx, mtmp->my) > 2)
                You(
      "get the creepy feeling that somebody noticed your taking the Amulet.");
            return;
        }
    }
}

int
mon_has_amulet(mtmp)
register struct monst *mtmp;
{
    register struct obj *otmp;

    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == AMULET_OF_YENDOR)
            return 1;
    return 0;
}

int
mon_has_special(mtmp)
register struct monst *mtmp;
{
    register struct obj *otmp;

    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == AMULET_OF_YENDOR
            || any_quest_artifact(otmp)
            || otmp->otyp == BELL_OF_OPENING
            || otmp->otyp == CANDELABRUM_OF_INVOCATION
            || otmp->otyp == SPE_BOOK_OF_THE_DEAD)
            return 1;
    return 0;
}

/*
 *      New for 3.1  Strategy / Tactics for the wiz, as well as other
 *      monsters that are "after" something (defined via mflag3).
 *
 *      The strategy section decides *what* the monster is going
 *      to attempt, the tactics section implements the decision.
 */
#define STRAT(w, x, y, typ)                            \
    ((unsigned long) (w) | ((unsigned long) (x) << 16) \
     | ((unsigned long) (y) << 8) | (unsigned long) (typ))

#define M_Wants(mask) (mtmp->data->mflags3 & (mask))

STATIC_OVL short
which_arti(mask)
unsigned long mask;
{
    switch (mask) {
    case M3_WANTSAMUL:
        return AMULET_OF_YENDOR;
    case M3_WANTSBELL:
        return BELL_OF_OPENING;
    case M3_WANTSCAND:
        return CANDELABRUM_OF_INVOCATION;
    case M3_WANTSBOOK:
        return SPE_BOOK_OF_THE_DEAD;
    default:
        break; /* 0 signifies quest artifact */
    }
    return 0;
}

/*
 *      If "otyp" is zero, it triggers a check for the quest_artifact,
 *      since bell, book, candle, and amulet are all objects, not really
 *      artifacts right now.  [MRS]
 */
STATIC_OVL boolean
mon_has_arti(mtmp, otyp)
register struct monst *mtmp;
register short otyp;
{
    register struct obj *otmp;

    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj) {
        if (otyp) {
            if (otmp->otyp == otyp)
                return 1;
        } else if (any_quest_artifact(otmp))
            return 1;
    }
    return 0;
}

STATIC_OVL struct monst *
other_mon_has_arti(mtmp, otyp)
register struct monst *mtmp;
register short otyp;
{
    register struct monst *mtmp2;

    for (mtmp2 = fmon; mtmp2; mtmp2 = mtmp2->nmon)
        /* no need for !DEADMONSTER check here since they have no inventory */
        if (mtmp2 != mtmp)
            if (mon_has_arti(mtmp2, otyp))
                return mtmp2;

    return (struct monst *) 0;
}

STATIC_OVL struct obj *
on_ground(otyp)
register short otyp;
{
    register struct obj *otmp;

    for (otmp = fobj; otmp; otmp = otmp->nobj)
        if (otyp) {
            if (otmp->otyp == otyp)
                return otmp;
        } else if (any_quest_artifact(otmp))
            return otmp;
    return (struct obj *) 0;
}

STATIC_OVL boolean
you_have_item(mask)
register unsigned long mask;
{
    switch (mask) {
    case M3_WANTSAMUL:
        return (boolean) u.uhave.amulet;
    case M3_WANTSBELL:
        return (boolean) u.uhave.bell;
    case M3_WANTSCAND:
        return (boolean) u.uhave.menorah;
    case M3_WANTSBOOK:
        return (boolean) u.uhave.book;
    case M3_WANTSARTI:
        return (boolean) u.uhave.questart;
    default:
        break;
    }
    return 0;
}

STATIC_OVL unsigned long
target_on(mask, mtmp)
register unsigned long mask;
register struct monst *mtmp;
{
    register short otyp;
    register struct obj *otmp;
    register struct monst *mtmp2;

    if (!M_Wants(mask))
        return (unsigned long) STRAT_NONE;

    otyp = which_arti(mask);
    if (!mon_has_arti(mtmp, otyp)) {
        if (you_have_item(mask))
            return STRAT(STRAT_PLAYER, u.ux, u.uy, mask);
        else if ((otmp = on_ground(otyp)))
            return STRAT(STRAT_GROUND, otmp->ox, otmp->oy, mask);
        else if ((mtmp2 = other_mon_has_arti(mtmp, otyp)) != 0
                 /* when seeking the Amulet, avoid targetting the Wizard
                    or temple priests (to protect Moloch's high priest) */
                 && (otyp != AMULET_OF_YENDOR
                     || (!mtmp2->iswiz && !inhistemple(mtmp2))))
            return STRAT(STRAT_MONSTR, mtmp2->mx, mtmp2->my, mask);
    }
    return (unsigned long) STRAT_NONE;
}

STATIC_OVL unsigned long
strategy(mtmp)
register struct monst *mtmp;
{
    unsigned long strat, dstrat;

    /* Covetous has been deactivated -- JG */
    if (mtmp->mnum != PM_WIZARD_OF_YENDOR // !is_covetous(mtmp->data)
        /* perhaps a shopkeeper has been polymorphed into a master
           lich; we don't want it teleporting to the stairs to heal
           because that will leave its shop untended */
        || (mtmp->isshk && inhishop(mtmp))
        /* likewise for temple priests */
        || (mtmp->ispriest && inhistemple(mtmp)))
        return (unsigned long) STRAT_NONE;

    switch ((mtmp->mhp * 3) / mtmp->mhpmax) { /* 0-3 */

    default:
    case 0: /* panic time - mtmp is almost snuffed */
        return (unsigned long) STRAT_HEAL;

    case 1: /* the wiz is less cautious */
        if (mtmp->data != &mons[PM_WIZARD_OF_YENDOR])
            return (unsigned long) STRAT_HEAL;
    /* else fall through */

    case 2:
        dstrat = STRAT_HEAL;
        break;

    case 3:
        dstrat = STRAT_NONE;
        break;
    }

    if (context.made_amulet)
        if ((strat = target_on(M3_WANTSAMUL, mtmp)) != STRAT_NONE)
            return strat;

    if (u.uevent.invoked) { /* priorities change once gate opened */
        if ((strat = target_on(M3_WANTSARTI, mtmp)) != STRAT_NONE)
            return strat;
        if ((strat = target_on(M3_WANTSBOOK, mtmp)) != STRAT_NONE)
            return strat;
        if ((strat = target_on(M3_WANTSBELL, mtmp)) != STRAT_NONE)
            return strat;
        if ((strat = target_on(M3_WANTSCAND, mtmp)) != STRAT_NONE)
            return strat;
    } else {
        if ((strat = target_on(M3_WANTSBOOK, mtmp)) != STRAT_NONE)
            return strat;
        if ((strat = target_on(M3_WANTSBELL, mtmp)) != STRAT_NONE)
            return strat;
        if ((strat = target_on(M3_WANTSCAND, mtmp)) != STRAT_NONE)
            return strat;
        if ((strat = target_on(M3_WANTSARTI, mtmp)) != STRAT_NONE)
            return strat;
    }
    return dstrat;
}

void
choose_stairs(sx, sy)
xchar *sx;
xchar *sy;
{
    xchar x = 0, y = 0;

    if (builds_up(&u.uz)) {
        if (xdnstair) {
            x = xdnstair;
            y = ydnstair;
        } else if (xdnladder) {
            x = xdnladder;
            y = ydnladder;
        }
    } else {
        if (xupstair) {
            x = xupstair;
            y = yupstair;
        } else if (xupladder) {
            x = xupladder;
            y = yupladder;
        }
    }

    if (!x && sstairs.sx) {
        x = sstairs.sx;
        y = sstairs.sy;
    }

    if (x && y) {
        *sx = x;
        *sy = y;
    }

}

int
tactics(mtmp)
register struct monst *mtmp;
{
    unsigned long strat = strategy(mtmp);
    xchar sx = 0, sy = 0;

    mtmp->mstrategy =
        (mtmp->mstrategy & (STRAT_WAITMASK | STRAT_APPEARMSG)) | strat;

    switch (strat)
    {
    case STRAT_HEAL: /* hide and recover */
        /* if wounded, hole up on or near the stairs (to block them) */
        choose_stairs(&sx, &sy);
        mtmp->mavenge = 1; /* covetous monsters attack while fleeing */
        if (In_W_tower(mtmp->mx, mtmp->my, &u.uz)
            || (mtmp->iswiz && !sx && !mon_has_amulet(mtmp)))
        {
            if (!rn2(3 + mtmp->mhp / 10))
            {
                (void)rloc2(mtmp, TRUE, TRUE);
            }
        }
        else if (sx && (mtmp->mx != sx || mtmp->my != sy))
        {
            if (!mnearto(mtmp, sx, sy, TRUE, TRUE))
            {
                m_into_limbo(mtmp, TRUE);
                return 0;
            }
        }

        /* if you're not around, cast healing spells */
        if (distu(mtmp->mx, mtmp->my) > (U_NOT_NEARBY_RANGE * U_NOT_NEARBY_RANGE) && !mtmp->mclericspell_used)
            if (mtmp->mhp <= mtmp->mhpmax - 8)
            {
                mtmp->mclericspell_used = 1 + rnd(3);
                mtmp->mhp += rnd(8);
                return 1;
            }
        /*FALLTHRU*/

    case STRAT_NONE: /* harass */
        if (!rn2(!is_fleeing(mtmp) ? 5 : 33))
            mnexto2(mtmp, TRUE);
        return 0;

    default: /* kill, maim, pillage! */
    {
        long where = (strat & STRAT_STRATMASK);
        xchar tx = STRAT_GOALX(strat), ty = STRAT_GOALY(strat);
        unsigned long targ = (strat & STRAT_GOAL);
        struct obj *otmp;

        if (!targ) { /* simply wants you to close */
            return 0;
        }
        if ((u.ux == tx && u.uy == ty) || where == STRAT_PLAYER)
        {
            /* player is standing on it (or has it) */
            mnexto2(mtmp, TRUE);
            return 0;
        }
        if (where == STRAT_GROUND) {
            if (!MON_AT(tx, ty) || (mtmp->mx == tx && mtmp->my == ty)) 
            {
                /* teleport to it and pick it up */
                rloc_to(mtmp, tx, ty); /* clean old pos */

                if ((otmp = on_ground(which_arti(targ))) != 0)
                {
                    if (cansee(mtmp->mx, mtmp->my))
                        pline("%s picks up %s.", Monnam(mtmp),
                              (distu(mtmp->mx, mtmp->my) <= 5)
                                  ? doname(otmp)
                                  : distant_name(otmp, doname));
                    obj_extract_self(otmp);
                    (void) mpickobj(mtmp, otmp);
                    return 1;
                } else
                    return 0;
            } 
            else
            {
                /* a monster is standing on it - cause some trouble */
                if (!rn2(5))
                    mnexto2(mtmp, TRUE);
                return 0;
            }
        } 
        else 
        { /* a monster has it - 'port beside it. */
            if (!mnearto(mtmp, tx, ty, FALSE, TRUE))
                m_into_limbo(mtmp, TRUE);
            return 0;
        }
    }
    }
    /*NOTREACHED*/
    return 0;
}

/* are there any monsters mon could aggravate? */
boolean
has_aggravatables(mon)
struct monst *mon;
{
    struct monst *mtmp;
    boolean in_w_tower = In_W_tower(mon->mx, mon->my, &u.uz);

    if (in_w_tower != In_W_tower(u.ux, u.uy, &u.uz))
        return FALSE;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (in_w_tower != In_W_tower(mtmp->mx, mtmp->my, &u.uz))
            continue;
        if ((mtmp->mstrategy & STRAT_WAITFORU) != 0
            ||!mon_can_move(mtmp))
            return TRUE;
    }
    return FALSE;
}

void
aggravate()
{
    register struct monst *mtmp;
    boolean in_w_tower = In_W_tower(u.ux, u.uy, &u.uz);
    int cnt = 0;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (in_w_tower != In_W_tower(mtmp->mx, mtmp->my, &u.uz))
            continue;
        if (mtmp->iswiz) /* Exclude Wizard of Yendor from aggravation */
            continue;
        mtmp->mstrategy &= ~(STRAT_WAITFORU | STRAT_APPEARMSG);
        boolean refresh = FALSE;
        if (mtmp->msleeping)
        {
            mtmp->msleeping = 0;
            refresh = TRUE;
        }
        if (!mtmp->mcanmove && !rn2(5)) {
            mtmp->mfrozen = 0;
            mtmp->mcanmove = 1;
            refresh = TRUE;
        }
        if (refresh)
        {
            refresh_m_tile_gui_info(mtmp, FALSE);
            cnt++;
        }
    }
    if (cnt > 0)
        flush_screen(1);
}

void
clonewiz()
{
    register struct monst *mtmp2;

    if ((mtmp2 = makemon(&mons[PM_WIZARD_OF_YENDOR], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_NASTY_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END))
        != 0) {
        mtmp2->msleeping = mtmp2->mtame = mtmp2->mpeaceful = 0;
        mtmp2->mon_flags |= MON_FLAGS_CLONED_WIZ;
        if (!u.uhave.amulet && rn2(2)) { /* give clone a fake */
            (void) add_to_minv(mtmp2,
                               mksobj(FAKE_AMULET_OF_YENDOR, TRUE, FALSE, FALSE));
        }
        mtmp2->m_ap_type = M_AP_MONSTER;
        mtmp2->mappearance = wizapp[rn2(SIZE(wizapp))];
        newsym(mtmp2->mx, mtmp2->my);
    }
}

/* also used by newcham() */
int
pick_nasty(summoner_level)
int summoner_level;
{
    int res = NON_PM;
    int roguetrycnt = 0;

    do
    {
        for(int tryct = 0; tryct < 50 && (res == NON_PM || (res >= NON_PM && mons[res].difficulty > (9 * summoner_level + 1) / 10)); tryct++)
        {
            res = nasties[rn2(SIZE(nasties))];
        }

        if(res == NON_PM)
        {
            struct permonst* pm = rndmonst();

            if(pm)
                res = monsndx(pm);
        }
        roguetrycnt++;
    } while ((!Is_really_rogue_level(&u.uz) && res == NON_PM && roguetrycnt <= 5)
        || (Is_really_rogue_level(&u.uz) && (res == NON_PM || !('A' <= mons[res].mlet && mons[res].mlet <= 'Z')) && roguetrycnt <= 5));

    /* Finally, if nothing, just pick something from the nasties list */
    if (res == NON_PM)
        res = nasties[rn2(SIZE(nasties))];

    return res;
}

/* create some nasty monsters, aligned with the caster or neutral; chaotic
   and unaligned are treated as equivalent; if summoner is Null, this is
   for late-game harassment (after the Wizard has been killed at least once
   or the invocation ritual has been performed), in which case we treat
   'summoner' as neutral, since that will produce the greatest number of
   creatures on average (in 3.6.0 and earlier, Null was treated as chaotic);
   returns the number of monsters created */
int
summon_nasties(summoner)
struct monst *summoner;
{
    register struct monst *mtmp;
    register int i;
    //int castalign = (summoner ? sgn(summoner->data->maligntyp) : 0);
    coord bypos;
    int count, census, summon_num, makeindex, s_cls, m_cls;

    if (!summoner || is_cancelled(summoner) || has_summon_forbidden(summoner))
        return 0;

    /* some candidates may be created in groups, so simple count
       of non-null makemon() return is inadequate */
    census = monster_census(FALSE);
    context.makemon_spef_idx = 0;

    if (!rn2(20) && Inhell)
    {
        /* this might summon a demon prince or lord */
        count = msummon((struct monst *) 0); /* summons like WoY */
    }
    else
    {
        count = 0;
        s_cls = summoner ? summoner->data->mlet : 0;
        if (summoner)
        {
            if (summoner != &youmonst && (summoner->iswiz || summoner->m_lev >= 50))
                summon_num = rnd(3) + 1;
            else if (summoner != &youmonst && summoner->m_lev >= 30)
                summon_num = 1 + rnd(2);
            else
                summon_num = rnd(3);
        }
        else
            summon_num = rnd(3) + 1;

        /* if we don't have a casting monster, nasties appear around hero,
           otherwise they'll appear around spot summoner thinks she's at */
        bypos.x = u.ux;
        bypos.y = u.uy;
        for (i = 1; i <= summon_num; i++)
        {
            do
            {
                makeindex = pick_nasty((summoner && summoner != &youmonst) ? summoner->data->difficulty : u.ulevel);
                m_cls = mons[makeindex].mlet;
            } while (summoner
                        && ((attacktype(&mons[makeindex], AT_MAGC)
                            && mons[makeindex].difficulty
                                >= mons[summoner->mnum].difficulty)
                            || (s_cls == S_DEMON && m_cls == S_ANGEL)
                            || (s_cls == S_ANGEL && m_cls == S_DEMON)));

            /* do this after picking the monster to place */
            if (summoner && !enexto(&bypos, summoner->mux, summoner->muy, &mons[makeindex]))
                continue;

            /* this honors genocide but overrides extinction; it ignores
                inside-hell-only (G_HELL) & outside-hell-only (G_NOHELL) */
            if ((mtmp = makemon(&mons[makeindex], bypos.x, bypos.y, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_NASTY_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL))) != 0)
            {
                mtmp->msleeping = mtmp->mpeaceful = mtmp->mtame = 0;
                set_mhostility(mtmp);
                newsym(mtmp->mx, mtmp->my);
            }
            else /* random monster to substitute for geno'd selection */
                mtmp = makemon((struct permonst *) 0, bypos.x, bypos.y, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_NASTY_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL));

            if (mtmp)
            {
                /* delay first use of spell or breath attack */
                mtmp->mspec_used = (3 + rnd(3)) / mon_spec_cooldown_divisor(mtmp);
                mtmp->mmagespell_used = mtmp->mspec_used;
                mtmp->mmageultimate_used = mtmp->mspec_used;
                mtmp->mclericspell_used = mtmp->mspec_used;
                mtmp->mclericultimate_used = mtmp->mspec_used;
                mtmp->mdemonsummon_used = 30;
                mtmp->mspecialsummon_used = 30;
                context.makemon_spef_idx++;
                count++;
            }
        }
    }
    makemon_animation_wait_until_end();

    if (count)
        count = monster_census(FALSE) - census;
    return count;
}

int
summon_level_appropriate_monsters(summoner)
struct monst* summoner;
{
    register struct monst* mtmp;
    register int i;
    coord bypos;
    int count, census, summon_num;

    if (!summoner || is_cancelled(summoner) || has_summon_forbidden(summoner))
        return 0;

    census = monster_census(FALSE);

    count = 0;
    int ml = 0;
    if (summoner && !summoner->iswiz)
    {
        ml = summoner->m_lev;
        if (summoner == &youmonst)
            ml = u.ulevel;
        else
        {
            struct attack* mattk = attacktype_fordmg(summoner->data, AT_MAGC, AD_SPEL);
            if (!mattk)
                mattk = attacktype_fordmg(summoner->data, AT_MAGC, AD_CLRC);
            if (mattk && mattk->mcadj > 0)
                ml = mattk->mcadj;
        }

        if(ml < 8)
            summon_num = 0;
        else if (ml < 14)
            summon_num = 1;
        else if (ml < 20)
            summon_num = rnd(2);
        else if (ml < 26)
            summon_num = 2;
        else if (ml < 32)
            summon_num = 1 + rnd(2);
        else if (ml < 38)
            summon_num = 1 + rnd(3);
        else if (ml < 44)
            summon_num = 2 + rnd(2);
        else
            summon_num = 2 + rnd(3);
    }
    else
        summon_num = 2 + rnd(3);

    /* if we don't have a casting monster, nasties appear around hero,
        otherwise they'll appear around spot summoner thinks she's at */
    bypos.x = u.ux;
    bypos.y = u.uy;
    context.makemon_spef_idx = 0;
    for (i = 1; i <= summon_num; i++)
    {
        mtmp = makemon_limited((struct permonst*) 0, bypos.x, bypos.y, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END, 0, max(1, ml - 1), 0);

        if (mtmp)
        {
            /* delay first use of spell or breath attack */
            mtmp->mspec_used = (3 + rnd(3)) / mon_spec_cooldown_divisor(mtmp);
            mtmp->mmagespell_used = mtmp->mspec_used;
            mtmp->mmageintermediate_used = mtmp->mspec_used;
            mtmp->mmageultimate_used = mtmp->mspec_used;
            mtmp->mclericspell_used = mtmp->mspec_used;
            mtmp->mclericintermediate_used = mtmp->mspec_used;
            mtmp->mclericultimate_used = mtmp->mspec_used;
            mtmp->mdemonsummon_used = 30;
            mtmp->mspecialsummon_used = 30;
            mtmp->mspecialsummon2_used = 30;

            mtmp->mpeaceful = 0;
            newsym(mtmp->mx, mtmp->my);
            count++;
        }
    }
    makemon_animation_wait_until_end();

    if (count)
        count = monster_census(FALSE) - census;
    return count;
}

/* Let's resurrect the wizard, for some unexpected fun. */
void
resurrect()
{
    struct monst *mtmp, **mmtmp;
    long elapsed;
    const char *verb;

    if (!context.no_of_wizards) {
        /* make a new Wizard */
        verb = "kill";
        mtmp = makemon(&mons[PM_WIZARD_OF_YENDOR], u.ux, u.uy, MM_NOWAIT | MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_NASTY_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
        /* affects experience; he's not coming back from a corpse
           but is subject to repeated killing like a revived corpse */
        if (mtmp) mtmp->mrevived = 1;
    } else {
        /* look for a migrating Wizard */
        verb = "elude";
        mmtmp = &migrating_mons;
        while ((mtmp = *mmtmp) != 0) {
            if (mtmp->iswiz
                /* if he has the Amulet, he won't bring it to you */
                && !mon_has_amulet(mtmp)
                && (elapsed = monstermoves - mtmp->mlstmv) > 0L) {
                mon_catchup_elapsed_time(mtmp, elapsed);
                if (elapsed >= LARGEST_INT)
                    elapsed = LARGEST_INT - 1;
                elapsed /= 50L;
                if (mtmp->msleeping && rn2((int) elapsed + 1))
                    mtmp->msleeping = 0;
                if (mtmp->mfrozen == 1) /* would unfreeze on next move */
                    mtmp->mfrozen = 0, mtmp->mcanmove = 1;

                for(int i = 1; i <= LAST_PROP; i++)
                    mtmp->mprops[i] = 0;

                update_all_mon_statistics(mtmp, FALSE);

                if (mon_can_move(mtmp))
                {
                    *mmtmp = mtmp->nmon;
                    mon_arrive(mtmp, TRUE);
                    /* note: there might be a second Wizard; if so,
                       he'll have to wait til the next resurrection */
                    break;
                }
            }
            mmtmp = &mtmp->nmon;
        }
    }

    if (mtmp) {
        mtmp->mtame = mtmp->mpeaceful = 0; /* paranoia */
        set_mhostility(mtmp);
        newsym(mtmp->mx, mtmp->my);
        check_boss_fight(mtmp);
        if (!Deaf) {
            pline("A voice booms out...");
            play_voice_wizard_of_yendor_simple_line(mtmp, 
                !strcmp(verb, "kill") ? WIZARD_OF_YENDOR_LINE_SO_THOU_THOUGHT_THOU_COULDST_KILL_ME : 
                !strcmp(verb, "elude") ? WIZARD_OF_YENDOR_LINE_SO_THOU_THOUGHT_THOU_COULDST_ELUDE_ME : 
                WIZARD_OF_YENDOR_LINE_NONE);
            verbalize("So thou thought thou couldst %s me, fool.", verb);
            if(canseemon(mtmp))
               talkeff(mtmp->mx, mtmp->my);

        }
    }
}

/* Here, we make trouble for the poor shmuck who actually
   managed to do in the Wizard. */
void
intervene()
{
    int which = Is_astralevel(&u.uz) ? rnd(4) : rn2(6);
    /* cases 0 and 5 don't apply on the Astral level */
    switch (which) {
    case 0:
    case 1:
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "vaguely nervous.");
        break;
    case 2:
        if (!Blind)
            You_ex(ATR_NONE, CLR_MSG_WARNING, "notice a %s glow surrounding you.", hcolor(NH_BLACK));
        rndcurse();
        break;
    case 3:
        play_sfx_sound(SFX_AGGRAVATE_MONSTER);
        aggravate();
        break;
    case 4:
        (void) summon_nasties((struct monst *) 0);
        break;
    case 5:
        resurrect();
        break;
    }
}

void
wizdead()
{
    context.no_of_wizards--;
    if (!u.uevent.udemigod) {
        u.uevent.udemigod = TRUE;
        u.udg_cnt = rn1(250, 50);
    }
}

const char *const random_insult[] = {
    "antic",      "blackguard",   "caitiff",    "chucklehead",
    "coistrel",   "craven",       "cretin",     "cur",
    "dastard",    "demon fodder", "dimwit",     "dolt",
    "fool",       "footpad",      "imbecile",   "knave",
    "maledict",   "miscreant",    "niddering",  "poltroon",
    "rattlepate", "reprobate",    "scapegrace", "varlet",
    "villein", /* (sic.) */
    "wittol",     "worm",         "wretch",
};

const char *const random_malediction[] = {
    "Hell shall soon claim thy remains,", "I chortle at thee, thou pathetic",
    "Prepare to die, thou", "Resistance is useless,",
    "Surrender or die, thou", "There shall be no mercy, thou",
    "Thou shalt repent of thy cunning,", "Thou art as a flea to me,",
    "Thou art doomed,", "Thy fate is sealed,",
    "Verily, thou shalt be one dead"
};

/* Insult or intimidate the player */
boolean
cuss(mtmp)
register struct monst *mtmp;
{
    if (Deaf)
        return FALSE;

    boolean res = TRUE;

    if (mtmp->iswiz)
    {
        if (!rn2(5)) /* typical bad guy action */
        {
            play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_LAUGHTER);
            pline("%s laughs fiendishly.", Monnam(mtmp));
        }
        else if (u.uhave.amulet && !rn2(SIZE(random_insult)))
        {
            int iroll = rn2(SIZE(random_insult));
            play_voice_wizard_of_yendor_cuss(mtmp, 11, iroll);
            verbalize("Relinquish the amulet, %s!",
                random_insult[rn2(SIZE(random_insult))]);
        }
        else if (u.uhp < 5 && !rn2(2)) /* Panic */
        {
            int proll = rn2(2);
            int iroll = rn2(SIZE(random_insult));
            play_voice_wizard_of_yendor_cuss(mtmp, proll + 12, iroll);
            verbalize(proll ? "Even now thy life force ebbs, %s!"
                : "Savor thy breath, %s, it be thy last!",
                random_insult[iroll]);
        }
        else if (mtmp->mhp < 5 && !rn2(2)) /* Parthian shot */
        {
            int proll = rn2(2);
            play_voice_wizard_of_yendor_simple_line(mtmp, proll ? WIZARD_OF_YENDOR_LINE_I_SHALL_RETURN : WIZARD_OF_YENDOR_LINE_ILL_BE_BACK);
            verbalize(proll ? "I shall return." : "I'll be back.");
        }
        else
        {
            int mroll = rn2(SIZE(random_malediction));
            int iroll = rn2(SIZE(random_insult));
            play_voice_wizard_of_yendor_cuss(mtmp, mroll, iroll);
            verbalize("%s %s!",
                random_malediction[mroll],
                random_insult[iroll]);
        }
    } 
    else if (is_lminion(mtmp)
               && !(mtmp->isminion && EMIN(mtmp)->renegade)) 
    {
        int cuss_rnd = 1;
        do
        {
            cuss_rnd = rn2(QTN_ANGELIC - 1 + (Hallucination ? 1 : 0));
        }
        while (iflags.using_gui_sounds && (cuss_rnd >= 3 && cuss_rnd <= 5)); /* For the time being, remove ones using god names as difficult to implement when using voice acting */

//        play_voice_monster_cuss_with_god_name(mtmp, cuss_rnd + 1);
        com_pager(mtmp, cuss_rnd
                  + QT_ANGELIC);
    } 
    else 
    {
        if (!rn2(is_minion(mtmp->data) ? 100 : 5))
        {
            play_voice_monster_cuss(mtmp, 0);
            pline("%s casts aspersions on your ancestry.", Monnam(mtmp));
        }
        else
        {
            int cuss_rnd = rn2(QTN_DEMONIC);
//            play_voice_monster_cuss(mtmp, cuss_rnd + 1);
            com_pager(mtmp, cuss_rnd + QT_DEMONIC);
        }
    }
    return res;
}

/*wizard.c*/
