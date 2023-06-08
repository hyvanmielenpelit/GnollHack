/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

/* GnollHack 4.0    exper.c    $NHDT-Date: 1553296396 2019/03/22 23:13:16 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.32 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2007. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#ifndef LONG_MAX
#include <limits.h>
#endif

long
newuexp(lev)
int lev;
{
    if (lev <= 0)
        return 0;

    long total_level_exp = 0L;
    double monsters_to_be_killed = 10.0;
    double base = 1.0525;

    for(int i = 1; i <= lev; i++)
    {
        double levd = (double)i;
        double monst_exp_cur_lvl = 1.0 + levd * levd;
        double monst_exp_prev_lvl = 1.0 + (levd - 1.0) * (levd - 1.0);
        double avg_monst_exp = (monst_exp_cur_lvl + monst_exp_prev_lvl) / 2.0;

        if (levd > 5.0)
            monsters_to_be_killed *= base;

        double exp_needed = avg_monst_exp * monsters_to_be_killed;
        long exp_needed_long = ((long)(exp_needed / 10L) + 1L) * 10L; // Round up to the closest 10
        total_level_exp += exp_needed_long;
    }

    if (total_level_exp >= 2000000)
        total_level_exp = (total_level_exp / 100000) * 100000;
    else if (total_level_exp >= 1000000)
        total_level_exp = (total_level_exp / 50000) * 50000;
    else if (total_level_exp >= 500000)
        total_level_exp = (total_level_exp / 25000) * 25000;
    else if (total_level_exp >= 200000)
        total_level_exp = (total_level_exp / 10000) * 10000;
    else if (total_level_exp >= 100000)
        total_level_exp = (total_level_exp / 5000) * 5000;
    else if (total_level_exp >= 50000)
        total_level_exp = (total_level_exp / 2500) * 2500;
    else if (total_level_exp >= 20000)
        total_level_exp = (total_level_exp / 1000) * 1000; 
    else if (total_level_exp >= 10000)
        total_level_exp = (total_level_exp / 500) * 500; 
    else if (total_level_exp >= 5000)
        total_level_exp = (total_level_exp / 250) * 250;
    else if (total_level_exp >= 2000)
        total_level_exp = (total_level_exp / 100) * 100; 
    else if (total_level_exp >= 1000)
        total_level_exp = (total_level_exp / 50) * 50;
    else if (total_level_exp >= 500)
        total_level_exp = (total_level_exp / 25) * 25;
    else if (total_level_exp >= 200)
        total_level_exp = (total_level_exp / 10) * 10;

    return total_level_exp;
}

/* calculate spell power/energy points for new level */
int
newpw()
{
    int en = 0, enrnd, enfix;

    if (u.ulevel == 0) {
        en = urole.enadv.infix + urace.enadv.infix;
        if (urole.enadv.inrnd > 0)
            en += rn2(urole.enadv.inrnd + 1); //Between 0 and inrnd
        if (urace.enadv.inrnd > 0)
            en += rn2(urace.enadv.inrnd + 1); //Between 0 and inrnd
    } else {
        enfix = 0;// (int)max(0, max(ACURR(A_INT), ACURR(A_WIS)) - 6);
        enrnd = 1;//So that enrnd 1 randomizes between 0 and 1
        if (u.ulevel < urole.xlev) {
            enrnd += urole.enadv.lornd + urace.enadv.lornd;
            enfix += urole.enadv.lofix + urace.enadv.lofix;
        } else {
            enrnd += urole.enadv.hirnd + urace.enadv.hirnd;
            enfix += urole.enadv.hifix + urace.enadv.hifix;
        }
        if (enrnd < 2)
            enrnd = 2;

        en = rn1(enrnd, enfix); // enermod(rn1(enrnd, enfix));
    }
    if (en <= 0)
        en = 1;
    if (u.ulevel < MAXULEV)
        u.ueninc[u.ulevel] = (xchar) en;
    return en;
}

int
enmaxadjustment()
{
    int baseen = u.ubaseenmax;
    int baseadj = ((int)(max(0, max(ACURR(A_INT), ACURR(A_WIS)) - 1) * (u.ulevel + 3)) / 6) + u.ubaseendrain;
    int adj = baseadj;
    int otyp = 0;
    struct obj* uitem;

    for (uitem = invent; uitem; uitem = uitem->nobj)
    {
        otyp = uitem->otyp;
        boolean worn = is_obj_worn(uitem);
        if (!object_uses_spellbook_wand_flags_and_properties(uitem)
            && objects[otyp].oc_mana_bonus != 0)
        {
            boolean inappr = inappropriate_character_type(uitem);

            if ((worn || (!worn && (objects[otyp].oc_pflags & P1_MANA_BONUS_APPLIES_WHEN_CARRIED)))
                && ((!inappr && !(objects[otyp].oc_pflags & (P1_MANA_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    || (objects[otyp].oc_pflags & P1_MANA_BONUS_APPLIES_TO_ALL_CHARACTERS)
                    || (inappr && (objects[otyp].oc_pflags & (P1_MANA_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY | P1_MANA_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)))
                    )
                )
            {
                int multiplier = (((objects[otyp].oc_pflags & P1_CURSED_ITEM_YIELDS_NEGATIVE) && uitem->cursed) || 
                    ((objects[otyp].oc_pflags & P1_MANA_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS) && inappr) ? -1 : 1);

                if (objects[otyp].oc_pflags & P1_MANA_PERCENTAGE_BONUS)
                    adj += multiplier * (objects[otyp].oc_mana_bonus * (baseen + baseadj)) / 100;
                else
                    adj += multiplier * objects[otyp].oc_mana_bonus;
            }
        }
        
        if (has_obj_mythic_mana_gain(uitem) && worn)
        {
            adj += 1 * (MYTHIC_MANA_GAIN_PERCENTAGE * (baseen + baseadj)) / 100;
        }
    }


    return adj;
}


void
updatemaxen()
{
    u.uenmax = u.ubaseenmax + enmaxadjustment();
    if (u.uenmax < 0)
        u.uenmax = 0;
    if (u.uen > u.uenmax)
        u.uen = u.uenmax;
    if (u.uen < 0)
        u.uen = 0;

    context.botl = 1;

    return;
}

/* return # of exp points for mtmp after nk killed */
int
experience(mtmp, nk)
register struct monst *mtmp;
register int nk;
{
    register struct permonst *ptr = mtmp->data;
    int i, tmp, tmp2;

    tmp = 1 + ptr->difficulty * ptr->difficulty; //mtmp->m_lev * mtmp->m_lev;
    tmp += mtmp->extra_encounter_xp;

    if (ptr->difficulty >= 100)
        tmp = (tmp / 100) * 100;
    else if (ptr->difficulty >= 50)
        tmp = (tmp / 50) * 50;
    else if (ptr->difficulty >= 10)
        tmp = (tmp / 10) * 10;
    else if (ptr->difficulty >= 5)
        tmp = (tmp / 5) * 5;

#ifdef MAIL
    /* Mail daemons put up no fight. */
    if (mtmp->data == &mons[PM_MAIL_DAEMON])
        tmp = 1;
#endif

    if (mtmp->mrevived || mtmp->mcloned) {
        /*
         *      Reduce experience awarded for repeated killings of
         *      "the same monster".  Kill count includes all of this
         *      monster's type which have been killed--including the
         *      current monster--regardless of how they were created.
         *        1.. 20        full experience
         *       21.. 40        xp / 2
         *       41.. 80        xp / 4
         *       81..120        xp / 8
         *      121..180        xp / 16
         *      181..240        xp / 32
         *      241..255+       xp / 64
         */
        for (i = 0, tmp2 = 20; nk > tmp2 && tmp > 1; ++i) {
            tmp = (tmp + 1) / 2;
            nk -= tmp2;
            if (i & 1)
                tmp2 += 20;
        }
    }

    return (tmp);
}

long
game_score_difficulty_adjustment(points_added)
long points_added;
{
    long added_gamescore = points_added;

    /* game difficulty adjustment */
    switch (context.game_difficulty)
    {
    case -4:
        added_gamescore /= 100;
        break;
    case -3:
        added_gamescore /= 30;
        break;
    case -2:
        added_gamescore /= 10;
        break;
    case -1:
        added_gamescore /= 3;
        break;
    case 1:
        added_gamescore *= 2;
        break;
    case 2:
        added_gamescore *= 4;
        break;
    default:
        break;
    }

    return added_gamescore;
}


void
more_experienced(exper, gamescore)
register int exper, gamescore;
{
    long added_experience = exper == 0 ? 0 : max(1, exper + (exper * max(-9, u.uexperiencebonus)) / 10);
    long added_gamescore = gamescore;
    long oldexp = u.uexp,
        oldscore = u.u_gamescore,
        newexp = oldexp + added_experience,
        scoreincr = game_score_difficulty_adjustment(4 * added_experience + added_gamescore),
        newscore = oldscore + scoreincr;

    /* cap experience and score on wraparound */
    if (newexp < 0 && added_experience > 0)
        newexp = LONG_MAX;
    if (newscore < 0 && scoreincr > 0)
        newscore = LONG_MAX;

    if (newexp != oldexp) {
        u.uexp = newexp;
        if (flags.showexp)
            context.botl = TRUE;
        
        if(flags.tellexp && newexp - oldexp == added_experience)
            You("gain %ld experience point%s.", added_experience, added_experience == 1 ? "" : "s");

    }
    /* newrexp will always differ from oldrexp unless they're LONG_MAX */
    if (newscore != oldscore) {
        u.u_gamescore = newscore;
        if (flags.showscore)
            context.botl = TRUE;
    }
    if (u.u_gamescore >= (Role_if(PM_WIZARD) ? 1000 : 2000))
        flags.beginner = 0;
}

/* e.g., hit by drain life attack */
void
losexp(drainer)
const char *drainer; /* cause of death, if drain should be fatal */
{
    register int num;

    /* override life-drain resistance when handling an explicit
       wizard mode request to reduce level; never fatal though */
    if (drainer && !strcmp(drainer, "#levelchange"))
        drainer = 0;
    else if (resists_drli(&youmonst))
        return;

    play_sfx_sound(SFX_LOSE_LEVEL);
    if (u.ulevel > 1)
    {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s level %d.", Goodbye(), u.ulevel);
        u.ulevel--;
        /* remove intrinsic abilities */
        adjabil(u.ulevel + 1, u.ulevel);
        reset_rndmonst(NON_PM); /* new monster selection */
    }
    else 
    {
        if (drainer) {
            killer.format = KILLED_BY;
            if (killer.name != drainer)
                Strcpy(killer.name, drainer);
            done(DIED);
        }
        /* no drainer or lifesaved */
        u.uexp = 0;
    }
    num = (int) u.uhpinc[u.ulevel];
    u.ubasehpmax -= num;
    u.uhp -= num;
    updatemaxhp();
    
    if (u.uhp < 1)
        u.uhp = 1;
    else if (u.uhp > u.uhpmax)
        u.uhp = u.uhpmax;

    num = (int) u.ueninc[u.ulevel];
    u.ubaseenmax -= num;
    u.uen -= num;
    updatemaxen();
    if (u.uenmax < 0)
        u.uenmax = 0;
    if (u.uen < 0)
        u.uen = 0;
    else if (u.uen > u.uenmax)
        u.uen = u.uenmax;

    if (u.uexp > 0)
        u.uexp = newuexp(u.ulevel) - 1;

    if (Upolyd) {
        num = monbasehp_per_lvl(&youmonst);
        u.basemhmax -= num;
        u.mh -= num;
        updatemaxhp();
        if (u.mh <= 0)
            rehumanize();
    }

    /* Monks */
    find_ac();
    find_mc();

    context.botl = TRUE;

    if(drainer && !strcmp(drainer, "life drainage"))
        standard_hint("Acquire drain resistance to protect yourself against level draining. You can restore lost levels by using a potion of restore ability or a cloudberry.", &u.uhint.got_level_drained);

}

/*
 * Make experience gaining similar to AD&D(tm), whereby you can at most go
 * up by one level at a time, extra expr possibly helping you along.
 * After all, how much real experience does one get shooting a wand of death
 * at a dragon created with a wand of polymorph??
 */
void
newexplevel()
{
    if (u.ulevel < MAXULEV && u.uexp >= newuexp(u.ulevel))
        pluslvl(TRUE);
}

void
pluslvl(incr)
boolean incr; /* true iff via incremental experience growth */
{             /*        (false for potion of gain level)    */
    int hpinc, eninc;

    if (!incr)
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "more experienced.");

    play_sfx_sound(SFX_GAIN_LEVEL);

    /* increase hit points (when polymorphed, do monster form first
       in order to retain normal human/whatever increase for later) */
    if (Upolyd) {
        hpinc = monbasehp_per_lvl(&youmonst);
        u.basemhmax += hpinc;
        u.mh += hpinc;
    }
    hpinc = newhp();
    u.ubasehpmax += hpinc;
    u.uhp += hpinc;

    /* increase spell power/energy points */
    eninc = newpw();
    u.ubaseenmax += eninc;
    u.uen += eninc;

    /* increase level (unless already maxxed) */
    if (u.ulevel < MAXULEV) 
    {
        int prevrank = xlev_to_rank(u.ulevel);
        /* increase experience points to reflect new level */
        if (incr) {
            long tmp = newuexp(u.ulevel + 1);
            if (u.uexp >= tmp)
                u.uexp = tmp - 1;
        } else {
            u.uexp = newuexp(u.ulevel);
        }
        ++u.ulevel;
        boolean welcomenormal = (u.ulevelmax < u.ulevel);
        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Welcome %sto experience level %d.",
            welcomenormal ? "" : "back ",
              u.ulevel);
        if (u.ulevelmax < u.ulevel)
            u.ulevelmax = u.ulevel;
        adjabil(u.ulevel - 1, u.ulevel); /* give new intrinsics */
        reset_rndmonst(NON_PM);          /* new monster selection */

        char lvlbuf[BUFSZ];
        Sprintf(lvlbuf, "Experience Level %d", u.ulevel);
        display_screen_text(lvlbuf, welcomenormal ? "Welcome to" : "Welcome Back to", (const char*)0, SCREEN_TEXT_GAIN_LEVEL, 0, 0, 0UL);

        int currank = xlev_to_rank(u.ulevel);
        if (currank > 0 && currank > prevrank)
        {
            unsigned short rankbit = 1 << (currank - 1);
            if (!(u.uevent.ranks_attained & rankbit))
            {
                u.uevent.ranks_attained |= rankbit;
                const char* ranktxt = rank_of(u.ulevel, Role_switch, flags.female);
                custompline_ex_prefix(ATR_NONE, CLR_MSG_HINT, "NEW RANK", ATR_NONE, NO_COLOR, " - ", ATR_BOLD, CLR_WHITE, 0, "%s", str_upper_start(ranktxt));
                livelog_printf(LL_ACHIEVE,
                    "attained the rank of %s (level %d)",
                    rank_of(u.ulevel, Role_switch, flags.female),
                    u.ulevel);
            }
        }
    }
    updatemaxhp();
    updatemaxen();

    /* Monks */
    find_ac();
    find_mc();

    context.botl = TRUE;
}

/* compute a random amount of experience points suitable for the hero's
   experience level:  base number of points needed to reach the current
   level plus a random portion of what it takes to get to the next level */
long
rndexp(gaining)
boolean gaining; /* gaining XP via potion vs setting XP for polyself */
{
    long minexp, maxexp, diff, factor, result;

    minexp = (u.ulevel == 1) ? 0L : newuexp(u.ulevel - 1);
    maxexp = newuexp(u.ulevel);
    diff = maxexp - minexp, factor = 1L;
    /* make sure that `diff' is an argument which rn2() can handle */
    while (diff >= (long) LARGEST_INT)
        diff /= 2L, factor *= 2L;
    result = minexp + factor * (long) rn2((int) diff);
    /* 3.4.1:  if already at level 30, add to current experience
       points rather than to threshold needed to reach the current
       level; otherwise blessed potions of gain level can result
       in lowering the experience points instead of raising them */
    if (u.ulevel == MAXULEV && gaining) {
        result += (u.uexp - minexp);
        /* avoid wrapping (over 400 blessed potions needed for that...) */
        if (result < u.uexp)
            result = u.uexp;
    }
    return result;
}

/*exper.c*/
