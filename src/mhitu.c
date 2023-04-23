/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

/* GnollHack 4.0    mhitu.c    $NHDT-Date: 1556649298 2019/04/30 18:34:58 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.164 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <math.h>

STATIC_VAR NEARDATA struct obj *mon_currwep = (struct obj *) 0;

STATIC_DCL boolean FDECL(u_slip_free, (struct monst *, struct attack *));
STATIC_DCL int FDECL(passiveum, (struct permonst *, struct monst *,
                                 struct attack *));
STATIC_DCL void FDECL(mayberem, (struct monst *, const char *,
                                 struct obj *, const char *));
STATIC_DCL boolean FDECL(diseasemu, (struct monst *));
STATIC_DCL boolean FDECL(mummyrotmu, (struct monst*));
STATIC_DCL int FDECL(hitmu, (struct monst *, struct attack *, struct obj*));
STATIC_DCL int FDECL(gulpmu, (struct monst *, struct attack *));
STATIC_DCL int FDECL(explmu, (struct monst *, struct attack *, BOOLEAN_P));
STATIC_DCL void FDECL(missmu, (struct monst *, BOOLEAN_P, struct attack *));
STATIC_DCL void FDECL(mswings, (struct monst *, struct obj *, int));
STATIC_DCL void FDECL(wildmiss, (struct monst *, struct attack *));
STATIC_DCL void FDECL(hitmsg, (struct monst *, struct attack *, int, BOOLEAN_P));

/* See comment in mhitm.c.  If we use this a lot it probably should be */
/* changed to a parameter to mhitu. */
STATIC_VAR int dieroll;

STATIC_OVL void
hitmsg(mtmp, mattk, damage, display_hit_tile)
struct monst *mtmp;
struct attack *mattk;
int damage;
boolean display_hit_tile;
{
    int compat;
    const char *pfmt = 0;
    char *Monst_name = Monnam(mtmp);
    boolean use_mhis = FALSE;
    const char* Monst_his = mhis(mtmp);

    /* Note: if opposite gender, "seductively" */
    /* If same gender, "engagingly" for nymph, normal msg for others */
    if ((compat = could_seduce(mtmp, &youmonst, mattk)) != 0
        && !is_cancelled(mtmp) && !mtmp->mspec_used)
    {
        pline("%s %s you %s.", Monst_name,
              !Blind ? "smiles at" : !Deaf && !is_silenced(mtmp) ? "talks to" : "touches",
              (compat == 2) ? "engagingly" : "seductively");
    } else {
        if (damage < 1)
        {
            int clr = NO_COLOR;
            switch (mattk->aatyp) {
            case AT_BITE:
                    pfmt = "%s bites!";
                break;
            case AT_KICK:
                pline("%s kicks%c", Monst_name,
                      thick_skinned(youmonst.data) ? '.' : '!');
                break;
            case AT_STNG:
                pfmt = "%s stings!";
                break;
            case AT_BUTT:
                pfmt = "%s butts!";
                break;
            case AT_TUSK:
            case AT_HORN:
                pfmt = "%s gores!";
                break;
            case AT_TUCH:
                pfmt = "%s touches you!";
                break;
            case AT_TENT:
                pfmt = "%s tentacles suck you!";
                Monst_name = s_suffix(Monst_name);
                break;
            case AT_TAIL:
                use_mhis = TRUE;
                pfmt = "%s lashes %s tail at you!";
                break;
            case AT_EXPL:
            case AT_BOOM:
                clr = CLR_MSG_NEGATIVE;
                pfmt = "%s explodes!";
                break;
            case AT_RAMS:
                pfmt = "%s rams into you!";
                break;
            default:
                pfmt = "%s hits!";
            }
            if (pfmt)
            {
                if(use_mhis)
                    pline_ex(ATR_NONE, clr, pfmt, Monst_name, Monst_his);
                else
                    pline_ex(ATR_NONE, clr, pfmt, Monst_name);
            }
            }
        else
        {
            int clr = NO_COLOR;
            switch (mattk->aatyp) {
            case AT_BITE:
                pfmt = "%s bites for %d damage!";
                break;
            case AT_RAMS:
                pfmt = "%s rams for %d damage!";
                break;
            case AT_KICK:
                pfmt = "%s kicks for %d damage!";
                break;
            case AT_STNG:
                pfmt = "%s stings for %d damage!";
                break;
            case AT_BUTT:
                pfmt = "%s butts for %d damage!";
                break;
            case AT_TUSK:
            case AT_HORN:
                pfmt = "%s gores for %d damage!";
                break;
            case AT_TUCH:
                pfmt = "%s touches you for %d damage!";
                break;
            case AT_TAIL:
                use_mhis = TRUE;
                pfmt = "%s lashes %s tail at you for %d damage!";
                break;
            case AT_TENT:
                pfmt = "%s tentacles suck you for %d damage!";
                Monst_name = s_suffix(Monst_name);
                break;
            case AT_EXPL:
            case AT_BOOM:
                clr = CLR_MSG_NEGATIVE;
                pfmt = "%s explodes for %d damage!";
                break;
            default:
                pfmt = "%s hits for %d damage!";
            }
            if (pfmt)
            {
                if(use_mhis)
                    pline_ex(ATR_NONE, clr, pfmt, Monst_name, Monst_his, damage);
                else
                    pline_ex(ATR_NONE, clr, pfmt, Monst_name, damage);
            }
        }
        if (display_hit_tile)
        {
            enum hit_tile_types hit_tile = get_hit_tile_by_adtyp(mattk->adtyp);
            display_u_being_hit(hit_tile, damage, 0UL);
        }
    }
}

/* monster missed you */
STATIC_OVL void
missmu(mtmp, nearmiss, mattk)
struct monst *mtmp;
boolean nearmiss;
struct attack *mattk;
{
    if (!canspotmon(mtmp))
        map_invisible(mtmp->mx, mtmp->my);

    if (could_seduce(mtmp, &youmonst, mattk) && !is_cancelled(mtmp))
        pline("%s pretends to be friendly.", Monnam(mtmp));
    else
        pline("%s %smisses!", Monnam(mtmp),
              (nearmiss && flags.verbose) ? "just " : "");

    stop_occupation();
}

/* monster swings obj */
STATIC_OVL void
mswings(mtmp, otemp, strikeindex)
struct monst *mtmp;
struct obj *otemp;
int strikeindex;
{
    if (flags.verbose && !Blind && mon_visible(mtmp)) 
    {
        pline("%s %s %s%s %s%s.", Monnam(mtmp),
              (objects[otemp->otyp].oc_dir & PIERCE) ? "thrusts" : "swings",
              (otemp->quan > 1L) ? "one of " : "", mhis(mtmp), xname(otemp),
            !strikeindex ? "" : strikeindex == 1 ? " a second time" : strikeindex == 2 ? " a third time" : " once more");
    }
}

/* return how a poison attack was delivered */
const char *
mpoisons_subj(mtmp, mattk)
struct monst *mtmp;
struct attack *mattk;
{
    if (mattk->aatyp == AT_WEAP) {
        struct obj *mwep = (mtmp == &youmonst) ? uwep : MON_WEP(mtmp);
        /* "Foo's attack was poisoned." is pretty lame, but at least
           it's better than "sting" when not a stinging attack... */
        return (!mwep || !mwep->opoisoned) ? "attack" : "weapon";
    } else {
        return (mattk->aatyp == AT_TUCH) ? "contact"
                  : (mattk->aatyp == AT_GAZE) ? "gaze"
                       : (mattk->aatyp == AT_BITE) ? "bite" : "sting";
    }
}

/* called when your intrinsic speed is taken away */
void
u_slow_down()
{
    /* OBSOLETE, now shade really slows */
    HFast = 0L;
    if (!Fast)
        You("slow down.");
    else /* speed boots */
        Your("quickness feels less natural.");
    exercise(A_DEX, FALSE);
}

/* monster attacked your displaced image */
STATIC_OVL void
wildmiss(mtmp, mattk)
struct monst *mtmp;
struct attack *mattk;
{
    int compat;
    const char *Monst_name; /* Monnam(mtmp) */

    /* no map_invisible() -- no way to tell where _this_ is coming from */

    if (!flags.verbose)
        return;
    if (!cansee(mtmp->mx, mtmp->my))
        return;
    /* maybe it's attacking an image around the corner? */

    compat = ((mattk->adtyp == AD_SEDU || mattk->adtyp == AD_SSEX)
              ? could_seduce(mtmp, &youmonst, mattk) : 0);
    Monst_name = Monnam(mtmp);

    if (is_blinded(mtmp) || (Invis && !has_see_invisible(mtmp))) {
        const char *swings = (mattk->aatyp == AT_BITE) ? "snaps"
                             : (mattk->aatyp == AT_KICK) ? "kicks"
                             : (mattk->aatyp == AT_RAMS) ? "rams"
                             : (mattk->aatyp == AT_TAIL) ? "lashes tail"
                               : (mattk->aatyp == AT_STNG
                                  || mattk->aatyp == AT_BUTT
                                   || mattk->aatyp == AT_HORN
                                   || mattk->aatyp == AT_TUSK
                                   || nolimbs(mtmp->data)) ? "lunges"
                                 : "swings";

        if (compat)
            pline("%s tries to touch you and misses!", Monst_name);
        else
            switch (rn2(3)) {
            case 0:
                pline("%s %s wildly and misses!", Monst_name, swings);
                break;
            case 1:
                pline("%s attacks a spot beside you.", Monst_name);
                break;
            case 2:
                pline("%s %s at %s!", Monst_name, mattk->aatyp == AT_RAMS ? "rams" : "strikes",
                      (levl[mtmp->mux][mtmp->muy].typ == WATER)
                        ? "empty water"
                        : "thin air");
                break;
            default:
                pline("%s %s wildly!", Monst_name, swings);
                break;
            }

    } else if (Displaced) {
        /* give 'displaced' message even if hero is Blind */
        if (compat)
            pline("%s smiles %s at your %sdisplaced image...", Monst_name,
                  (compat == 2) ? "engagingly" : "seductively",
                  Invis ? "invisible " : "");
        else
            pline("%s %s at your %sdisplaced image and misses you!",
                  /* Note:  if you're both invisible and displaced, only
                   * monsters which see invisible will attack your displaced
                   * image, since the displaced image is also invisible. */
                  Monst_name, mattk->aatyp == AT_RAMS ? "rams" : "strikes", Invis ? "invisible " : "");

    } else if (Underwater) {
        /* monsters may miss especially on water level where
           bubbles shake the player here and there */
        if (compat)
            pline("%s reaches towards your distorted image.", Monst_name);
        else
            pline("%s is fooled by water reflections and misses!",
                  Monst_name);

    } else
        impossible("%s attacks you without knowing your location?",
                   Monst_name);
}

void
expels(mtmp, mdat, message)
struct monst *mtmp;
struct permonst *mdat; /* if mtmp is polymorphed, mdat != mtmp->data */
boolean message;
{
    if (message) {
        if (is_animal(mdat)) {
            You("get regurgitated!");
        } else {
            char blast[40];
            struct attack *attk = attacktype_fordmg(mdat, AT_ENGL, AD_ANY);

            blast[0] = '\0';
            if (!attk) {
                impossible("Swallower has no engulfing attack?");
            } else {
                if (is_whirly(mdat)) {
                    switch (attk->adtyp) {
                    case AD_ELEC:
                        Strcpy(blast, " in a shower of sparks");
                        break;
                    case AD_COLD:
                        Strcpy(blast, " in a blast of frost");
                        break;
                    }
                } else {
                    Strcpy(blast, " with a squelch");
                }
                You("get expelled from %s%s!", mon_nam(mtmp), blast);
            }
        }
    }
    unstuck(mtmp); /* ball&chain returned in unstuck() */
    mnexto(mtmp);
    newsym(u.ux, u.uy);
    spoteffects(TRUE);
    /* to cover for a case where mtmp is not in a next square */
    if (um_dist(mtmp->mx, mtmp->my, 1))
        pline("Brrooaa...  You land hard at some distance.");
}

/* select a monster's next attack, possibly substituting for its usual one */
struct attack *
getmattk(magr, mdef, indx, prev_result, alt_attk_buf)
struct monst *magr, *mdef;
int indx, prev_result[];
struct attack *alt_attk_buf;
{
    struct permonst *mptr = magr->data;
    struct attack *attk = &mptr->mattk[indx];
    struct obj *weap = (magr == &youmonst) ? uwep : MON_WEP(magr);

    /* honor SEDUCE=0 */
    if (!SYSOPT_SEDUCE) {
        extern const struct attack sa_no[NATTK];

        /* if the first attack is for SSEX damage, all six attacks will be
           substituted (expected succubus/incubus handling); if it isn't
           but another one is, only that other one will be substituted */
        if (mptr->mattk[0].adtyp == AD_SSEX) {
            *alt_attk_buf = sa_no[indx];
            attk = alt_attk_buf;
        } else if (attk->adtyp == AD_SSEX) {
            *alt_attk_buf = *attk;
            attk = alt_attk_buf;
            attk->adtyp = AD_DRLI;
        }
    }

    /* prevent a monster with two consecutive disease or hunger attacks
       from hitting with both of them on the same turn; if the first has
       already hit, switch to a stun attack for the second */
    if (indx > 0 && prev_result[indx - 1] > 0
        && (attk->adtyp == AD_DISE || attk->adtyp == AD_ROTS || attk->adtyp == AD_PEST
            || attk->adtyp == AD_FAMN)
        && attk->adtyp == mptr->mattk[indx - 1].adtyp) {
        *alt_attk_buf = *attk;
        attk = alt_attk_buf;
        attk->adtyp = AD_STUN;

    /* make drain-energy damage be somewhat in proportion to energy */
    } else if (attk->adtyp == AD_DREN && mdef == &youmonst) {
        int ulev = max(u.ulevel, 6);

        *alt_attk_buf = *attk;
        attk = alt_attk_buf;
        /* 3.6.0 used 4d6 but since energy drain came out of max energy
           once current energy was gone, that tended to have a severe
           effect on low energy characters; it's now 2d6 with ajustments */
        if (u.uen <= 5 * ulev && attk->damn > 1) {
            attk->damn -= 1; /* low energy: 2d6 -> 1d6 */
            if (u.uenmax <= 2 * ulev && attk->damd > 3)
                attk->damd -= 3; /* very low energy: 1d6 -> 1d3 */
        } else if (u.uen > 12 * ulev) {
            attk->damn += 1; /* high energy: 2d6 -> 3d6 */
            if (u.uenmax > 20 * ulev)
                attk->damd += 3; /* very high energy: 3d6 -> 3d9 */
            /* note: 3d9 is slightly higher than previous 4d6 */
        }

    } else if (attk->aatyp == AT_ENGL && magr->mspec_used) {
        /* can't re-engulf yet; switch to simpler attack */
        *alt_attk_buf = *attk;
        attk = alt_attk_buf;
        if (attk->adtyp == AD_ACID || attk->adtyp == AD_ELEC
            || attk->adtyp == AD_COLD || attk->adtyp == AD_FIRE) {
            attk->aatyp = AT_TUCH;
        } else {
            attk->aatyp = AT_CLAW; /* attack message will be "<foo> hits" */
            attk->adtyp = AD_PHYS;
        }
        attk->damn = 1; /* relatively weak: 1d6 */
        attk->damd = 6;

    /* barrow wight, Nazgul, erinys have weapon attack for non-physical
       damage; force physical damage if attacker has been cancelled or
       if weapon is sufficiently interesting; a few unique creatures
       have two weapon attacks where one does physical damage and other
       doesn't--avoid forcing physical damage for those */
    } else if (indx == 0 && magr != &youmonst
               && attk->aatyp == AT_WEAP && attk->adtyp != AD_PHYS
               && !(mptr->mattk[1].aatyp == AT_WEAP
                    && mptr->mattk[1].adtyp == AD_PHYS)
               && (is_cancelled(magr)
                   || (weap && ((weap->otyp == CORPSE
                                 && touch_petrifies(&mons[weap->corpsenm]))
                                || weap->oartifact == ART_STORMBRINGER || weap->oartifact == ART_MOURNBLADE
                                || weap->oartifact == ART_VORPAL_BLADE)))) {
        *alt_attk_buf = *attk;
        attk = alt_attk_buf;
        attk->adtyp = AD_PHYS;
    }
    return attk;
}

/*
 * mattacku: monster attacks you
 *      returns 1 if monster dies (e.g. "yellow light"), 0 otherwise
 *      Note: if you're displaced or invisible the monster might attack the
 *              wrong position...
 *      Assumption: it's attacking you or an empty square; if there's another
 *              monster which it attacks by mistake, the caller had better
 *              take care of it...
 */
int
mattacku(mtmp)
register struct monst *mtmp;
{
    struct attack *mattk, alt_attk;
    int i, j = 0, tmp, sum[NATTK];
    struct permonst *mdat = mtmp->data;
    boolean ranged = (distu(mtmp->mx, mtmp->my) > 3);
    /* Is it near you?  Affects your actions */
    boolean range2 = !monnear(mtmp, mtmp->mux, mtmp->muy);
    /* Does it think it's near you?  Affects its actions */
    boolean foundyou = (mtmp->mux == u.ux && mtmp->muy == u.uy);
    /* Is it attacking you or your image? */
    boolean youseeit = canseemon(mtmp);
    /* Might be attacking your image around the corner, or
     * invisible, or you might be blind....
     */
    boolean skipnonmagc = FALSE;
    /* Are further physical attack attempts useless? */

    if (!ranged)
        nomul(0);
    if (DEADMONSTER(mtmp) || (Underwater && !is_swimmer(mtmp->data)))
        return 0;

    /* If swallowed, can only be affected by u.ustuck */
    if (u.uswallow) {
        if (mtmp != u.ustuck)
            return 0;
        u.ustuck->mux = u.ux;
        u.ustuck->muy = u.uy;
        range2 = 0;
        foundyou = 1;
        if (u.uinvulnerable)
            return 0; /* stomachs can't hurt you! */

    } else if (u.usteed) {
        if (mtmp == u.usteed)
            /* Your steed won't attack you */
            return 0;
        /* Orcs like to steal and eat horses and the like */
        if (!rn2(is_orc(mtmp->data) ? 2 : 4)
            && distu(mtmp->mx, mtmp->my) <= 2) {
            /* Attack your steed instead */
            bhitpos.x = u.usteed->mx;
            bhitpos.y = u.usteed->my;
            i = mattackm(mtmp, u.usteed);
            if ((i & MM_AGR_DIED))
                return 1;
            /* make sure steed is still alive and within range */
            if ((i & MM_DEF_DIED) || !u.usteed
                || distu(mtmp->mx, mtmp->my) > 2)
                return 0;
            /* Let your steed retaliate */
            bhitpos.x = mtmp->mx;
            bhitpos.y = mtmp->my;
            return !!(mattackm(u.usteed, mtmp) & MM_DEF_DIED);
        }
    }

    update_m_facing(mtmp, u.ux - mtmp->mx, TRUE);

    if (u.uundetected && !range2 && foundyou && !u.uswallow) {
        if (!canspotmon(mtmp))
            map_invisible(mtmp->mx, mtmp->my);
        u.uundetected = 0;
        if (is_hider(youmonst.data) && u.umonnum != PM_TRAPPER) {
            /* ceiling hider */
            coord cc; /* maybe we need a unexto() function? */
            struct obj *obj;

            You("fall from the %s!", ceiling(u.ux, u.uy));
            /* take monster off map now so that its location
               is eligible for placing hero; we assume that a
               removed monster remembers its old spot <mx,my> */
            remove_monster(mtmp->mx, mtmp->my);
            if (!enexto(&cc, u.ux, u.uy, youmonst.data)
                /* a fish won't voluntarily swap positions
                   when it's in water and hero is over land */
                || (mtmp->data->mlet == S_EEL
                    && is_pool(mtmp->mx, mtmp->my)
                    && !is_pool(u.ux, u.uy))) {
                /* couldn't find any spot for hero; this used to
                   kill off attacker, but now we just give a "miss"
                   message and keep both mtmp and hero at their
                   original positions; hero has become unconcealed
                   so mtmp's next move will be a regular attack */
                place_monster(mtmp, mtmp->mx, mtmp->my); /* put back */
                newsym(u.ux, u.uy); /* u.uundetected was toggled */
                pline("%s draws back as you drop!", Monnam(mtmp));
                return 0;
            }

            /* put mtmp at hero's spot and move hero to <cc.x,.y> */
            newsym(mtmp->mx, mtmp->my); /* finish removal */
            place_monster(mtmp, u.ux, u.uy);
            if (mtmp->wormno) {
                worm_move(mtmp);
                /* tail hasn't grown, so if it now occupies <cc.x,.y>
                   then one of its original spots must be free */
                if (m_at(cc.x, cc.y))
                    (void) enexto(&cc, u.ux, u.uy, youmonst.data);
            }
            teleds(cc.x, cc.y, TRUE, FALSE); /* move hero */
            set_apparxy(mtmp);
            newsym(u.ux, u.uy);

            if (youmonst.data->mlet != S_PIERCER)
                return 0; /* lurkers don't attack */

            obj = which_armor(mtmp, WORN_HELMET);
            if (obj && is_metallic(obj)) {
                Your("blow glances off %s %s.", s_suffix(mon_nam(mtmp)),
                     helm_simple_name(obj));
            } else {
                if (3 + find_mac(mtmp) <= rnd(20)) {
                    pline("%s is hit by a falling piercer (you)!",
                          Monnam(mtmp));
                    if (deduct_monster_hp(mtmp, adjust_damage(d(3, 6), &youmonst, mtmp, AD_PHYS, ADFLAGS_NONE)) < 1)
                        killed(mtmp);
                } else
                    pline("%s is almost hit by a falling piercer (you)!",
                          Monnam(mtmp));
            }

        } else {
            /* surface hider */
            if (!youseeit) {
                pline("It tries to move where you are hiding.");
            } else {
                /* Ugly kludge for eggs.  The message is phrased so as
                 * to be directed at the monster, not the player,
                 * which makes "laid by you" wrong.  For the
                 * parallelism to work, we can't rephrase it, so we
                 * zap the "laid by you" momentarily instead.
                 */
                struct obj *obj = level.objects[u.ux][u.uy];

                if (obj || u.umonnum == PM_TRAPPER
                    || (youmonst.data->mlet == S_EEL
                        && is_pool(u.ux, u.uy))) {
                    unsigned long save_speflags = 0; /* suppress warning */

                    if (obj) {
                        save_speflags = obj->speflags;
                        if (obj->otyp == EGG)
                            obj->speflags = 0;
                    }

                    play_sfx_sound(SFX_WAS_HIDING);
                    /* note that m_monnam() overrides hallucination, which is
                       what we want when message is from mtmp's perspective */
                    if (youmonst.data->mlet == S_EEL
                        || u.umonnum == PM_TRAPPER)
                        pline(
                             "Wait, %s!  There's a hidden %s named %s there!",
                              m_monnam(mtmp), mon_monster_name(&youmonst), plname);
                    else
                        pline(
                          "Wait, %s!  There's a %s named %s hiding under %s!",
                              m_monnam(mtmp), mon_monster_name(&youmonst), plname,
                              doname(level.objects[u.ux][u.uy]));
                    if (obj)
                        obj->speflags = save_speflags;
                } else
                    impossible("hiding under nothing?");
            }
            newsym(u.ux, u.uy);
        }
        return 0;
    }

    /* hero might be a mimic, concealed via #monster */
    if (is_mimic(youmonst.data) && U_AP_TYPE && !range2
        && foundyou && !u.uswallow) {
        boolean sticky = sticks(youmonst.data);

        if (!canspotmon(mtmp))
            map_invisible(mtmp->mx, mtmp->my);
        if (sticky && !youseeit)
            pline("It gets stuck on you.");
        else /* see note about m_monnam() above */
            pline("Wait, %s!  That's a %s named %s!", m_monnam(mtmp),
                mon_monster_name(&youmonst), plname);
        if (sticky)
        {
            play_sfx_sound(SFX_ACQUIRE_GRAB);
            u.ustuck = mtmp;
            refresh_m_tile_gui_info(mtmp, FALSE);
            grab_hint(mtmp);
        }
        youmonst.m_ap_type = M_AP_NOTHING;
        youmonst.mappearance = 0;
        newsym(u.ux, u.uy);
        return 0;
    }

    /* non-mimic hero might be mimicking an object after eating m corpse */
    if (U_AP_TYPE == M_AP_OBJECT && !range2 && foundyou && !u.uswallow) {
        if (!canspotmon(mtmp))
            map_invisible(mtmp->mx, mtmp->my);
        if (!youseeit)
            pline("%s %s!", Something, (likes_gold(mtmp->data)
                                        && youmonst.mappearance == GOLD_PIECE)
                                           ? "tries to pick you up"
                                           : "disturbs you");
        else /* see note about m_monnam() above */
            pline("Wait, %s!  That %s is really %s named %s!", m_monnam(mtmp),
                  mimic_obj_name(&youmonst), an(pm_monster_name(&mons[u.umonnum], flags.female)),
                  plname);
        if (multi < 0) { /* this should always be the case */
            char buf[BUFSZ];

            Sprintf(buf, "You appear to be %s again.",
                    Upolyd ? (const char *) an(mon_monster_name(&youmonst))
                           : (const char *) "yourself");
            unmul(buf); /* immediately stop mimicking */
        }
        return 0;
    }

    /*  Work out the armor class differential   */
    tmp = AC_VALUE(u.uac) + 10; /* tmp ~= 0 - 20 */
    tmp += mtmp->m_lev;    //Add level to hit chance
    tmp += m_strdex_to_hit_bonus(mtmp); //Add monster's STR and DEX bonus, thrown weapons are dealt separately
    tmp += mtmp->mhitinc;
    if (multi < 0 || Sleeping || Paralyzed_or_immobile)
        tmp += 4;
    if ((Invis && !has_see_invisible(mtmp)) || is_blinded(mtmp))
        tmp -= 2;
    if (mtmp->mtrapped)
        tmp -= 2;
    if (tmp <= 0)
        tmp = 1;

    /* make eels visible the moment they hit/miss us */
    if (mdat->mlet == S_EEL && mtmp->mundetected && cansee(mtmp->mx, mtmp->my))
    {
        mtmp->mundetected = 0;
        newsym(mtmp->mx, mtmp->my);
    }


    if (u.uinvulnerable) {
        /* monsters won't attack you */
        if (mtmp == u.ustuck) {
            pline("%s loosens its grip slightly.", Monnam(mtmp));
        } else if (!range2) {
            if (youseeit || sensemon(mtmp))
                pline("%s starts to attack you, but pulls back.",
                      Monnam(mtmp));
            else
                You_feel("%s move nearby.", something);
        }
        return 0;
    }

    /* Unlike defensive stuff, don't let them use item _and_ attack. */
    if (find_offensive(mtmp)) {
        int foo = use_offensive(mtmp);

        if (foo != 0)
            return (foo == 1);
    }


    int tmp2 = tmp;
    int weaponattackcount = 0;
    int bite_butt_count = 0;

    boolean first_attack = TRUE;

    for (i = 0; i < NATTK; i++) 
    {
        tmp = tmp2; // Revert hit bonus to original value
        sum[i] = 0;
        mon_currwep = (struct obj *)0;
        mattk = getmattk(mtmp, &youmonst, i, sum, &alt_attk);
        if ((u.uswallow && mattk->aatyp != AT_ENGL)
            || (skipnonmagc && mattk->aatyp != AT_MAGC && mattk->aatyp != AT_SMMN))
            continue;

        if (mattk->aatyp == AT_BITE || mattk->aatyp == AT_BUTT)
            bite_butt_count++;
        if (mtmp->data->heads > 1 && mtmp->heads_left < bite_butt_count)
            continue;

        switch (mattk->aatyp) {
        case AT_CLAW: /* "hand to hand" attacks */
        case AT_KICK:
        case AT_BITE:
        case AT_RAMS:
        case AT_STNG:
        case AT_TUCH:
        case AT_TUSK:
        case AT_HORN:
        case AT_BUTT:
        case AT_TAIL:
        case AT_TENT:
            if (!range2 && (!MON_WEP(mtmp) || is_confused(mtmp) || Conflict || is_crazed(mtmp) || !touch_petrifies(youmonst.data)))
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }
                update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : mattk->aatyp == AT_KICK ? ACTION_TILE_KICK : ACTION_TILE_ATTACK);
                play_monster_simple_weapon_sound(mtmp, i, MON_WEP(mtmp), OBJECT_SOUND_TYPE_SWING_MELEE);
                m_wait_until_action();
                if (foundyou)
                {
                    if (tmp > (j = rnd(20 + i)))
                    {
                        if (mattk->aatyp != AT_KICK || !thick_skinned(youmonst.data))
                            sum[i] = hitmu(mtmp, mattk, (struct obj*) 0);
                    } else
                        missmu(mtmp, (tmp == j), mattk);

                }
                else
                {
                    wildmiss(mtmp, mattk);
                    /* skip any remaining non-spell attacks */
                    skipnonmagc = TRUE;
                }
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            }
            break;

        case AT_HUGS: /* automatic if prev two attacks succeed */
            /* Note: if displaced, prev attacks never succeeded */
            if ((!range2 && ((!hug_requires_two_previous_attacks(mtmp->data) && tmp > (j = rnd(20 + i))) || (hug_requires_two_previous_attacks(mtmp->data) && i >= 2 && sum[i - 1] && sum[i - 2])))
                || mtmp == u.ustuck)
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }

                update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
                play_monster_simple_weapon_sound(mtmp, i, MON_WEP(mtmp), OBJECT_SOUND_TYPE_SWING_MELEE);
                m_wait_until_action();
                sum[i] = hitmu(mtmp, mattk, (struct obj*)0);
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            }
            break;

        case AT_GAZE: /* can affect you either ranged or not */
            /* Medusa gaze already operated through m_respond in
               dochug(); don't gaze more than once per round. */
            if (mdat != &mons[PM_MEDUSA])
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }

                update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
                play_monster_simple_weapon_sound(mtmp, i, MON_WEP(mtmp), OBJECT_SOUND_TYPE_SWING_MELEE);
                m_wait_until_action();
                sum[i] = gazemu(mtmp, mattk);
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            }
            break;

        case AT_EXPL: /* automatic hit if next to, and aimed at you */
            if (!range2)
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }

                update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
                play_monster_simple_weapon_sound(mtmp, i, MON_WEP(mtmp), OBJECT_SOUND_TYPE_SWING_MELEE);
                m_wait_until_action();
                sum[i] = explmu(mtmp, mattk, foundyou);
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            }
            break;

        case AT_ENGL:
            if (!range2) 
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }
                if (foundyou)
                {
                    update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
                    play_monster_simple_weapon_sound(mtmp, i, MON_WEP(mtmp), OBJECT_SOUND_TYPE_SWING_MELEE);
                    m_wait_until_action();
                    if (u.uswallow
                        || (!mtmp->mspec_used && tmp > (j = rnd(20 + i)))) {
                        /* force swallowing monster to be displayed
                           even when hero is moving away */
                        flush_screen(1);
                        sum[i] = gulpmu(mtmp, mattk);
                    } else {
                        missmu(mtmp, (tmp == j), mattk);
                    }
                    update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
                } else if (is_animal(mtmp->data)) {
                    pline("%s gulps some air!", Monnam(mtmp));
                } else {
                    if (youseeit)
                        pline("%s lunges forward and recoils!", Monnam(mtmp));
                    else
                        You_hear("a %s nearby.",
                                 is_whirly(mtmp->data) ? "rushing noise"
                                                       : "splat");
                }
            }
            break;
        case AT_BREA:
            if (range2)
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }
                sum[i] = breamu(mtmp, mattk);
            }
            /* Note: breamu takes care of displacement */
            break;
        case AT_EYES:
            if (!is_blinded(mtmp) && !Reflecting && (!range2 || rn2(6))) /* Blinded already here to prevent continuous blinking */
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }
                sum[i] = eyesmu(mtmp, mattk);
            }
            break;
        case AT_SPIT:
            if (range2)
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }
                sum[i] = spitmu(mtmp, mattk);
            }
            /* Note: spitmu takes care of displacement */
            break;
        case AT_WEAP:
            if (range2) 
            {
                if (!Is_really_rogue_level(&u.uz))
                {
                    thrwmu(mtmp);
                }
            }
            else
            {
                int hittmp = 0;

                /* Rare but not impossible.  Normally the monster
                 * wields when 2 spaces away, but it can be
                 * teleported or whatever....
                 */
                boolean is_nonwelded_launcher = (MON_WEP(mtmp) && (is_launcher(MON_WEP(mtmp)) && !mwelded(MON_WEP(mtmp), mtmp)));
                int min_range = 0, max_range = 1;
                boolean poletooclose = FALSE;
                if (MON_WEP(mtmp) && is_appliable_pole_type_weapon(MON_WEP(mtmp)) && !mwelded(MON_WEP(mtmp), mtmp))
                {
                    get_pole_type_weapon_min_max_distances(MON_WEP(mtmp), mtmp, &min_range, &max_range);
                    poletooclose = distu(mtmp->mx, mtmp->my) < min_range * min_range;
                }

                if (mtmp->weapon_strategy == NEED_WEAPON || !MON_WEP(mtmp) || is_nonwelded_launcher || poletooclose)
                {
                    if ((is_nonwelded_launcher || poletooclose) && !select_hwep(mtmp, FALSE, u.ux, u.uy))
                    {
                        if(canseemon(mtmp))
                            pline("%s unwields %s.", Monnam(mtmp), the(cxname(MON_WEP(mtmp))));
                        setmnotwielded(mtmp, MON_WEP(mtmp));
                    }
                    mtmp->weapon_strategy = poletooclose ? NEED_HTH_NO_POLE : NEED_HTH_WEAPON;
                    /* mon_wield_item resets weapon_strategy as appropriate */
                    if (mon_wield_item(mtmp, FALSE, u.ux, u.uy) != 0)
                        break;
                }

                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }
                update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : ACTION_TILE_ATTACK);
                if (foundyou)
                {
                    weaponattackcount++;
                    if(is_multiweaponmonster(mtmp->data))
                        mon_currwep = select_multiweapon_nth_hwep(mtmp, weaponattackcount);
                    else
                        mon_currwep = MON_WEP(mtmp);

                    int multistrike = 1;
                    int wieldermultistrike = 1;

                    if (mon_currwep) 
                    {
                        hittmp = weapon_to_hit_value(mon_currwep, &youmonst, mtmp, 0);
                        tmp += hittmp;

                        struct multishot_result msres = get_multishot_stats(mtmp, mon_currwep, mon_currwep, FALSE);
                        wieldermultistrike = msres.wielder_attacks;
                        multistrike = msres.weapon_attacks;
                    }
                    int strikeindex;
                    int wielderstrikeindex;
                    for (wielderstrikeindex = 0; wielderstrikeindex < wieldermultistrike; wielderstrikeindex++)
                    {
                        play_monster_simple_weapon_sound(mtmp, i, mon_currwep, OBJECT_SOUND_TYPE_SWING_MELEE);
                        if (mon_currwep && flags.verbose && !Blind && mon_visible(mtmp))
                        {
                            mswings(mtmp, mon_currwep, wielderstrikeindex);
                        }

                        for (strikeindex = 0; strikeindex < multistrike; strikeindex++)
                        {
                            //play_monster_simple_weapon_sound(mtmp, i, mon_currwep, OBJECT_SOUND_TYPE_SWING_MELEE);
                            if (mon_currwep)
                            {
                                if (flags.verbose && !Blind && mon_visible(mtmp) && strikeindex > 0)
                                {
                                    /* To be consistent with mswings */
                                    pline("%s %s %s!", s_suffix(Monnam(mtmp)), aobjnam(mon_currwep, "strike"), strikeindex == 1 ? "a second time" : strikeindex == 2 ? "a third time" : "once more");
                                }
                            }

                            if (strikeindex == 0)
                                m_wait_until_action();

                            //TO-HIT IS DONE HERE
                            if (tmp > (j = dieroll = rnd(20 + i)))
                                sum[i] = hitmu(mtmp, mattk, mon_currwep);
                            else
                                missmu(mtmp, (tmp == j), mattk);

                        }
                    }
                } 
                else
                {
                    wildmiss(mtmp, mattk);
                    /* skip any remaining non-spell attacks */
                    skipnonmagc = TRUE;
                }
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            }
            break;
        case AT_MAGC:
            if(!is_cancelled(mtmp) && !is_silenced(mtmp))
            {
                if (first_attack)
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }
                if (range2)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_DIR);
                    m_wait_until_action();
                    sum[i] = buzzmu(mtmp, mattk);
                    update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
                }
                else
                {
                    sum[i] = castmu(mtmp, mattk, TRUE, foundyou);
                }
            }
            break;

        case AT_SMMN:
            if (!has_summon_forbidden(mtmp))
            {
                if (!mtmp->mdemonsummon_used && mattk->adtyp == AD_DMNS)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_NODIR);
                    m_wait_until_action();
                    /*  Special demon handling code */
                    if ((mtmp->cham == NON_PM) && !range2)
                    { //Chameleons do not summon, others only in close range
                        int chance = mattk->mlevel;
                        if (!is_cancelled(mtmp) && rn2(100) < chance && !item_prevents_summoning(mtmp->mnum))
                        {
                            pline("%s gates in some help.", Monnam(mtmp));
                            (void)msummon(mtmp);
                            sum[i] = 1;
                            mtmp->mdemonsummon_used = 30;
                        }
                        else
                        {
                            if(mtmp->data == &mons[PM_DEMOGORGON])
                                pline("%s hisses angrily.", Monnam(mtmp));
                            else if(is_prince(mtmp->data) || is_lord(mtmp->data))
                                pline("%s concentrates for a moment.", Monnam(mtmp));
                            else
                                pline("%s attempts to gate in some help, but nothing happens.", Monnam(mtmp));
                        }
                    }
                }
                else if (!mtmp->mspecialsummon_used && mattk->adtyp == AD_LYCA)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_NODIR);
                    m_wait_until_action();
                    /*  Special lycanthrope handling code */
                    if ((mtmp->cham == NON_PM) && is_were(mdat) && !range2) 
                    {
                        if (is_human(mdat)) 
                        {
                            if (!rn2(5 - (night() * 2)) && !is_cancelled(mtmp))
                                new_were(mtmp);
                        }
                        else if (!rn2(30) && !is_cancelled(mtmp))
                            new_were(mtmp);
                        mdat = mtmp->data;

                        if (!rn2(10) && !is_cancelled(mtmp) && !item_prevents_summoning(mtmp->mnum)) 
                        {
                            int numseen, numhelp;
                            char buf[BUFSZ], genericwere[BUFSZ];

                            Strcpy(genericwere, "creature");
                            numhelp = were_summon(mdat, FALSE, &numseen, genericwere);
                            mtmp->mdemonsummon_used = 30;
                            if (youseeit)
                            {
                                pline("%s summons help!", Monnam(mtmp));
                                if (numhelp > 0) 
                                {
                                    if (numseen == 0)
                                        You_feel("hemmed in.");
                                }
                                else
                                    pline("But none comes.");
                            }
                            else
                            {
                                const char* from_nowhere;

                                if (!Deaf) 
                                {
                                    pline("%s %s!", Something, makeplural(growl_sound(mtmp)));
                                    from_nowhere = "";
                                }
                                else
                                    from_nowhere = " from nowhere";

                                if (numhelp > 0)
                                {
                                    if (numseen < 1)
                                        You_feel("hemmed in.");
                                    else 
                                    {
                                        if (numseen == 1)
                                            Sprintf(buf, "%s appears", an(genericwere));
                                        else
                                            Sprintf(buf, "%s appear",
                                                makeplural(genericwere));
                                        pline("%s%s!", upstart(buf), from_nowhere);
                                    }
                                } /* else no help came; but you didn't know it tried */
                            }
                        }
                    }
                }
                else if (!mtmp->mspecialsummon_used && mattk->adtyp == AD_GNOL)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_NODIR);
                    m_wait_until_action();
                    /*  Special gnoll handling code */
                    if ((mtmp->cham == NON_PM) && !range2)
                    { //Chameleons do not summon, others only in close range
                        int chance = mattk->mlevel;
                        if (!is_cancelled(mtmp) && rn2(100) < chance && !item_prevents_summoning(mtmp->mnum))
                        {
                            (void)yeenaghu_gnoll_summon(mtmp);
                            sum[i] = 1;
                            mtmp->mspecialsummon_used = 45;
                        }
                        else
                        {
                            if ((!rn2(2) || is_silenced(mtmp))
                                && (m_carrying(mtmp, TRIPLE_HEADED_FLAIL) 
                                    || m_carrying(mtmp, DOUBLE_HEADED_FLAIL) || m_carrying(mtmp, FLAIL)
                                    || m_carrying(mtmp, RUNED_FLAIL)))
                                pline("%s swings his flail commandingly.", Monnam(mtmp));
                            else if (!Deaf)
                            {
                                play_monster_special_dialogue_line(mtmp, YEENAGHU_LINE_GROWLS);
                                pline("%s growls menacingly.", Monnam(mtmp));
                            }
                        }
                    }
                }
                else if (!mtmp->mspecialsummon2_used && mattk->adtyp == AD_GHUL)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_NODIR);
                    m_wait_until_action();
                    /*  Special ghoul handling code */
                    if ((mtmp->cham == NON_PM) && !range2)
                    { //Chameleons do not summon, others only in close range
                        int chance = mattk->mlevel;
                        if (!is_cancelled(mtmp) && rn2(100) < chance && !item_prevents_summoning(mtmp->mnum))
                        {
                            (void)yeenaghu_ghoul_summon(mtmp);
                            sum[i] = 1;
                            mtmp->mspecialsummon2_used = 45;
                        }
                        else if(!is_silenced(mtmp) && !Deaf)
                        {
                            if (!rn2(2))
                            {
                                play_monster_special_dialogue_line(mtmp, YEENAGHU_LINE_BLOOD_CURDLING_HOWL);
                                pline("%s lets loose a blood-curdling howl!", Monnam(mtmp));
                            }
                            else
                            {
                                play_monster_special_dialogue_line(mtmp, YEENAGHU_LINE_HOWLS);
                                pline("%s howls!", Monnam(mtmp));
                            }
                        }
                    }
                }
                else if (!mtmp->mspecialsummon_used && mattk->adtyp == AD_BISN)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_NODIR);
                    m_wait_until_action();
                    /*  Special bison handling code */
                    if ((mtmp->cham == NON_PM) && !range2)
                    { //Chameleons do not summon, others only in close range
                        int chance = mattk->mlevel;
                        if (!is_cancelled(mtmp) && rn2(100) < chance && !item_prevents_summoning(mtmp->mnum))
                        {
                            pline("%s summons some bison!", Monnam(mtmp));
                            (void)yacc_bison_summon();
                            sum[i] = 1;
                            mtmp->mspecialsummon_used = 45;
                        }
                        else if (canseemon(mtmp))
                        {
                            if (!rn2(2) || is_silenced(mtmp))
                                pline("%s raises its head in rage!", Monnam(mtmp));
                            else if(!Deaf)
                                pline("%s grunts threateningly!", Monnam(mtmp));
                        }
                    }
                }
                else if (!mtmp->mspecialsummon_used && mattk->adtyp == AD_UNDO)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_NODIR);
                    m_wait_until_action();
                    /*  Special gnoll handling code */
                    if ((mtmp->cham == NON_PM) && !range2)
                    { //Chameleons do not summon, others only in close range
                        int chance = mattk->mlevel;
                        if (!is_cancelled(mtmp) && rn2(100) < chance && !item_prevents_summoning(mtmp->mnum))
                        {
                            pline("%s summons some undead!", Monnam(mtmp));
                            (void)orcus_undead_summon();
                            sum[i] = 1;
                            mtmp->mspecialsummon_used = 90;
                        }
                        else
                        {
                            if ((!rn2(2) || !canseemon(mtmp) || (!m_carrying(mtmp, MACE_OF_THE_UNDERWORLD) && !m_carrying(mtmp, WAN_DEATH))) && !is_silenced(mtmp) && !Deaf)
                            {
                                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_LAUGHTER);
                                pline("%s laughs at you!", Monnam(mtmp));
                            }
                            else if(canseemon(mtmp) && (m_carrying(mtmp, MACE_OF_THE_UNDERWORLD) || m_carrying(mtmp, WAN_DEATH)))
                                pline("%s swings his wand menacingly.", Monnam(mtmp));
                        }
                    }
                }
                else if (!mtmp->mspecialsummon_used && mattk->adtyp == AD_MINO)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_NODIR);
                    m_wait_until_action();
                    if ((mtmp->cham == NON_PM) && !range2)
                    { //Chameleons do not summon, others only in close range
                        int chance = mattk->mlevel;
                        if (!is_cancelled(mtmp) && !(mvitals[PM_MINOTAUR].mvflags & MV_GONE) && rn2(100) < chance && !item_prevents_summoning(mtmp->mnum))
                        {
                            struct monst* mtmp2 = makemon(&mons[PM_MINOTAUR], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
                            if(mtmp2)
                                pline("%s summons %s.", Monnam(mtmp), a_monnam(mtmp2));
                            sum[i] = 1;
                            mtmp->mspecialsummon_used = 90;
                        }
                        else if(!is_silenced(mtmp) && canseemon(mtmp))
                        {
                            if (!rn2(2))
                            {
                                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_CURSE);
                                pline("%s curses at you!", Monnam(mtmp));
                            }
                            else
                            {
                                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_LAUGHTER);
                                pline("%s laughs menacingly.", Monnam(mtmp));
                            }
                        }
                    }
                }
                else if (!mtmp->mspecialsummon_used && mattk->adtyp == AD_GDRA)
                {
                    update_m_action(mtmp, ACTION_TILE_CAST_NODIR);
                    m_wait_until_action();
                    if ((mtmp->cham == NON_PM) && !range2)
                    { //Chameleons do not summon, others only in close range
                        int chance = mattk->mlevel;
                        if (!is_cancelled(mtmp) && !((mvitals[PM_ANCIENT_GOLD_DRAGON].mvflags & MV_GONE) && (mvitals[PM_GOLD_DRAGON].mvflags & MV_GONE)) && rn2(100) < chance && !item_prevents_summoning(mtmp->mnum))
                        {
                            struct monst* mtmp2 = makemon(&mons[PM_ANCIENT_GOLD_DRAGON], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
                            if (!mtmp2)
                                 mtmp2 = makemon(&mons[PM_GOLD_DRAGON], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION);
                            if (mtmp2)
                                pline("%s summons %s.", Monnam(mtmp), a_monnam(mtmp2));
                            sum[i] = 1;
                            mtmp->mspecialsummon_used = 120;
                        }
                        else if (!is_silenced(mtmp) && canseemon(mtmp))
                        {
                            if (!rn2(2))
                                pline("Fumes raise out of %s nostrils!", s_suffix(mon_nam(mtmp)));
                            else
                                pline("%s bellows menacingly.", Monnam(mtmp));
                        }
                    }
                }
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            }

            break;

        default: /* no attack */
            break;
        }
        if (context.botl)
            bot();
        /* give player a chance of waking up before dying -kaa */
        if (sum[i] == 1) { /* successful attack */
            if (u.usleep && u.usleep < monstermoves && !rn2(10)) {
                multi = -1;
                nomovemsg = "The combat suddenly awakens you.";
            }
        }

        if (sum[i] == 2)
            return 1; /* attacker dead */
        if (sum[i] == 3)
            break; /* attacker teleported, no more attacks */
        /* sum[i] == 0: unsuccessful attack */
    }
    return 0;
}

STATIC_OVL boolean
diseasemu(mtmp)
struct monst *mtmp;
{
    if (Sick_resistance) {
        You_feel("a slight illness.");
        return FALSE;
    } 
    else
    {
        /* Sound is played always to indicate reduced timer */
        play_sfx_sound(SFX_CATCH_TERMINAL_ILLNESS);
        make_sick(Sick ? Sick / 3L + 1L : (long) rn1(ACURR(A_CON), 20),
                  mon_monster_name(mtmp), TRUE, HINT_KILLED_TERMINAL_ILLNESS);
        return TRUE;
    }
}

STATIC_OVL boolean
mummyrotmu(mtmp)
struct monst* mtmp;
{
    if (Sick_resistance) {
        You_feel("a slight illness.");
        return FALSE;
    }
    else
    {
        if(!MummyRot)
            play_sfx_sound(SFX_CATCH_MUMMY_ROT);
        make_mummy_rotted(-1L, mon_monster_name(mtmp), TRUE, HINT_KILLED_MUMMY_ROT);
        return TRUE;
    }
}

/* check whether slippery clothing protects from hug or wrap attack */
STATIC_OVL boolean
u_slip_free(mtmp, mattk)
struct monst *mtmp;
struct attack *mattk;
{
    struct obj *obj = (uarmc ? uarmc : (uarmo ? uarmo : uarm));

    if (!obj)
        obj = uarmu;
    if (mattk->adtyp == AD_DRIN)
        obj = uarmh;

    /* if your cloak/armor is greased, monster slips off; this
       protection might fail (33% chance) when the armor is cursed */
    if (obj && (obj->greased || obj->otyp == OILSKIN_CLOAK)
        && (!obj->cursed || rn2(3))) {
        pline("%s %s your %s %s!", Monnam(mtmp),
              (mattk->adtyp == AD_WRAP) ? "slips off of"
                                        : "grabs you, but cannot hold onto",
              obj->greased ? "greased" : "slippery",
              /* avoid "slippery slippery cloak"
                 for undiscovered oilskin cloak */
              (obj->greased || objects[obj->otyp].oc_name_known)
                  ? xname(obj)
                  : cloak_simple_name(obj));

        if (obj->greased && !rn2(2)) {
            pline_The("grease wears off.");
            obj->greased = 0;
            update_inventory();
        }
        return TRUE;
    }
    return FALSE;
}

boolean
check_ability_resistance_success(mtmp, ability, adjustment_to_roll)
struct monst* mtmp;
int ability;
int adjustment_to_roll;
{
    if (!mtmp)
        return FALSE;

    int ability_score = 0;
    int str = 0;
    int dex = 0;
    int con = 0;
    int intl = 0;
    int wis = 0;
    int cha = 0;

    str = m_acurrstr(mtmp);
    dex = m_acurr(mtmp, A_DEX);
    con = m_acurr(mtmp, A_CON);
    intl = m_acurr(mtmp, A_INT);
    wis = m_acurr(mtmp, A_WIS);
    cha = m_acurr(mtmp, A_CHA);

    switch (ability)
    {
    case A_STR:
        ability_score = str;
        break;
    case A_DEX:
        ability_score = dex;
        break;
    case A_CON:
        ability_score = con;
        break;
    case A_INT:
        ability_score = intl;
        break;
    case A_WIS:
        ability_score = wis;
        break;
    case A_CHA:
        ability_score = cha;
        break;
    case A_MAX_INT_WIS:
        ability_score = max(intl, wis);
        break;
    case A_MAX_INT_CHA:
        ability_score = max(intl, cha);
        break;
    case A_MAX_WIS_CHA:
        ability_score = max(wis, cha);
        break;
    case A_MAX_INT_WIS_CHA:
        ability_score = max(max(intl, wis), cha);
        break;
    case A_AVG_INT_WIS:
        ability_score = (intl + wis) / 2;
        break;
    case A_AVG_INT_CHA:
        ability_score = (intl + wis) / 2;
        break;
    case A_AVG_WIS_CHA:
        ability_score = (wis + cha) / 2;
        break;
    case A_AVG_INT_WIS_CHA:
        ability_score = (intl + wis + cha) / 3;
        break;
    default:
        break;
    }

    int adjscore = ability_score + adjustment_to_roll;
    int percentage = 0;

    percentage = 5 * adjscore - 30;

    boolean success = (rn2(100) < percentage);
    return success;
}


boolean
check_magic_cancellation_success(mtmp, adjustment_to_roll)
struct monst* mtmp;
int adjustment_to_roll;
{
    boolean success = (rn2(100) < magic_negation_percentage(((mtmp == &youmonst) ? u.umc : magic_negation(mtmp)) + adjustment_to_roll));
    return success;
}

/* armor that sufficiently covers the body might be able to block magic */
int
magic_negation(mon)
struct monst *mon;
{
    if (!mon)
        return 0;

    struct obj *o;
    long wearmask;

    int natural_mc_base = mons[mon->mnum].mc;
    int natural_mc = natural_mc_base;
    int armor_mc = 0;
    int armor_mc_bonus = 0;

    int mc = 0; /* base magic cancellation for current form */

    boolean is_you = (mon == &youmonst);

    int item_mc_bonus = 0;

    int suit_mc_bonus = 0;
    int robe_mc_bonus = 0;
    int combined_mc_bonus = 0;

    for (o = is_you ? invent : mon->minvent; o; o = o->nobj) 
    {
        /* oc_magic_cancellation field is only applicable for armor (which must be worn), this should exclude spellbooks and wands, which use oc_oc2 for something else */
        /* omit W_SWAPWEP+W_QUIVER; W_ARTIFACT_CARRIED+W_ARTIFACT_INVOKED handled by protects() */
        wearmask = W_WORN_NOT_WIELDED;

        if (is_wielded_item(o) || (objects[o->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED) || has_obj_mythic_defense(o))
            wearmask |= W_WIELDED_WEAPON;

        item_mc_bonus = 0;
        int otyp = o->otyp;
        boolean inappr = inappropriate_monster_character_type(mon, o);
        boolean worn = !!(o->owornmask & wearmask);

        if (worn)
        {
            /* MC always from worn */
            int basemc = get_object_base_mc(o);
            if (objects[o->otyp].oc_flags & O1_EROSION_DOES_NOT_AFFECT_MC)
                item_mc_bonus += basemc;
            else
                item_mc_bonus += max(0, basemc - greatest_erosion(o) / 3);

            if (((o->oclass == ARMOR_CLASS || o->oclass == MISCELLANEOUS_CLASS || (objects[o->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED)) && !(objects[o->otyp].oc_flags & O1_ENCHANTMENT_DOES_NOT_AFFECT_MC))
                || has_obj_mythic_defense(o))
                item_mc_bonus += o->enchantment / 3;

            if (is_shield(o))
                item_mc_bonus += is_you ? shield_skill_mc_bonus(P_SKILL_LEVEL(P_SHIELD)) : 0;
        }

        if ((worn || (!worn && (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_APPLIES_WHEN_CARRIED)))
            && ((!inappr && !(objects[otyp].oc_pflags & (P1_ATTRIBUTE_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                || (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_APPLIES_TO_ALL_CHARACTERS)
                || (inappr && (objects[otyp].oc_pflags & (P1_ATTRIBUTE_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY | P1_ATTRIBUTE_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)))
                )
            )
        {

            /* Note u.umcbonus is not being used at the moment, even though it contains appropriate bonuses for you */
            if (objects[o->otyp].oc_bonus_attributes & BONUS_TO_MC)
            {
                item_mc_bonus += objects[o->otyp].oc_attribute_bonus / ((objects[o->otyp].oc_bonus_attributes & FULL_MC_BONUS) != 0 ? 1 : 3);
                if (!(objects[o->otyp].oc_bonus_attributes & IGNORE_ENCHANTMENT))
                    item_mc_bonus += o->enchantment / ((objects[o->otyp].oc_bonus_attributes & FULL_MC_BONUS) != 0 ? 1 : 3);
            }
        }

        if (o->owornmask == W_ARM)
            suit_mc_bonus = item_mc_bonus;
        else if (o->owornmask == W_ARMO)
            robe_mc_bonus = item_mc_bonus;
        else
            armor_mc_bonus += item_mc_bonus;
    }

    /* Finally, add greated of suit and robe MC bonus */
    combined_mc_bonus = max(suit_mc_bonus, robe_mc_bonus);
    armor_mc_bonus += combined_mc_bonus;
    if (is_you)
    {
        context.suit_yielding_mc_bonus = (suit_mc_bonus == combined_mc_bonus);
        context.robe_yielding_mc_bonus = (robe_mc_bonus == combined_mc_bonus);
    }

    natural_mc += armor_mc_bonus / 3;
    armor_mc += armor_mc_bonus + natural_mc_base / 3;

    /* Pick better of armor mc and natural mc */
    if (natural_mc >= armor_mc)
        mc = natural_mc;
    else
        mc = armor_mc;

    if (is_you)
    {
        if (Magical_stoneskin)
            mc += 6;
        else if (Magical_barkskin)
            mc += 4;
        else if (Magical_shielding)
            mc += 2;
        else if (Magical_protection)
            mc += 1;

        /* Divine protection */
        mc += u.ublessed / 3;

        /* Monk protection */
        mc += get_role_MC_bonus();
    }
    else
    {
        if (mon->mprops[MAGICAL_STONESKIN] != 0)
            mc += 6;
        else if (mon->mprops[MAGICAL_BARKSKIN] != 0)
            mc += 4;
        else if (mon->mprops[MAGICAL_SHIELDING] != 0)
            mc += 2;
        else if (mon->mprops[MAGICAL_PROTECTION] != 0)
            mc += 1;
    }

    return mc;
}


int
magic_negation_percentage(mclevel)
int mclevel;
{
    if (mclevel <= 0)
        return 0;
    if (mclevel > 25)
        return 99;

    int mcpercentage = 0;

    if (mclevel > 16)
    {
        mcpercentage = mclevel + 74;
    }
    else
    {
        switch (mclevel)
        {
        case 1:
            mcpercentage = 10;
            break;
        case 2:
            mcpercentage = 20;
            break;
        case 3:
            mcpercentage = 30;
            break;
        case 4:
            mcpercentage = 40;
            break;
        case 5:
            mcpercentage = 50;
            break;
        case 6:
            mcpercentage = 55;
            break;
        case 7:
            mcpercentage = 60;
            break;
        case 8:
            mcpercentage = 65;
            break;
        case 9:
            mcpercentage = 70;
            break;
        case 10:
            mcpercentage = 75;
            break;
        case 11:
            mcpercentage = 80;
            break;
        case 12:
            mcpercentage = 82;
            break;
        case 13:
            mcpercentage = 84;
            break;
        case 14:
            mcpercentage = 86;
            break;
        case 15:
            mcpercentage = 88;
            break;
        case 16:
            mcpercentage = 90;
            break;
        default:
            mcpercentage = 0;
            break;
        }
    }
    return mcpercentage;
}


/*
 * hitmu: monster hits you
 *        returns 2 if monster dies (e.g. "yellow light"), 1 otherwise
 *        3 if the monster lives but teleported/paralyzed, so it can't keep
 *             attacking you
 */
STATIC_OVL int
hitmu(mtmp, mattk, omonwep)
register struct monst *mtmp;
register struct attack *mattk;
register struct obj* omonwep;
{
    struct permonst *mdat = mtmp->data;
    int uncancelled, ptmp;
    int extradmg = 0, permdmg, tmphp;
    double damage = 0;
    char buf[BUFSZ];
    struct permonst *olduasmon = youmonst.data;
    int res;
    boolean objectshatters = FALSE;
    boolean isinstakilled = FALSE;
    boolean isdisintegrated = FALSE;
    boolean sharpness_effect = FALSE;
    int critstrikeroll = rn2(100);

    if (!canspotmon(mtmp))
        map_invisible(mtmp->mx, mtmp->my);

    /*  If the monster is undetected & hits you, you should know where
     *  the attack came from.
     */
    if (mtmp->mundetected && (hides_under(mdat) || mdat->mlet == S_EEL)) {
        mtmp->mundetected = 0;
        if (!(Blind ? (Blind_telepat || Unblind_telepat) : Unblind_telepat)) {
            struct obj *obj;
            const char *what;

            if ((obj = level.objects[mtmp->mx][mtmp->my]) != 0) {
                if (Blind && !obj->dknown)
                    what = something;
                else if (is_pool(mtmp->mx, mtmp->my) && !Underwater)
                    what = "the water";
                else
                    what = doname(obj);

                play_sfx_sound(SFX_WAS_HIDING);
                pline("%s was hidden under %s!", Amonnam(mtmp), what);
            }
            newsym(mtmp->mx, mtmp->my);
        }
    }

    /*  First determine the base damage done */
    struct obj* mweapon = omonwep; // MON_WEP(mtmp);
    boolean uses_spell_flags = omonwep ? object_uses_spellbook_wand_flags_and_properties(omonwep) : FALSE;
    int increase_damage_adtyp = AD_MAGM;

    if (mweapon)
    {
        //Use weapon damage
        if (is_launcher(mweapon))
            damage += d(1, 2);
        else
        {
            int basedmg = weapon_dmg_value(mweapon, &youmonst, mtmp, 0);
            damage += adjust_damage(basedmg, mtmp, &youmonst, objects[mweapon->otyp].oc_damagetype, ADFLAGS_NONE);
            extradmg += weapon_extra_dmg_value(mweapon, &youmonst, mtmp, basedmg);
            damage += extradmg;
            increase_damage_adtyp = objects[mweapon->otyp].oc_damagetype;
        }
    }
    else
    {
        int basedmg = 0;
        //Use stats from ATTK
        if (mattk->damn > 0 && mattk->damd > 0)
            basedmg += d((int)mattk->damn, (int)mattk->damd);
        basedmg += (int)mattk->damp;
        damage += adjust_damage(basedmg, mtmp, &youmonst, mattk->adtyp, ADFLAGS_NONE);
        increase_damage_adtyp = mattk->adtyp;
    }
    
    damage += adjust_damage(mtmp->mdaminc, mtmp, &youmonst, increase_damage_adtyp, ADFLAGS_NONE);

    //Get damage bonus in both cases if physical
    if(mattk->adtyp == AD_PHYS || mattk->adtyp == AD_SHRP || mattk->adtyp == AD_DRIN)
    {
        if(omonwep || mattk->aatyp == AT_WEAP || mattk->aatyp == AT_HUGS)
            damage += adjust_damage(m_str_dmg_bonus(mtmp), mtmp, &youmonst, mattk->adtyp, ADFLAGS_NONE);
        else
            damage += adjust_damage(m_str_dmg_bonus(mtmp) / 2, mtmp, & youmonst, mattk->adtyp, ADFLAGS_NONE);
    }

    if (mattk->adtyp == AD_SHRP && (rn2(100) < SHARPNESS_PERCENTAGE_CHANCE))
    {
        damage += adjust_damage(((Upolyd ? u.mhmax : u.uhpmax) * SHARPNESS_MAX_HP_PERCENTAGE_DAMAGE) / 100, mtmp, &youmonst, mattk->adtyp, ADFLAGS_NONE);
        sharpness_effect = TRUE;
    }

    //Let's add this even if a weapon is being used
    if ((is_undead(mdat) || is_vampshifter(mtmp)) && midnight())
    {
        int basedmg = 0;
        if (mattk->damn > 0 && mattk->damd > 0)
            basedmg += d((int) mattk->damn, (int) mattk->damd); /* extra damage */
        basedmg += (int)mattk->damp;
        damage += adjust_damage(basedmg, mtmp, &youmonst, mattk->adtyp, ADFLAGS_NONE);
    }

    //Make sure damage is at least 1
    if (damage < 1)
        damage = 1;


    /*  Next a cancellation factor.
     *  Use uncancelled when cancellation factor takes into account certain
     *  armor's special magic protection.  Otherwise just use !is_cancelled(mtmp).
     */
    boolean mcsuccess = check_magic_cancellation_success(&youmonst, mattk->mcadj);
    uncancelled = !is_cancelled(mtmp) && !mcsuccess;

    permdmg = 0;

    //This is separately applied to hand-to-hand weapons
    //Random additions so that we can correctly calculate the final damage early
    if ((mattk->adtyp == AD_PHYS && mattk->aatyp == AT_HUGS && !sticks(youmonst.data)) || mattk->adtyp == AD_DRIN)
    {
        /* Strength bonus*/
        //dmg += m_str_dmg_bonus(mtmp);

    }
    else 
    {
        /*This happens almost always*/
        /*  Negative armor class reduces damage done instead of fully protecting against hits. */
        if (damage > 1 && u.uac < 0)
        {
            int absac = -u.uac;
            int damage_reduction_max = absac <= 20 ? absac : absac <= 40 ? (20 + (absac - 20) / 2) : (30 + (absac - 40) / 3);

            damage -= (double)rnd(damage_reduction_max);
            if (damage < 1)
                damage = 1;
        }
    }

    /*  Now, adjust damages via resistances or specific attacks */
    int damagedealt = (int)damage + ((damage - (double)((int)damage) - ((double)(Upolyd ? u.mh_fraction : u.uhp_fraction) / 10000)) > 0 ? 1 : 0);
    switch (mattk->adtyp)
    {
    case AD_PHYS:
    {
        if (mattk->aatyp == AT_HUGS && !sticks(youmonst.data))
        {
            if (!u.ustuck && !rn2(2))
            {
                if (u_slip_free(mtmp, mattk))
                {
                    damage = 0;
                }
                else
                {
                    play_sfx_sound(SFX_ACQUIRE_GRAB);
                    u.ustuck = mtmp;
                    if (damagedealt > 0)
                        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s grabs you%s! You sustain %d damage.", Monnam(mtmp), (hug_throttles(mtmp->data) && has_neck(youmonst.data)) ? " by the throat" : "", damagedealt);
                    else
                        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s grabs you%s!", Monnam(mtmp), (hug_throttles(mtmp->data) && has_neck(youmonst.data)) ? " by the throat" : "");

                    update_extrinsics();
                    context.botl = context.botlx = 1;
                    refresh_m_tile_gui_info(mtmp, FALSE);
                    display_u_being_hit(HIT_CRUSHED, damagedealt, 0UL);

                    if (is_constrictor(mtmp->data))
                    {
                        if (hug_throttles(mtmp->data))
                        {
                            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s is choking you to death!", Monnam(mtmp));
                        }
                        else
                        {
                            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s is constricting you to death!", Monnam(mtmp));
                        }
                        if (Breathless)
                            pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, Magical_breathing ? "However, you can still breathe normally." : "However, you do not feel particularly concerned.");
                    }
                    grab_hint(mtmp);
                }
            }
            else if (u.ustuck == mtmp)
            {
                exercise(A_STR, FALSE);
                if (damagedealt > 0)
                {
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are being %s%s You sustain %d damage.",
                        hug_throttles(mtmp->data) ? "choked" : "crushed",
                        is_constrictor(mtmp->data) ? " to death!" : ".",
                        damagedealt);
                    display_u_being_hit(HIT_CRUSHED, damagedealt, 0UL);
                    if (Breathless)
                        pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, Magical_breathing ? "However, you can still breathe normally." : "However, you do not feel particularly concerned.");
                }
                else if (is_constrictor(mtmp->data))
                {
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s is %s you to death!", Monnam(mtmp), hug_throttles(mtmp->data) ? "choked" : "constricting");
                    display_u_being_hit(HIT_CRUSHED, damagedealt, 0UL);
                    if (Breathless)
                        pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, Magical_breathing ? "However, you can still breathe normally." : "However, you do not feel particularly concerned.");
                }
                else
                {
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are being %s%s.", hug_throttles(mtmp->data)
                        ? "choked"
                        : "crushed", damage == 0 ? ", but sustain no damage" : "");
                    display_u_being_hit(HIT_CRUSHED, damagedealt, 0UL);
                }
            }
        }
        else
        { /* hand to hand weapon */
            struct obj* otmp = omonwep; //mon_currwep;

            if (mattk->aatyp == AT_WEAP && otmp)
            {
                if (otmp->otyp == CORPSE && touch_petrifies(&mons[otmp->corpsenm]))
                {
                    damage = 1;
                    pline("%s hits you with the %s corpse.", Monnam(mtmp), corpse_monster_name(otmp));

                    if (check_magic_cancellation_success(&youmonst, 0) || Stone_resistance)
                    {
                        play_sfx_sound(SFX_GENERAL_RESISTS);
                        shieldeff(u.ux, u.uy);
                        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "resist!");
                    }
                    else
                    {
                        if (!Stoned)
                            goto do_stone;
                    }
                }

                /* Negative AC reduces damage */
                if (damage > 1 && u.uac < 0)
                {
                    damage -= (double)rnd(-u.uac);
                    if (damage < 1)
                        damage = 1;
                }
                boolean hittxt = FALSE;
                boolean displaysustain = FALSE;
                int ahres = 0;

                hittxt = (otmp->oartifact
                    && (ahres = artifact_hit(mtmp, &youmonst, otmp, &damage, &isinstakilled, dieroll)));

                if (ahres == 1)
                    displaysustain = TRUE;

                short spec_adtyp = 0;
                int special_hit_dmg = pseudo_artifact_hit(mtmp, &youmonst, otmp, extradmg, dieroll, critstrikeroll, &spec_adtyp);
                if (special_hit_dmg < 0)
                {
                    isinstakilled = TRUE;
                    isdisintegrated = TRUE;
                    hittxt = TRUE; /* This means that hit text is already given */
                }
                else if (special_hit_dmg > 0)
                {
                    damage += adjust_damage(special_hit_dmg, mtmp, &youmonst, spec_adtyp, ADFLAGS_NONE);
                }

                boolean silvermsg = FALSE;
                if (otmp->material == MAT_SILVER && Hate_silver)
                {
                    damage += adjust_damage(rnd(20), mtmp, &youmonst, objects[otmp->otyp].oc_damagetype, ADFLAGS_NONE);
                    silvermsg = TRUE;
                }

                damagedealt = (int)damage + ((damage - (double)((int)damage) - ((double)(Upolyd ? u.mh_fraction : u.uhp_fraction) / 10000)) > 0 ? 1 : 0);

                //Finally, display damage caused
                if (!hittxt)
                    hitmsg(mtmp, mattk, damagedealt, TRUE);
                else if (displaysustain && damagedealt > 0)
                    You("sustain %d damage.", damagedealt);

                if(damagedealt > 0)
                    display_u_being_hit(isdisintegrated ? HIT_DISINTEGRATED : HIT_GENERAL, damagedealt, 0UL);
                
                /* Silver message immediately next */
                if (silvermsg)
                {
                    pline_The("silver sears your flesh!");
                    exercise(A_CON, FALSE);
                }


                /* Check if the object should shatter */
                if (omonwep && is_fragile(omonwep)
                    && !is_obj_indestructible(omonwep)
                    && !is_quest_artifact(omonwep)
                    && !omonwep->oartifact
                    )
                {
                    /* Shattering is done below, here just the message */
                    objectshatters = TRUE;
                    if (omonwep->quan == 1)
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s shatters from the blow!", s_suffix(Monnam(mtmp)), xname(omonwep));
                    else
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "One of %s %s shatters from the blow!", s_suffix(mon_nam(mtmp)), xname(omonwep));
                }


                if (damage == 0)
                    break;


                /* this redundancy necessary because you have
                   to take the damage _before_ being cloned;
                   need to have at least 2 hp left to split */
                if (!isinstakilled && (double)u.mh + (((double)u.mh_fraction)/10000) - damage >= 2 && is_metallic(otmp) /* relevant 'metal' objects are scalpel and tsurugi */
                    && (u.umonnum >= 0 && does_split_upon_hit(&mons[u.umonnum])))
                {
                    if (damage > 1)
                        exercise(A_STR, FALSE);
                    /* inflict damage now; we know it can't be fatal */

                    deduct_player_hp(damage);
                    damage = 0; /* don't inflict more damage below */
                    if (cloneu())
                        You("divide as %s hits you!", mon_nam(mtmp));
                }
                rustm(&youmonst, otmp);
            }
            else if (mattk->aatyp != AT_TUCH || damagedealt != 0
                || mtmp != u.ustuck)
                hitmsg(mtmp, mattk, damagedealt, TRUE);
        }
        break;
    }
    case AD_DISE:
        if (Sick_resistance)
            damage = 0;
        
        hitmsg(mtmp, mattk, damage == 0 ? 0 : damagedealt, FALSE);

        //Must now bypass your MC -- Demogorgon has high penalties for saving throw
        if (!mcsuccess && !Sick_resistance)
        {
            if (diseasemu(mtmp))
                display_u_being_hit(HIT_SICK, damagedealt, 0UL);
            else if(damage > 0)
                display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        }
        else if (damage > 0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);

        break;
    case AD_ROTS:
        //if (Sick_resistance) // Note: Apply still the damage, since this is mummies' main attack.
        //    damage = 0;

        hitmsg(mtmp, mattk, damage == 0 ? 0 : damagedealt, FALSE);

        if (!mcsuccess && !Sick_resistance)
        {
            if(mummyrotmu(mtmp))
                display_u_being_hit(HIT_SICK, damagedealt, 0UL);
            else if (damage > 0)
                display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        }
        else if (damage > 0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_FIRE:
        hitmsg(mtmp, mattk, -1, TRUE);
        //if (uncancelled) 
        {
            play_sfx_sound(SFX_MONSTER_ON_FIRE);
            if (completelyburns(youmonst.data)) 
            { /* paper or straw golem */
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "go up in flames!");
                /* KMH -- this is okay with unchanging */
                rehumanize();
                break;
            } 
            else if (Fire_immunity || damage == 0) 
            {
                pline("You are %s, but the fire doesn't feel hot!", on_fire(youmonst.data, mattk));
                damage = 0;
            }
            else
                pline("You're %s! You sustain %d damage.", on_fire(youmonst.data, mattk), damage == 0 ? 0 : damagedealt);

            display_u_being_hit(HIT_ON_FIRE, damagedealt, 0UL);

            if ((int) mtmp->m_lev > rn2(20))
                destroy_item(SCROLL_CLASS, AD_FIRE);
            if ((int) mtmp->m_lev > rn2(20))
                destroy_item(POTION_CLASS, AD_FIRE);
            if ((int) mtmp->m_lev > rn2(25))
                destroy_item(SPBOOK_CLASS, AD_FIRE);
            burn_away_slime();
            item_destruction_hint(AD_FIRE, FALSE);
        }
        //else
        //    damage = 0;
        break;
    case AD_COLD:
        hitmsg(mtmp, mattk, -1, TRUE);
        //if (uncancelled) 
        {
            play_sfx_sound(SFX_MONSTER_COVERED_IN_FROST);
            if (Cold_immunity || damage == 0)
            {
                play_sfx_sound(SFX_GENERAL_UNHARMED);
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "You're covered in frost, but the frost doesn't feel cold!");
                damage = 0;
            }
            else
                pline("You're covered in frost! You sustain %d damage.", damage == 0 ? 0 : damagedealt);

            display_u_being_hit(HIT_FROZEN, damagedealt, 0UL);

            if ((int)mtmp->m_lev > rn2(20))
            {
                destroy_item(POTION_CLASS, AD_COLD);
                item_destruction_hint(AD_COLD, FALSE);
            }
        }
        //else
        //    damage = 0;

        break;
    case AD_ELEC:
        play_sfx_sound(SFX_MONSTER_GETS_ZAPPED);
        hitmsg(mtmp, mattk, -1, TRUE);
        //if (uncancelled) 
        {
            if (Shock_immunity || damage == 0)
            {
                play_sfx_sound(SFX_GENERAL_UNHARMED);
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "get zapped, but the zap doesn't shock you!");
                //pline_The("zap doesn't shock you!");
                damage = 0;
            }
            else
                You("get zapped for %d damage!", damagedealt);

            display_u_being_hit(HIT_ELECTROCUTED, damagedealt, 0UL);

            if ((int) mtmp->m_lev > rn2(20))
                destroy_item(WAND_CLASS, AD_ELEC);
            if ((int) mtmp->m_lev > rn2(20))
                destroy_item(RING_CLASS, AD_ELEC);

            item_destruction_hint(AD_ELEC, FALSE);
        }
        //else
        //    damage = 0;
        break;
    case AD_SLEE:
        hitmsg(mtmp, mattk, damagedealt, FALSE);
        if (uncancelled) 
        {
            if (Sleep_resistance || check_ability_resistance_success(&youmonst, A_WIS, mattk->mcadj))
                break;
            display_u_being_hit(HIT_SLEEP, damagedealt, 0UL);
            fall_asleep(-rn1(3, 8), TRUE);
            if (Sleeping)
            {
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "feel even sleepier than before!");
            }
            else
            {
                play_sfx_sound(SFX_ACQUIRE_SLEEP);
                if (Blind)
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are put to sleep!");
                else
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are put to sleep by %s!", mon_nam(mtmp));
            }
            set_itimeout(&HSleep_resistance, 20);
            refresh_u_tile_gui_info(TRUE);
        }
        else if(damagedealt > 0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_BLND:
        if (can_blnd(mtmp, &youmonst, mattk->aatyp, (struct obj *) 0))
        {
            if (!Blind)
            {
                play_sfx_sound(SFX_ACQUIRE_BLINDNESS);
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s blinds you!", Monnam(mtmp));
            }
            make_blinded(Blinded + 1 + (long)ceil(damage), FALSE);
            if (!Blind)
                Your1(vision_clears);
        }
        damage = 0;
        break;
    case AD_DRST:
        ptmp = A_STR;
        goto dopois;
    case AD_DRDX:
        ptmp = A_DEX;
        goto dopois;
    case AD_DRCO:
        ptmp = A_CON;
 dopois:
        hitmsg(mtmp, mattk, damagedealt, FALSE);
        if (uncancelled && !rn2(2))
        {
            play_sfx_sound(SFX_MONSTER_IS_POISONED);
            display_u_being_hit(HIT_POISONED, damagedealt, 0UL);
            Sprintf(buf, "%s %s", s_suffix(Monnam(mtmp)),
                    mpoisons_subj(mtmp, mattk));

            poisoned(buf, ptmp, mon_monster_name(mtmp), mtmp->m_lev <= 8 ? 0 : 10, FALSE, mtmp->m_lev <= 2 ? 1 : mtmp->m_lev <= 5 ? 2 : 3);
        }
        else if(damagedealt > 0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_DRIN:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        if (Brain_protection || !has_head(youmonst.data)) {
            play_sfx_sound(SFX_GENERAL_UNHARMED);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "don't seem harmed.");
            /* Not clear what to do for green slimes */
            break;
        }
        if (u_slip_free(mtmp, mattk))
            break;

        if (uarmh && rn2(8)) {
            /* not body_part(HEAD) */
            play_sfx_sound(SFX_HELMET_BLOCKS_ATTACK);
            Your("%s blocks the attack to your head.",
                 helm_simple_name(uarmh));
            break;
        }
        //Cause the damage here rather than below
        mdamageu(mtmp, damage, FALSE);
        damage = 0; /* don't inflict a second dose below */

        if (!uarmh || uarmh->otyp != DUNCE_CAP) 
        {
            /* eat_brains() will miss if target is mindless (won't
               happen here; hero is considered to retain his mind
               regardless of current shape) or is is_incorporeal
               (can't happen here; no one can poly into a ghost
               or shade) so this check for missing is academic */
            if (eat_brains(mtmp, &youmonst, TRUE, (double *) 0) == MM_MISS)
                break;
        }
        /* adjattrib gives dunce cap message when appropriate */
        break;
    case AD_PLYS:
        hitmsg(mtmp, mattk, damagedealt, FALSE);
        if (uncancelled) 
        {
            if (Free_action) 
            {
                if (damagedealt > 0)
                    display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
                if(!HFree_action)
                    You("momentarily stiffen.");
            } 
            else 
            {
                play_sfx_sound(SFX_ACQUIRE_PARALYSIS);
                display_u_being_hit(HIT_PARALYZED, damagedealt, 0UL);
                if (Blind)
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are frozen!");
                else
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are frozen by %s!", mon_nam(mtmp));

                incr_itimeout(&HParalyzed, (rnd(8) + 2));
                context.botl = context.botlx = 1;
                refresh_u_tile_gui_info(TRUE);
                standard_hint("Get free action as early as possible. Avoid engaging in close combat with paralyzing monsters before that.", &u.uhint.paralyzed_by_monster);
                /* No new paralysis for a while */
                set_itimeout(&HFree_action, 20);
                refresh_u_tile_gui_info(TRUE);
            }
        } else if(damagedealt > 0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_SHRP:
        hitmsg(mtmp, mattk, damagedealt, FALSE);
        if (sharpness_effect)
        {
            play_sfx_sound(SFX_SHARPNESS_SLICE);
            display_u_being_hit(HIT_CRITICAL, damagedealt, 0UL);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s strike slices a part of you off!", s_suffix(Monnam(mtmp)));
        } else if (damagedealt > 0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_DRLI:
        hitmsg(mtmp, mattk, damagedealt, FALSE);
        if (uncancelled && !Drain_resistance) //!rn2(3) && 
        {
            play_sfx_sound(SFX_DRAIN_LIFE);
            display_u_being_hit(HIT_DRAIN_LEVEL, damagedealt, 0UL);
            losexp("life drainage");
        }
        else if (damagedealt > 0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_LEGS:
    {
        long side = rn2(2) ? RIGHT_SIDE : LEFT_SIDE;
        const char *sidestr = (side == RIGHT_SIDE) ? "right" : "left",
                   *Monst_name = Monnam(mtmp), *leg = body_part(LEG);

        /* This case is too obvious to ignore, but GnollHack is not in
         * general very good at considering height--most short monsters
         * still _can_ attack you when you're flying or mounted.
         * [FIXME: why can't a flying attacker overcome this?]
         */
        if (u.usteed || Levitation || Flying)
        {
            pline("%s tries to reach your %s %s!", Monst_name, sidestr, leg);
            damage = 0;
        } 
        else if (!uncancelled) 
        {
            pline("%s nuzzles against your %s %s!", Monnam(mtmp),
                  sidestr, leg);
            damage = 0;
        }
        else 
        {
            if (uarmf) 
            {
                if (rn2(2) && (uarmf->otyp == LOW_BOOTS || uarmf->otyp == SHOES)) 
                {
                    pline("%s pricks the exposed part of your %s %s for %d damage!",
                          Monst_name, sidestr, leg, damagedealt);
                }
                else if (!rn2(5))
                {
                    pline("%s pricks through your %s boot for %d damage!", Monst_name,
                          sidestr, damagedealt);
                }
                else
                {
                    pline("%s scratches your %s boot!", Monst_name,
                          sidestr);
                    damage = 0;
                    break;
                }
            } 
            else
                pline("%s pricks your %s %s for %d damage!", Monst_name, sidestr, leg, damagedealt);

            set_wounded_legs(side, rnd(60 - ACURR(A_DEX)));
            display_u_being_hit(HIT_CRITICAL, damagedealt, 0UL);
            exercise(A_STR, FALSE);
            exercise(A_DEX, FALSE);
        }
        break;
    }
    case AD_STON: /* cockatrice */
        hitmsg(mtmp, mattk, damagedealt, FALSE);
        if (!uncancelled) //Needs to bypass MC
        {
            play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_COUGH);
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
            if (!Deaf)
                You_hear("a cough from %s!", mon_nam(mtmp));
        }
        else
        {
            play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_HOWL_IN_ANGER);
            if (!Deaf)
                You_hear("%s hissing!", s_suffix(mon_nam(mtmp)));
 do_stone:
            if (!Stoned && !Stone_resistance
                && !(poly_when_stoned(youmonst.data)
                        && polymon(PM_STONE_GOLEM)))
            {
                display_u_being_hit(HIT_PETRIFIED, 0, 0UL);
                int kformat = KILLED_BY_AN;
                const char *kname = mon_monster_name(mtmp);

                if (mtmp->data->geno & G_UNIQ)
                {
                    if (!is_mname_proper_name(mtmp->data))
                        kname = the(kname);
                    kformat = KILLED_BY;
                }
                make_stoned(5L, (char *) 0, kformat, kname, HINT_KILLED_COCKATRICE);
                return 1;
                /* done_in_by(mtmp, STONING); */
            }
            else if (damagedealt > 0)
                display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        }
        break;
    case AD_STCK:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        if (!is_cancelled(mtmp) && !u.ustuck && !sticks(youmonst.data))
        {
            play_sfx_sound(SFX_ACQUIRE_GRAB);
            u.ustuck = mtmp;
            refresh_m_tile_gui_info(mtmp, FALSE);
            grab_hint(mtmp);
        }
        break;
    case AD_WRAP:
        if ((!is_cancelled(mtmp) || u.ustuck == mtmp) && !sticks(youmonst.data)) 
        {
            if (!u.ustuck && !rn2(2)) 
            {
                if (u_slip_free(mtmp, mattk)) 
                {
                    damage = 0;
                } 
                else 
                {
                    if (damagedealt > 0)
                    {
                        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s swings itself around you. You sustain %d damage!", Monnam(mtmp), damagedealt);
                        display_u_being_hit(HIT_CRUSHED, damagedealt, 0UL);
                    }
                    else
                        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s swings itself around you!", Monnam(mtmp));

                    if (is_pool(mtmp->mx, mtmp->my) && !Swimming && !Amphibious)
                    {
                        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s is drowning you!", Monnam(mtmp));
                        if (Breathless)
                            pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, Magical_breathing ? "However, you can still breathe normally." : "However, you do not feel particularly concerned.");
                    }
                    else if (is_constrictor(mtmp->data))
                    {
                        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s is constricting you to death!", Monnam(mtmp));
                        if (Breathless)
                            pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, Magical_breathing ? "However, you can still breathe normally." : "However, you do not feel particularly concerned.");
                    }

                    play_sfx_sound(SFX_ACQUIRE_GRAB);
                    u.ustuck = mtmp;
                    refresh_m_tile_gui_info(mtmp, FALSE);

                    update_extrinsics();
                    context.botl = context.botlx = 1;

                    grab_hint(mtmp);
                }
            } 
            else if (u.ustuck == mtmp) 
            {
                if (is_pool(mtmp->mx, mtmp->my) && !Swimming && !Amphibious)
                {
                    /* Drowning is now delayed death */
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s is drowning you!", Monnam(mtmp));
                    if (Breathless)
                        pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, Magical_breathing ? "However, you can still breathe normally." : "However, you do not feel particularly concerned.");

                    /*
                    boolean moat = (levl[mtmp->mx][mtmp->my].typ != POOL)
                                   && (levl[mtmp->mx][mtmp->my].typ != WATER)
                                   && !Is_medusa_level(&u.uz)
                                   && !Is_waterlevel(&u.uz);


                    killer.format = KILLED_BY_AN;
                    Sprintf(killer.name, "%s by %s",
                            moat ? "moat" : "pool of water",
                            an(mon_monster_name(mtmp)));
                    done(DROWNING);
                    */
                } 
                else if (is_constrictor(mtmp->data))
                {
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s is constricting you to death!", Monnam(mtmp));
                    if (Breathless)
                        pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, Magical_breathing ? "However, you can still breathe normally." : "However, you do not feel particularly concerned.");
                }
                else if (mattk->aatyp == AT_HUGS)
                {
                    if (damagedealt > 0)
                    {
                        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are being crushed! You sustain %d damage.", damagedealt);
                        display_u_being_hit(HIT_CRUSHED, damagedealt, 0UL);
                    }
                    else
                        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are being crushed%s.", damage == 0 ? ", but sustain no damage" : "");
                }
            } 
            else 
            {
                damage = 0;
                if (flags.verbose)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s brushes against your %s.", Monnam(mtmp),
                          body_part(LEG));
            }
        }
        else
            damage = 0;
        break;
    case AD_WERE:
        hitmsg(mtmp, mattk, damagedealt, FALSE);
        if (uncancelled /* && !rn2(4)*/ && u.ulycn == NON_PM
            && !Protection_from_shape_changers && !Lycanthropy_resistance)
        {
            display_u_being_hit(HIT_WERE, damagedealt, 0UL);
            play_sfx_sound(SFX_CATCH_LYCANTHROPY);
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "feverish.");
            exercise(A_CON, FALSE);
            set_ulycn(mtmp->mnum);
            retouch_equipment(2);
        } else if (damagedealt > 0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_SGLD:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        if (youmonst.data->mlet == mdat->mlet)
            break;
        if (uncancelled)
            stealgold(mtmp);
        break;

    case AD_SSEX:
        if (SYSOPT_SEDUCE) {
            if (could_seduce(mtmp, &youmonst, mattk) == 1 && uncancelled && !resists_charm(&youmonst) && !Charm_resistance && !Mind_shielding)
                if (doseduce(mtmp))
                    return 3;
            break;
        }
        /*FALLTHRU*/
    case AD_SITM: /* for now these are the same */
    case AD_SEDU:
    {
        boolean nymphcancelled = !uncancelled;
        if (is_animal(mtmp->data))
        {
            hitmsg(mtmp, mattk, -1, TRUE);
            if (nymphcancelled)
                break;
            /* Continue below */
        } 
        else if (dmgtype(youmonst.data, AD_SEDU)
                   /* !SYSOPT_SEDUCE: when hero is attacking and AD_SSEX
                      is disabled, it would be changed to another damage
                      type, but when defending, it remains as-is */
                   || dmgtype(youmonst.data, AD_SSEX))
        {
            pline("%s %s.", Monnam(mtmp),
                  Deaf ? "says something but you can't hear it"
                       : mtmp->minvent
                      ? "brags about the goods some dungeon explorer provided"
                  : "makes some remarks about how difficult theft is lately");
            if (!tele_restrict(mtmp))
            {
                (void)rloc2(mtmp, TRUE, TRUE);
            }
            return 3;
        } 
        else if (nymphcancelled || resists_charm(&youmonst) || Charm_resistance || Mind_shielding) 
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s tries to %s you, but you seem %s.",
                      Adjmonnam(mtmp, "plain"),
                      flags.female ? "charm" : "seduce",
                      flags.female ? "unaffected" : "uninterested");
            if (rn2(3)) 
            {
                if (!tele_restrict(mtmp))
                {
                    (void)rloc2(mtmp, TRUE, TRUE);
                }
                return 3;
            }
            break;
        }
        buf[0] = '\0';
        switch (steal(mtmp, buf)) {
        case -1:
            return 2;
        case 0:
            break;
        default:
            if (!is_animal(mtmp->data) && !tele_restrict(mtmp))
            {
                (void)rloc2(mtmp, TRUE, TRUE);
            }
            if (is_animal(mtmp->data) && *buf) {
                if (canseemon(mtmp))
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s tries to %s away with %s.", Monnam(mtmp),
                          locomotion(mtmp->data, "run"), buf);
            }
            monflee(mtmp, 0, FALSE, FALSE);
            return 3;
        }
        break;
    }
    case AD_SAMU:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        /* when the Wizard or quest nemesis hits, there's a 1/20 chance
           to steal a quest artifact (any, not just the one for the hero's
           own role) or the Amulet or one of the invocation tools */
        if (!rn2(20))
            stealamulet(mtmp);
        break;

    case AD_TLPT:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        if (uncancelled) 
        {
            if (flags.verbose)
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "position suddenly seems %suncertain!",
                     (Teleport_control && !Stunned && !unconscious()) ? ""
                     : "very ");
            tele();
            /* 3.6.2:  make sure damage isn't fatal; previously, it
               was possible to be teleported and then drop dead at
               the destination when QM's 1d4 damage gets applied below;
               even though that wasn't "wrong", it seemed strange,
               particularly if the teleportation had been controlled
               [applying the damage first and not teleporting if fatal
               is another alternative but it has its own complications] */
            
            tmphp = (Upolyd ? u.mh : u.uhp);

            if (damage > tmphp)
            {
                damage = (double)(tmphp - 1);
                if (damage < 1)
                { /* implies (tmphp <= 1) */
                    damage = 1;
                    /* this might increase current HP beyond maximum HP but
                       it will be immediately reduced below, so that should
                       be indistinguishable from zero damage; we don't drop
                       damage all the way to zero because that inhibits any
                       passive counterattack if poly'd hero has one */
                    if (Upolyd && u.mh == 1)
                        ++u.mh;
                    else if (!Upolyd && u.uhp == 1)
                        ++u.uhp;
                    /* [don't set context.botl here] */
                }
            }
        }
        break;
    case AD_RUST:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        if (is_cancelled(mtmp))
            break;
        if (is_iron(youmonst.data))
        {
            play_sfx_sound(SFX_YOU_RUST);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "rust!");
            /* KMH -- this is okay with unchanging */
            rehumanize();
            break;
        }
        erode_armor(&youmonst, ERODE_RUST);
        break;
    case AD_CORR:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        if (is_cancelled(mtmp))
            break;
        erode_armor(&youmonst, ERODE_CORRODE);
        break;
    case AD_DCAY:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        if (is_cancelled(mtmp))
            break;
        if (u.umonnum == PM_WOOD_GOLEM || u.umonnum == PM_LEATHER_GOLEM) 
        {
            play_sfx_sound(SFX_YOU_RUST);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "rot!");
            /* KMH -- this is okay with unchanging */
            rehumanize();
            break;
        }
        erode_armor(&youmonst, ERODE_ROT);
        break;
    case AD_HEAL:
        /* a cancelled nurse is just an ordinary monster,
         * nurses don't heal those that cause petrification */
        if (is_cancelled(mtmp) || (Upolyd && touch_petrifies(youmonst.data)))
        {
            hitmsg(mtmp, mattk, damagedealt, FALSE);
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
            break;
        }

        if (!uwep && !uarmu && !uarm && !uarmc && !uarmo && !uarmb
            && !uarms && !uarmg && !uarmf && !uarmh)
        {
            boolean goaway = FALSE;

            int healamount = 0;
            if (Upolyd)
            {
                int starthp = u.mh;
                u.mh += rnd(7);
                if (!rn2(7)) 
                {
                    /* no upper limit necessary; effect is temporary */
                    if(u.basemhdrain < 0)
                        u.basemhdrain++;
                    else
                        u.basemhmax++;
                    if (!rn2(13))
                        goaway = TRUE;
                }
                updatemaxhp();
                healamount = u.mh - starthp;
            } 
            else 
            {
                int starthp = u.uhp;
                u.uhp += rnd(7);
                if (!rn2(7))
                {
                    /* hard upper limit via nurse care: 25 * ulevel */
                    if(u.ubasehpdrain < 0)
                        u.ubasehpdrain++;
                    else if (u.uhpmax < 5 * u.ulevel + d(2 * u.ulevel, 10))
                        u.ubasehpmax++;
                    if (!rn2(13))
                        goaway = TRUE;
                }
                updatemaxhp();
                healamount = u.uhp - starthp;
            }
            play_sfx_sound(SFX_HEALING);
            display_u_being_hit(HIT_HEAL, healamount, 0UL);
            if(healamount > 0)
                pline("%s hits you for %d healing!", Monnam(mtmp), healamount);
            else
                pline("%s hits you but nothing happens.", Monnam(mtmp));

            if (!rn2(3))
                exercise(A_STR, TRUE);
            if (!rn2(3))
                exercise(A_CON, TRUE);
            if (Sick)
                make_sick(0L, (char *) 0, FALSE, 0);
            if (FoodPoisoned)
                make_food_poisoned(0L, (char*)0, FALSE, 0);
            if (MummyRot)
                make_mummy_rotted(0L, (char*)0, FALSE, 0);
            context.botl = context.botlx = TRUE;

            if (goaway)
            {
                mongone(mtmp);
                return 2;
            } 
            else if (!rn2(33))
            {
                if (!tele_restrict(mtmp))
                {
                    (void)rloc2(mtmp, TRUE, TRUE);
                }
                monflee(mtmp, d(3, 6), TRUE, FALSE);
                return 3;
            }
            damage = 0;
        } 
        else 
        {
            if (Role_if(PM_HEALER)) 
            {
                if (!Deaf && !(moves % 5))
                    verbalize("Doc, I can't help you unless you cooperate.");
                damage = 0;
            } 
            else
            {
                hitmsg(mtmp, mattk, damagedealt, FALSE);
                display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
            }
        }
        break;
    case AD_CURS:
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        if (!night() && mdat == &mons[PM_GREMLIN])
            break;
        if (!is_cancelled(mtmp) && !rn2(10)) 
        {
            if (!Deaf) 
            {
                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_LAUGHTER);
                if (Blind)
                    You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "laughter.");
                else
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s chuckles.", Monnam(mtmp));
            }
            if (u.umonnum == PM_CLAY_GOLEM) 
            {
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Some writing vanishes from your head!");
                /* KMH -- this is okay with unchanging */
                rehumanize();
                break;
            }
            attrcurse();
        }
        break;
    case AD_STUN:
        if (uncancelled && !Stun_resistance && !rn2(4)) 
        {
            //damage /= 2;
            //damagedealt = (int)damage + ((damage - (double)((int)damage) - ((double)(Upolyd ? u.mh_fraction : u.uhp_fraction) / 10000)) > 0 ? 1 : 0);
            hitmsg(mtmp, mattk, damagedealt, TRUE);
            if (!Stunned)
                play_sfx_sound(SFX_ACQUIRE_STUN);
            make_stunned((HStun & TIMEOUT) + (long) ceil(damage * 2), TRUE);
        } 
        else
            hitmsg(mtmp, mattk, damagedealt, TRUE);

        break;
    case AD_ACID:
        if (!is_cancelled(mtmp) && !rn2(3))
        {
            play_sfx_sound(SFX_MONSTER_GETS_SPLASHED_BY_ACID);
            if (Acid_immunity)
            {
                play_sfx_sound(SFX_GENERAL_UNHARMED);
                hitmsg(mtmp, mattk, -1, TRUE);
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "You're covered in %s, but it seems harmless.",
                    hliquid("acid"));
                damage = 0;
            }
            else
            {
                hitmsg(mtmp, mattk, damagedealt, TRUE);
                pline("You're covered in %s!  It burns!", hliquid("acid"));
                exercise(A_STR, FALSE);
            }
        }
        else
        {
            hitmsg(mtmp, mattk, -1, TRUE);
            damage = 0;
        }
        break;
    case AD_SLOW:
        hitmsg(mtmp, mattk, damagedealt, FALSE);
        if (uncancelled && ((HSlowed & TIMEOUT) < 300L))
        {
            play_sfx_sound(SFX_ACQUIRE_SLOW);
            display_u_being_hit(HIT_SLOW, damagedealt, 0UL);
            int speed_before = get_u_move_speed(TRUE);
            if ((HSlowed & TIMEOUT) > 100L)
                incr_itimeout(&HSlowed, 1);
            else
                incr_itimeout(&HSlowed, (rnd(10) + 20));
            refresh_u_tile_gui_info(TRUE);
            int speed_after = get_u_move_speed(TRUE);
            if (speed_after < speed_before)
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "slow down.");
            context.botl = context.botlx = 1;
        } 
        else if (damagedealt >0)
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_DREN:
        hitmsg(mtmp, mattk, -1, TRUE);
        if (uncancelled)
            drain_en((int)ceil(damage));
        damage = 0;
        break;
    case AD_CONF:
        hitmsg(mtmp, mattk, -1, TRUE);
        if (!is_cancelled(mtmp) && !mtmp->mspec_used)
        {
            mtmp->mspec_used = mtmp->mspec_used + ((int)ceil(damage) + rn2(6));
            if (Confusion)
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are getting even more confused.");
            else
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are getting confused.");

            if (!Confusion)
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);

            make_confused(itimeout_incr(HConfusion, (int)ceil(damage)), FALSE);
        }
        damage = 0;
        break;
    case AD_DETH:
    {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s reaches out with its deadly touch.", Monnam(mtmp));
        //boolean magic_resistance_success = check_magic_resistance_and_inflict_damage(&youmonst, (struct obj*)0, mtmp->m_lev, 0, 0, NOTELL);
        if (resists_death(&youmonst) || Death_resistance) //  || magic_resistance_success
        {
            /* Still does normal damage */
            pline("Was that the touch of death?");
            break;
        }
        switch (rn2(20)) {
        case 19:
        case 18:
        case 17:
            if (!Death_resistance) // && !check_magic_cancellation_success(&youmonst, mcadj)) 
            {
                display_u_being_hit(HIT_DEATH, damagedealt, 0UL);
                killer.format = KILLED_BY_AN;
                Strcpy(killer.name, "touch of death");
                done(DIED);
                damage = 0;
                break;
            }
            /*FALLTHRU*/
        default: /* case 16: ... case 5: */
            permdmg = 1; /* actual damage done below */
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "your life force draining away... You lose %d hit points, some of them permanently.", damagedealt);
            break;
        case 4:
        case 3:
        case 2:
        case 1:
        case 0:
            play_sfx_sound(SFX_LUCKILY_ITS_TOUCH_DID_NOT_WORK);
            if (Death_resistance)
                u_shieldeff();
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Lucky for you, it didn't work!");
            damage = 0;
            break;
        }
        break;
    }
    case AD_PEST:
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s reaches out, and you feel fever and chills. You lose %d hit points.", Monnam(mtmp), damagedealt);
        if(diseasemu(mtmp)) /* plus the normal damage */
            display_u_being_hit(HIT_SICK, damagedealt, 0UL);
        else
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        break;
    case AD_FAMN:
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s reaches out, and your body shrivels. You lose %d hit points.", Monnam(mtmp), damagedealt);
        exercise(A_CON, FALSE);
        if (!is_fainted())
        {
            int nutrloss = rn1(40, 40);
            morehungry(nutrloss);
            display_famine_floating_text(u.ux, u.uy, nutrloss);
            display_u_being_hit(HIT_FAMINE, damagedealt, 0UL);
        }
        else
            display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
        /* plus the normal damage */
        break;
    case AD_SLIM:
        if (!uncancelled)
            break;
        if (flaming(youmonst.data)) {
            hitmsg(mtmp, mattk, -1, FALSE);
            pline_The_ex(ATR_NONE, CLR_MSG_SUCCESS, "slime burns away!");
            damage = 0;
        } else if (Unchanging || Slime_resistance || slimeproof(youmonst.data)) {
            hitmsg(mtmp, mattk, -1, FALSE);
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "are unaffected.");
            damage = 0;
        } else if (!Slimed) {
            hitmsg(mtmp, mattk, damagedealt, TRUE);
            play_sfx_sound(SFX_START_SLIMING);
            You_ex(ATR_NONE, CLR_MSG_WARNING, "don't feel very well.");
            make_slimed(10L, (char *) 0, KILLED_BY_AN, mon_monster_name(mtmp), HINT_KILLED_SLIMED);
        }
        else {
            hitmsg(mtmp, mattk, damagedealt, TRUE);
            pline("Yuck!");
        }
        break;
    case AD_ENCH: /* KMH -- remove enchantment (disenchanter) */
        hitmsg(mtmp, mattk, damagedealt, TRUE);
        /* uncancelled is sufficient enough; please
           don't make this attack less frequent */
        if (uncancelled) {
            struct obj *obj = some_armor(&youmonst);

            if (!obj) {
                /* some rings are susceptible;
                   amulets and blindfolds aren't (at present) */
                switch (rn2(5)) {
                case 0:
                    break;
                case 1:
                    obj = uright;
                    break;
                case 2:
                    obj = uleft;
                    break;
                case 3:
                    obj = uamul;
                    break;
                case 4:
                    obj = ublindf;
                    break;
                }
            }
            if (drain_item(obj, FALSE)) {
                pline("%s less effective.", Yobjnam2(obj, "seem"));
            }
        }
        break;
    default:
        damage = 0;
        break;
    }

    if ((Upolyd ? u.mh : u.uhp) < 1) {
        /* already dead? call rehumanize() or done_in_by() as appropriate */
        mdamageu(mtmp, 1, FALSE);
        damage = 0;
    }

    int permdmg2 = 0;
    int crit_strike_probability = get_critical_strike_percentage_chance(omonwep, &youmonst, mtmp);
    int crit_strike_die_roll_threshold = crit_strike_probability / 5;

    /* Wounding */
    if (mattk->aatyp == AT_WEAP && omonwep && !uses_spell_flags)
    {
        if (
            (
                (objects[omonwep->otyp].oc_aflags & A1_WOUNDING) && eligible_for_extra_damage(omonwep, &youmonst, mtmp)
                && (
                    ((objects[omonwep->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && (
                            ((objects[omonwep->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && dieroll <= crit_strike_die_roll_threshold)
                            ||
                            (!(objects[omonwep->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && critstrikeroll < crit_strike_probability))
                        )
                    ||
                    (!(objects[omonwep->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && 1)
                    )
              )
           )
        {
            if (objects[omonwep->otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
                extradmg = (int)ceil(damage);

            permdmg2 += extradmg;

        }

        if (has_obj_mythic_wounding(omonwep))
            permdmg2 += mythic_wounding_amount(omonwep);

        if (permdmg2 > 0)
        {
            pline("%s's %s %s you deeply!", Monnam(mtmp), cxname(omonwep), otense(omonwep, "cut"));
        }
    }

    /* Life leech */
    if (mattk->aatyp == AT_WEAP && omonwep && !is_immune_to_life_leech(youmonst.data))
    {
        int life_leech = 0;
        if (!uses_spell_flags && (objects[omonwep->otyp].oc_aflags & A1_LIFE_LEECH) && eligible_for_extra_damage(omonwep, &youmonst, mtmp)
            && (
                ((objects[omonwep->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                    && (
                        ((objects[omonwep->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                            && dieroll <= crit_strike_die_roll_threshold)
                        ||
                        (!(objects[omonwep->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                            && critstrikeroll < crit_strike_probability))
                    )
                ||
                (!(objects[omonwep->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                    && 1)
                )
            )
        {
            if (objects[omonwep->otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
                extradmg = (int)ceil(damage);

            life_leech += extradmg;
        }

        if (has_obj_mythic_life_draining(omonwep))
            life_leech += mythic_life_draining_amount(omonwep);

        if (life_leech > 0)
        {
            int hpbefore = mtmp->mhp;
            deduct_monster_hp(mtmp, (double)-life_leech);

            if (mtmp->mhp > hpbefore)

            if (extradmg > 0)
            {
                play_sfx_sound(SFX_LIFE_LEECH);
                pline("%s's %s %s your life energy!", Monnam(mtmp), cxname(omonwep), otense(omonwep, "leech"));
            }
        }
    }

    int oldumort = u.umortality;

    if (Invulnerable)
    {
        damage = 0;
        permdmg = 0;
        permdmg2 = 0;
    }
    else if (isinstakilled)
    {
        if (Upolyd)
            u.mh = 0;
        else
            u.uhp = 0;
    }

    if (damage > 0 || isinstakilled)
    {
        if (permdmg > 0) 
        { /* Death's life force drain */
            int *hpdrain_p;
            /*
             * Apply some of the damage to permanent hit points:
             *  polymorphed         100% against poly'd hpmax
             *  hpmax > 25*lvl      100% against normal hpmax
             *  hpmax > 10*lvl  50..100%
             *  hpmax >  5*lvl  25..75%
             *  otherwise        0..50%
             * Never reduces hpmax below 1 hit point per level.
             */
            permdmg = rn2((int)ceil(damage / 2) + 1);
            if (Upolyd || u.uhpmax > 25 * u.ulevel)
                permdmg = (int)ceil(damage);
            else if (u.uhpmax > 10 * u.ulevel)
                permdmg += (int)ceil(damage / 2);
            else if (u.uhpmax > 5 * u.ulevel)
                permdmg += (int)ceil(damage / 4);

            if (Upolyd) {
                hpdrain_p = &u.basemhdrain;
            }
            else {
                hpdrain_p = &u.ubasehpdrain;
            }
            *hpdrain_p -= permdmg;

            updatemaxhp();
            if (Upolyd)
            {
                if (u.mh > u.mhmax)
                    u.mh = u.mhmax;
            }
            else
            {
                if (u.uhp > u.uhpmax)
                    u.uhp = u.uhpmax;
            }
            /* else unlikely...
             * already at or below minimum threshold; do nothing */
            context.botl = 1;
        }

        play_monster_weapon_hit_sound(mtmp, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(&youmonst), get_pm_attack_index(mtmp->data, mattk), omonwep, damage, 0);
        mdamageu(mtmp, damage, FALSE);

        if (permdmg2 > 0)
        {
            if (Upolyd)
            {
                u.basemhdrain -= permdmg2;
                updatemaxhp();
                if (u.mh > u.mhmax)
                    u.mh = u.mhmax;
            }
            else
            {
                u.ubasehpdrain -= permdmg2;
                updatemaxhp();
                if (u.uhp > u.uhpmax)
                    u.uhp = u.uhpmax;
            }
            context.botl = 1;
        }

    }

    //Add special enchantments
    if (mattk->aatyp == AT_WEAP && omonwep) 
    {
        if (omonwep->elemental_enchantment > 0)
        {
            char onmbuf[BUFSZ], knmbuf[BUFSZ];

            Strcpy(onmbuf, xname(mweapon));
            Strcpy(knmbuf, killer_xname(mweapon));

            extra_enchantment_damage(onmbuf, omonwep->elemental_enchantment, knmbuf, (u.umortality > oldumort));

            switch (omonwep->elemental_enchantment)
            {
            case COLD_ENCHANTMENT:
                if (is_ammo(omonwep) || throwing_weapon(omonwep) || objects[omonwep->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                    omonwep->elemental_enchantment = 0;
                break;
            case FIRE_ENCHANTMENT:
                if (is_ammo(omonwep) || throwing_weapon(omonwep) || objects[omonwep->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                    omonwep->elemental_enchantment = 0;
                break;
            case LIGHTNING_ENCHANTMENT:
                if(is_ammo(omonwep) || throwing_weapon(omonwep) || objects[omonwep->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                    omonwep->elemental_enchantment = 0;
                break;
            case DEATH_ENCHANTMENT:
                omonwep->elemental_enchantment = 0;
                break;
            }
        }
    }

    if (omonwep && (objectshatters || (!uses_spell_flags 
        && ((objects[omonwep->otyp].oc_aflags & A1_ITEM_VANISHES_ON_HIT)
            && (
                !(objects[omonwep->otyp].oc_aflags & A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET)
                || ((objects[omonwep->otyp].oc_aflags & A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET) && eligible_for_extra_damage(omonwep, &youmonst, mtmp))
                )
            )
        )
    ))
    {
        if(omonwep->where == OBJ_MINVENT)
            m_useup(mtmp, omonwep);
        else if (omonwep->where == OBJ_FLOOR)
        {
            int x = omonwep->ox, y = omonwep->oy;
            delobj(omonwep);
            newsym(x, y);
        }
        else if (omonwep->where == OBJ_FREE)
            obfree(omonwep, (struct obj*)0);
    }

    if (damage > 0)
        res = passiveum(olduasmon, mtmp, mattk);
    else
        res = 1;
    stop_occupation();
    return res;
}

/* An interface for use when taking a blindfold off, for example,
 * to see if an engulfing attack should immediately take affect, like
 * a passive attack. TRUE if engulfing blindness occurred */
boolean
gulp_blnd_check()
{
    struct attack *mattk;

    if (!Blinded && u.uswallow
        && (mattk = attacktype_fordmg(u.ustuck->data, AT_ENGL, AD_BLND))
        && can_blnd(u.ustuck, &youmonst, mattk->aatyp, (struct obj *) 0)) {
        ++u.uswldtim; /* compensate for gulpmu change */
        (void) gulpmu(u.ustuck, mattk);
        return TRUE;
    }
    return FALSE;
}

/* monster swallows you, or damage if u.uswallow */
STATIC_OVL int
gulpmu(mtmp, mattk)
struct monst *mtmp;
struct attack *mattk;
{
    struct trap *t = t_at(u.ux, u.uy);
    double damage = 0;

    int basedmg = 0;
    if(mattk->damn > 0 && mattk->damd > 0)
        basedmg += d((int)mattk->damn, (int)mattk->damd);
    basedmg += mattk->damp;

    damage += adjust_damage(basedmg, mtmp, &youmonst, mattk->adtyp, ADFLAGS_NONE);

    int tim_tmp;
    struct obj *otmp2;
    int i;
    enum hit_tile_types hit_tile = HIT_GENERAL;

    if (!u.uswallow) { /* swallows you */
        int omx = mtmp->mx, omy = mtmp->my;

        if (!engulf_target(mtmp, &youmonst))
            return 0;
        if ((t && is_pit(t->ttyp))
            && sobj_at(BOULDER, u.ux, u.uy))
            return 0;

        if (Punished)
            unplacebc(); /* ball&chain go away */
        remove_monster(omx, omy);
        mtmp->mtrapped = 0; /* no longer on old trap */
        place_monster(mtmp, u.ux, u.uy);
        u.ustuck = mtmp;
        newsym(mtmp->mx, mtmp->my);
        if (is_animal(mtmp->data) && u.usteed) {
            char buf[BUFSZ];

            /* Too many quirks presently if hero and steed
             * are swallowed. Pretend purple worms don't
             * like horses for now :-)
             */
            Strcpy(buf, mon_nam(u.usteed));
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s lunges forward and plucks you off %s!", Monnam(mtmp),
                  buf);
            dismount_steed(DISMOUNT_ENGULFED);
        } else
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s engulfs you!", Monnam(mtmp));
        stop_occupation();
        reset_occupations(); /* behave as if you had moved */

        if (u.utrap) {
            You("are released from the %s!",
                u.utraptype == TT_WEB ? "web" : "trap");
            reset_utrap(FALSE);
        }

        i = number_leashed();
        if (i > 0) {
            const char *s = (i > 1) ? "leashes" : "leash";

            pline_The("%s %s loose.", s, vtense(s, "snap"));
            unleash_all();
        }

        if (touch_petrifies(youmonst.data) && !resists_ston(mtmp)) {
            /* put the attacker back where it started;
               the resulting statue will end up there */
            remove_monster(mtmp->mx, mtmp->my); /* u.ux,u.uy */
            place_monster(mtmp, omx, omy);
            int existing_stoning = get_mon_property(mtmp, STONED);
            (void)set_mon_property_verbosely(mtmp, STONED, max(1, min(existing_stoning - 1, 5)));
            //minstapetrify(mtmp, TRUE);
            /* normally unstuck() would do this, but we're not
               fully swallowed yet so that won't work here */
            //if (Punished)
            //    placebc();
            //u.ustuck = 0;
            //return (!DEADMONSTER(mtmp)) ? 0 : 2;
        }

        display_nhwindow(WIN_MESSAGE, FALSE);
        vision_recalc(2); /* hero can't see anything */
        u.uswallow = 1;
        /* for digestion, shorter time is more dangerous;
           for other swallowings, longer time means more
           chances for the swallower to attack */
        if (mattk->adtyp == AD_DGST) {
            tim_tmp = 25 - (int) mtmp->m_lev;
            if (tim_tmp > 0)
                tim_tmp = rnd(tim_tmp) / 2;
            else if (tim_tmp < 0)
                tim_tmp = -(rnd(-tim_tmp) / 2);
            /* having good armor & high constitution makes
               it take longer for you to be digested, but
               you'll end up trapped inside for longer too */
            tim_tmp += -u.uac + 10 + (ACURR(A_CON) / 3 - 1);
        } else {
            /* higher level attacker takes longer to eject hero */
            tim_tmp = rnd((int) mtmp->m_lev + 10 / 2);
        }
        /* u.uswldtim always set > 1 */
        u.uswldtim = (unsigned) ((tim_tmp < 2) ? 2 : tim_tmp);
        swallowed(1);
        for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
            (void) snuff_lit(otmp2);
    }

    if (mtmp != u.ustuck)
        return 0;
    if (u.uswldtim > 0)
        u.uswldtim -= 1;

    switch (mattk->adtyp) {
    case AD_DGST:
        if (Slow_digestion || Invulnerable) 
        {
            /* Messages are handled below */
            u.uswldtim = 0;
            damage = 0;
        }
        else if (u.uswldtim == 0) 
        {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s totally digests you!", Monnam(mtmp));
            damage = u.uhp;
        } 
        else
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s%s digests you!", Monnam(mtmp),
                  (u.uswldtim == 2) ? " thoroughly"
                                    : (u.uswldtim == 1) ? " utterly" : "");
            exercise(A_STR, FALSE);
            standard_hint("If being digested by a monster, you can use a wand of digging to reduce the monster to one hit point, or wear a ring of slow digestion.", &u.uhint.got_digested);
        }
        break;
    case AD_PHYS:
        if (mtmp->data == &mons[PM_FOG_CLOUD]) 
        {
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are laden with moisture and %s",
                flaming(youmonst.data)
                    ? "are smoldering out!"
                    : Breathless ? "find it mildly uncomfortable."
                                 : amphibious(youmonst.data)
                                       ? "feel comforted."
                                       : "can barely breathe!");
            /* NB: Amphibious includes Breathless */
            if (Amphibious && !flaming(youmonst.data))
                damage = 0;
        } 
        else
        {
            You("are pummeled with debris!");
            exercise(A_STR, FALSE);
        }
        break;
    case AD_ACID:
        hit_tile = HIT_SPLASHED_ACID;
        if (Acid_immunity || Invulnerable) 
        {
            You("are covered with a seemingly harmless goo.");
            damage = 0;
        } else {
            if (Hallucination)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "Ouch!  You've been slimed!");
            else
                You_ex(ATR_NONE, CLR_MSG_WARNING, "are covered in slime!  It burns!");
            exercise(A_STR, FALSE);
        }
        break;
    case AD_BLND:
        if (can_blnd(mtmp, &youmonst, mattk->aatyp, (struct obj *) 0)) 
        {
            if (!Blind) 
            {
                long was_blinded = Blinded;

                if (!Blinded)
                    You_cant_ex(ATR_NONE, CLR_MSG_WARNING, "see in here!");
                make_blinded(1 + (long)ceil(damage), FALSE);
                if (!was_blinded && !Blind)
                    Your1(vision_clears);
            }
            else
            {
                /* keep him blind until disgorged */
                make_blinded(Blinded + 1, FALSE);
            }
        }
        damage = 0;
        break;
    case AD_ELEC:
        if (!is_cancelled(mtmp) && rn2(2)) 
        {
            hit_tile = HIT_ELECTROCUTED;
            pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "air around you crackles with electricity.");
            if (Shock_immunity || Invulnerable) {
                u_shieldeff();
                You("seem unhurt.");
                ugolemeffects(AD_ELEC, damage);
                damage = 0;
            }
        } else
            damage = 0;
        break;
    case AD_COLD:
        if (!is_cancelled(mtmp) && rn2(2)) {
            hit_tile = HIT_FROZEN;
            if (Cold_immunity || Invulnerable) {
                u_shieldeff();
                You_feel("mildly chilly.");
                ugolemeffects(AD_COLD, damage);
                damage = 0;
            } else
                You_ex(ATR_NONE, CLR_MSG_WARNING, "are freezing to death!");
        } else
            damage = 0;
        break;
    case AD_FIRE:
        if (!is_cancelled(mtmp) && rn2(2)) {
            hit_tile = HIT_ON_FIRE;
            if (Fire_immunity || Invulnerable) {
                u_shieldeff();
                You_feel("mildly hot.");
                ugolemeffects(AD_FIRE, damage);
                damage = 0;
            } else
                You_ex(ATR_NONE, CLR_MSG_WARNING, "are burning to a crisp!");
            burn_away_slime();
        } else
            damage = 0;
        break;
    case AD_DISE:
        if (!diseasemu(mtmp))
            damage = 0;
        break;
    case AD_ROTS:
        if (!mummyrotmu(mtmp))
        {
            //damage = 0;
        }
        break;
    case AD_DREN:
        /* AC magic cancellation doesn't help when engulfed */
        if (!is_cancelled(mtmp) && rn2(4)) /* 75% chance */
            drain_en((int)ceil(damage));
        damage = 0;
        break;
    default:
        damage = 0;
        break;
    }

    mdamageu_with_hit_tile(mtmp, damage, TRUE, hit_tile);

    if (damage > 0)
        stop_occupation();

    if (!u.uswallow) {
        ; /* life-saving has already expelled swallowed hero */
    } else if (touch_petrifies(youmonst.data) && !resists_ston(mtmp)) {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s very hurriedly %s you!", Monnam(mtmp),
              is_animal(mtmp->data) ? "regurgitates" : "expels");
        expels(mtmp, mtmp->data, FALSE);
    } else if (!u.uswldtim || youmonst.data->msize >= MZ_HUGE) {
        /* 3.6.2: u.uswldtim used to be set to 0 by life-saving but it
           expels now so the !u.uswldtim case is no longer possible;
           however, polymorphing into a huge form while already
           swallowed is still possible */
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "get %s!", is_animal(mtmp->data) ? "regurgitated" : "expelled");
        if (flags.verbose
            && (is_animal(mtmp->data)
                || (dmgtype(mtmp->data, AD_DGST) && Slow_digestion)))
            pline("Obviously %s doesn't like your taste.", mon_nam(mtmp));
        expels(mtmp, mtmp->data, FALSE);
    }
    return 1;
}

/* monster explodes in your face */
STATIC_OVL int
explmu(mtmp, mattk, ufound)
struct monst *mtmp;
struct attack *mattk;
boolean ufound;
{
    if (!mtmp || !mattk)
        return 0;

    boolean kill_agr = TRUE;

    if (is_cancelled(mtmp))
        return 0;

    enum sfx_sound_types sfx_sound = SFX_ILLEGAL;
    enum special_effect_types spef_idx = MAX_SPECIAL_EFFECTS;
    enum explosion_types expl_idx = MAX_EXPLOSIONS;

    switch (mattk->adtyp)
    {
    case AD_COLD:
        expl_idx = EXPL_FREEZING_SPHERE;
        sfx_sound = explosion_type_definitions[expl_idx].sfx;
        break;
    case AD_FIRE:
        expl_idx = EXPL_FLAMING_SPHERE;
        sfx_sound = explosion_type_definitions[expl_idx].sfx;
        break;
    case AD_ELEC:
        expl_idx = EXPL_SHOCKING_SPHERE;
        sfx_sound = explosion_type_definitions[expl_idx].sfx;
        break;
    case AD_BLND:
        sfx_sound = SFX_BLINDING_FLASH;
        spef_idx = SPECIAL_EFFECT_YELLOW_LIGHT_FLASH;
        break;
    case AD_HALU:
        sfx_sound = SFX_HALLUCINATING_FLASH;
        spef_idx = SPECIAL_EFFECT_BLACK_LIGHT_FLASH;
        break;
    default:
        break;
    }

    if(spef_idx < MAX_SPECIAL_EFFECTS)
        play_special_effect_at(spef_idx, 0, mtmp->mx, mtmp->my, FALSE);
    else if (expl_idx < MAX_EXPLOSIONS)
        play_explosion_animation_at(mtmp->mx, mtmp->my, expl_idx);

    if (sfx_sound != SFX_ILLEGAL)
        play_sfx_sound_at_location(sfx_sound, mtmp->mx, mtmp->my);

    if (spef_idx < MAX_SPECIAL_EFFECTS)
    {
        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        special_effect_wait_until_action(0);
        show_glyph_on_layer(mtmp->mx, mtmp->my, NO_GLYPH, LAYER_MONSTER);
        flush_screen(1);
    }
    else if (expl_idx < MAX_EXPLOSIONS)
    {
        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        explosion_wait_until_action();
        show_glyph_on_layer(mtmp->mx, mtmp->my, NO_GLYPH, LAYER_MONSTER);
        flush_screen(1);
    }

    if (!ufound)
    {
       pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s explodes at a spot in %s!",
              canseemon(mtmp) ? Monnam(mtmp) : "It",
              levl[mtmp->mux][mtmp->muy].typ == WATER ? "empty water"
                                                      : "thin air");
    } 
    else
    {
        double damage = 0;
        int basedmg = 0;
        if(mattk->damn > 0 && mattk->damd > 0)
            basedmg += d((int)mattk->damn, (int)mattk->damd);
        basedmg += (int)mattk->damp;
        damage += adjust_damage(basedmg, mtmp, &youmonst, mattk->adtyp, ADFLAGS_NONE);

        boolean not_affected = 0;

        hitmsg(mtmp, mattk, -1, TRUE);

        switch (mattk->adtyp)
        {
        case AD_COLD:
            not_affected |= Cold_immunity;
            goto common;
        case AD_FIRE:
            not_affected |= Fire_immunity;
            goto common;
        case AD_ELEC:
            not_affected |= Shock_immunity;
            goto common;
        case AD_PHYS:
            /* there aren't any exploding creatures with AT_EXPL attack
               for AD_PHYS damage but there might be someday; without this,
               static analysis complains that 'physical_damage' is always
               False when tested below; it's right, but having that in
               place means one less thing to update if AD_PHYS gets added */
 common:
            if (!not_affected)
            {
                if (ACURR(A_DEX) > rnd(20)) 
                {
                    You("duck some of the blast.");
                    damage = damage / 2;
                } 
                else 
                {
                    if (flags.verbose)
                        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "get blasted!");
                }
                if (mattk->adtyp == AD_FIRE)
                    burn_away_slime();

                mdamageu_with_hit_tile(mtmp, damage, TRUE, get_hit_tile_by_adtyp(mattk->adtyp));

                destroy_item(SCROLL_CLASS, (int)mattk->adtyp);
                destroy_item(SPBOOK_CLASS, (int)mattk->adtyp);
                destroy_item(POTION_CLASS, (int)mattk->adtyp);
                destroy_item(RING_CLASS, (int)mattk->adtyp);
                destroy_item(WAND_CLASS, (int)mattk->adtyp);

                item_destruction_hint((int)mattk->adtyp, FALSE);
            }
            break;

        case AD_BLND:
            not_affected = resists_blnd(&youmonst) || Flash_resistance;
            if (!not_affected)
            {
                /* sometimes you're affected even if it's invisible */
                if (mon_visible(mtmp) || (rnd(basedmg /= 2) > u.ulevel)) 
                {
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are blinded by a blast of light!");
                    make_blinded(1 + (long)basedmg, FALSE);
                    if (!Blind)
                        Your1(vision_clears);
                } 
                else if (flags.verbose)
                    You("get the impression it was not terribly bright.");
            }
            break;

        case AD_HALU:
            not_affected = Blind || Halluc_resistance || (u.umonnum == PM_BLACK_LIGHT
                                      || u.umonnum == PM_VIOLET_FUNGUS
                                      || dmgtype(youmonst.data, AD_STUN));
            if (!not_affected) 
            {
                boolean chg;
                if (!Hallucination)
                    You_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "are caught in a blast of kaleidoscopic light!");
                /* avoid hallucinating the black light as it dies */
                mondead_with_flags(mtmp, MONDIED_FLAGS_NO_DEATH_ACTION);    /* remove it from map now */
                kill_agr = FALSE; /* already killed (maybe lifesaved) */
                chg =
                    make_hallucinated(HHallucination + (long)basedmg, FALSE, 0L);
                if(chg)
                    play_sfx_sound(SFX_ACQUIRE_HALLUCINATION);
                else
                    play_sfx_sound(SFX_GENERAL_UNAFFECTED);

                You_ex(ATR_NONE, chg ? CLR_MSG_NEGATIVE : NO_COLOR,"%s.", chg ? "are freaked out" : "seem unaffected");
            }
            break;

        default:
            break;
        }
        if (not_affected) 
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "seem unaffected by it.");
            ugolemeffects((int) mattk->adtyp, damage);
        }
    }
    if (kill_agr)
        mondead_with_flags(mtmp, MONDIED_FLAGS_NO_DEATH_ACTION);
    wake_nearto(mtmp->mx, mtmp->my, 7 * 7);
    
    if (spef_idx < MAX_SPECIAL_EFFECTS)
    {
        special_effect_wait_until_end(0);
        context.global_newsym_flags = 0UL;
    }
    else if (expl_idx < MAX_EXPLOSIONS)
    {
        explosion_wait_until_end();
        context.global_newsym_flags = 0UL;
    }
    return (!DEADMONSTER(mtmp)) ? 0 : 2;
}

/* monster gazes at you */
int
gazemu(mtmp, mattk)
struct monst *mtmp;
struct attack *mattk;
{
    if (!mtmp || !mattk)
        return 0;

    static const char *const reactions[] = {
        "confused",              /* [0] */
        "stunned",               /* [1] */
        "puzzled",   "dazzled",  /* [2,3] */
        "irritated", "inflamed", /* [4,5] */
        "tired",                 /* [6] */
        "dulled",                /* [7] */
    };
    int react = -1;
    boolean cancelled = is_cancelled(mtmp), already = FALSE;
    
    int range = mattk->range ? mattk->range : M_GENERIC_RANGED_ATTACK_RANGE;
    if (distu(mtmp->mx, mtmp->my) > range * range)
        return 0; /* Out of range */

    if ((mattk->aflags & ATTKFLAG_MUST_SEE_TARGET) && !(m_canseeu(mtmp) && m_cansee(mtmp, u.ux, u.uy)))
        return 0;

    /* assumes that hero has to see monster's gaze in order to be
       affected, rather than monster just having to look at hero;
       when hallucinating, hero's brain doesn't register what
       it's seeing correctly so the gaze is usually ineffective
       [this could be taken a lot farther and select a gaze effect
       appropriate to what's currently being displayed, giving
       ordinary monsters a gaze attack when hero thinks he or she
       is facing a gazing creature, but let's not go that far...] */
    if (Hallucination && rn2(4))
        cancelled = TRUE;

    switch (mattk->adtyp) {
    case AD_STON:
        if (cancelled || is_blinded(mtmp))
        {
            if (!canseemon(mtmp))
                break; /* silently */
            pline("%s %s.", Monnam(mtmp),
                  (mtmp->data == &mons[PM_MEDUSA] && is_cancelled(mtmp))
                      ? "doesn't look all that ugly"
                      : "gazes ineffectually");
            break;
        }
        if (Reflecting && couldsee(mtmp->mx, mtmp->my)
            && mtmp->data == &mons[PM_MEDUSA]) 
        {
            /* hero has line of sight to Medusa and she's not blind */
            boolean useeit = canseemon(mtmp);

            if (useeit)
            {
                play_sfx_sound(SFX_GENERAL_REFLECTS);
                (void)ureflects("%s gaze is reflected by your %s.",
                    s_suffix(Monnam(mtmp)));

            }
            if (mon_reflects(
                mtmp, !useeit ? (char*)0
                : "The gaze is reflected away by %s %s!"))
            {
                play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mtmp->mx, mtmp->my);
                break;
            }

            if (!m_canseeu(mtmp)) { /* probably you're invisible */
                if (useeit)
                    pline(
                      "%s doesn't seem to notice that %s gaze was reflected.",
                          Monnam(mtmp), mhis(mtmp));
                break;
            }
            play_sfx_sound_at_location(SFX_PETRIFY, mtmp->mx, mtmp->my);
            if (useeit)
            {
                display_m_being_hit(mtmp, HIT_PETRIFIED, 0, 0UL, FALSE);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is turned to stone!", Monnam(mtmp));
            }
            stoned = TRUE;
            killed(mtmp);

            if (!DEADMONSTER(mtmp))
                break;
            return 2;
        }
        if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my)
            && !Stone_resistance) {
            You_ex(ATR_NONE, CLR_MSG_WARNING, "meet %s gaze.", s_suffix(mon_nam(mtmp)));
            stop_occupation();
            if (poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM))
                break;
            play_sfx_sound(SFX_PETRIFY);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "turn to stone...");
            killer.hint_idx = HINT_KILLED_MEDUSA_GAZE;
            killer.format = KILLED_BY;
            Strcpy(killer.name, mon_monster_name(mtmp));
            done(STONING);
        }
        break;
    case AD_CONF:
        if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my) && !is_blinded(mtmp)
            && !mtmp->mspec_used && rn2(5)) {
            if (cancelled) {
                react = 0; /* "confused" */
                already = (is_confused(mtmp) != 0);
            } else {
                int conf = d(3, 4);

                mtmp->mspec_used = mtmp->mspec_used + (conf + rn2(6));
                if (!Confusion)
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s gaze confuses you!", s_suffix(Monnam(mtmp)));
                else
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are getting more and more confused.");
                if (!Confusion)
                    play_sfx_sound(SFX_ACQUIRE_CONFUSION);
                make_confused(itimeout_incr(HConfusion, conf), FALSE);
                stop_occupation();
            }
        }
        break;
    case AD_STUN:
        if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my) && !is_blinded(mtmp)
            && !mtmp->mspec_used && rn2(5)) 
        {
            if (cancelled) 
            {
                react = 1; /* "stunned" */
                already = (is_stunned(mtmp) != 0);
            } 
            else
            {
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s stares piercingly at you!", Monnam(mtmp));
                if (!Stun_resistance)
                {
                    int stun = d(2, 6);

                    mtmp->mspec_used = mtmp->mspec_used + (stun + rn2(6));
                    if (!Stunned)
                        play_sfx_sound(SFX_ACQUIRE_STUN);
                    make_stunned((HStun & TIMEOUT) + (long)stun, TRUE);
                    stop_occupation();
                }
                else
                {
                    play_sfx_sound(SFX_GENERAL_RESISTS);
                    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "resist!");
                    u_shieldeff();
                }
            }
        }
        break;
    case AD_BLND:
        if (canseemon(mtmp) && !resists_blnd(&youmonst) && !Flash_resistance
            && distu(mtmp->mx, mtmp->my) <= GAZE_RANGE * GAZE_RANGE)
        {
            if (cancelled) 
            {
                react = rn1(2, 2); /* "puzzled" || "dazzled" */
                already = (mtmp->mprops[BLINDED] != 0);
                /* Archons gaze every round; we don't want cancelled ones
                   giving the "seems puzzled/dazzled" message that often */
                if (is_cancelled(mtmp) && mtmp->data == &mons[PM_ARCHON] && rn2(5))
                    react = -1;
            }
            else 
            {
                int blnd = 0;
                if(mattk->damn > 0 && mattk->damd > 0)
                    blnd += d((int)mattk->damn, (int)mattk->damd);
                blnd += mattk->damp;

                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are blinded by %s radiance!", s_suffix(mon_nam(mtmp)));
                if (!Blind)
                    play_sfx_sound(SFX_ACQUIRE_BLINDNESS);
                make_blinded(1 + (long) blnd, FALSE);
                stop_occupation();
                /* not blind at this point implies you're wearing
                   the Eyes of the Overworld; make them block this
                   particular stun attack too */
                if (!Blind) 
                {
                    pline("But you can still see!");
                    //Your1(vision_clears);
                } 
                else 
                {
                    long oldstun = (HStun & TIMEOUT), newstun = (long) rnd(3);

                    /* we don't want to increment stun duration every time
                       or sighted hero will become incapacitated */
                    make_stunned(max(oldstun, newstun), TRUE);
                }
            }
        }
        break;
    case AD_FIRE:
        if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my) && !is_blinded(mtmp)
            && !mtmp->mspec_used && rn2(5)) 
        {
            if (cancelled) 
            {
                react = rn1(2, 4); /* "irritated" || "inflamed" */
            }
            else
            {
                double damage = adjust_damage(d(2, 6), mtmp, &youmonst, mattk->adtyp, ADFLAGS_NONE);
                int lev = (int)mtmp->m_lev;

                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s attacks you with a fiery gaze!", Monnam(mtmp));
                stop_occupation();
                if (Fire_immunity || Invulnerable) {
                    pline_The("fire doesn't feel hot!");
                    damage = 0;
                }
                burn_away_slime();
                if (lev > rn2(20))
                    destroy_item(SCROLL_CLASS, AD_FIRE);
                if (lev > rn2(20))
                    destroy_item(POTION_CLASS, AD_FIRE);
                if (lev > rn2(25))
                    destroy_item(SPBOOK_CLASS, AD_FIRE);
                if (damage > 0)
                    mdamageu_with_hit_tile(mtmp, damage, TRUE, HIT_ON_FIRE);
                item_destruction_hint(AD_FIRE, FALSE);
            }
        }
        break;
    case AD_CNCL:
        if (cancelled) 
        {
            if (canseemon(mtmp))
                pline("%s gazes at you but without effect.", Monnam(mtmp));
            break;
        }
        else if (is_blinded(mtmp) || !m_canseeu(mtmp))
        {
            if (canseemon(mtmp))
                pline("%s stares blindly at your general direction.", Monnam(mtmp));
            break;
        }
#if 0
        else if (Reflecting)
        {
            if (canseemon(mtmp))
            {
                play_sfx_sound(SFX_GENERAL_REFLECTS);
                (void)ureflects("%s gazes at you, but the gaze is reflected away by your %s.", Monnam(mtmp));
            }
            break;
        }
#endif
        else
        {
            if (canseemon(mtmp))
            {
                if(!Cancelled)
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s gazes at you. You are hit by an invisible anti-magic ray!", Monnam(mtmp));
                else
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s focuses %s anti-magic gaze on you.", Monnam(mtmp), mhis(mtmp));
            }
            if (Cancellation_resistance)
            {
                play_sfx_sound(SFX_GENERAL_UNAFFECTED);
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "However, you are unaffected!");
                u_shieldeff();
            }
            else
            {
                set_itimeout(&HCancelled, max(HCancelled & TIMEOUT, d(2, 4)));
                set_itimeout(&HCancellation_resistance, max(HCancellation_resistance& TIMEOUT, 10));
                refresh_u_tile_gui_info(TRUE);
                context.botl = context.botlx = 1;
            }
        }
        break;
#if 0 /* work in progress */
    case AD_SLEE:
        if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my) && !is_blinded(mtmp)
            && multi >= 0 && !rn2(5) && !Sleep_resistance) {
            if (cancelled) {
                react = 6;                      /* "tired" */
                already = (mtmp->mfrozen != 0); /* can't happen... */
            } else {
                fall_asleep(-rnd(10), TRUE);
                pline("%s gaze makes you very sleepy...",
                      s_suffix(Monnam(mtmp)));
            }
        }
        break;
    case AD_SLOW:
        if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my) && !is_blinded(mtmp)
            && (HFast & (INTRINSIC | TIMEOUT)) && !defends(AD_SLOW, uwep)
            && !rn2(4)) {
            if (cancelled) {
                react = 7; /* "dulled" */
                already = (mtmp->mspeed == MSLOW);
            } else {
                u_slow_down();
                stop_occupation();
            }
        }
        break;
#endif
    default:
        impossible("Gaze attack %d?", mattk->adtyp);
        break;
    }
    if (react >= 0) {
        if (Hallucination && rn2(3))
            react = rn2(SIZE(reactions));
        /* cancelled/hallucinatory feedback; monster might look "confused",
           "stunned",&c but we don't actually set corresponding attribute */
        pline("%s looks %s%s.", Monnam(mtmp),
              !rn2(3) ? "" : already ? "quite "
                                     : (!rn2(2) ? "a bit " : "somewhat "),
              reactions[react]);
    }
    return 0;
}

/* mtmp hits you for n points damage */
void
mdamageu(mtmp, n, verbose)
struct monst* mtmp;
double n;
boolean verbose;
{
    mdamageu_with_hit_tile(mtmp, n, verbose, HIT_GENERAL);
}
    
void
mdamageu_with_hit_tile(mtmp, n, verbose, hit_tile)
struct monst *mtmp;
double n;
boolean verbose;
enum hit_tile_types hit_tile;
{
    int hp_before = Upolyd ? u.mh : u.uhp;
    deduct_player_hp(n);
    int hp_after = Upolyd ? u.mh : u.uhp;
    int damagedealt = hp_before - hp_after;

    if (verbose && damagedealt > 0)
    {
        You("sustain %d damage!", damagedealt);
        display_u_being_hit(hit_tile, damagedealt, 0UL);
    }
    if (Upolyd) {
        if (u.mh < 1)
            rehumanize();
    } else {
        if (u.uhp < 1)
            done_in_by(mtmp, DIED);
    }

}

/* returns 0 if seduction impossible,
 *         1 if fine,
 *         2 if wrong gender for nymph
 */
int
could_seduce(magr, mdef, mattk)
struct monst *magr, *mdef;
struct attack *mattk; /* non-Null: current attack; Null: general capability */
{
    struct permonst *pagr;
    boolean agrinvis, defperc;
    xchar genagr, gendef;
    int adtyp;

    if (is_animal(magr->data))
        return 0;
    if (magr == &youmonst) {
        pagr = youmonst.data;
        agrinvis = (Invis != 0);
        genagr = poly_gender();
    } else {
        pagr = magr->data;
        agrinvis = is_invisible(magr);
        genagr = gender(magr);
    }
    if (mdef == &youmonst) {
        defperc = (See_invisible != 0);
        gendef = poly_gender();
    } else {
        defperc = has_see_invisible(mdef);
        gendef = gender(mdef);
    }

    adtyp = mattk ? mattk->adtyp
            : dmgtype(pagr, AD_SSEX) ? AD_SSEX
              : dmgtype(pagr, AD_SEDU) ? AD_SEDU
                : AD_PHYS;
    if (adtyp == AD_SSEX && !SYSOPT_SEDUCE)
        adtyp = AD_SEDU;

    if (agrinvis && !defperc && adtyp == AD_SEDU)
        return 0;

    if ((pagr->mlet != S_NYMPH
         && pagr != &mons[PM_INCUBUS])
        || (adtyp != AD_SEDU && adtyp != AD_SSEX))
        return 0;

    return (genagr == 1 - gendef) ? 1 : (pagr->mlet == S_NYMPH) ? 2 : 0;
}

/* returns 1 if monster teleported (or hero leaves monster's vicinity) */
int
doseduce(mon)
struct monst *mon;
{
    struct obj *ring, *nring;
    boolean fem = mon->female; // (mon->data == &mons[PM_SUCCUBUS]); /* otherwise incubus */
    boolean seewho, naked; /* True iff no armor */
    int attr_tot, tried_gloves = 0;
    char qbuf[QBUFSZ], Who[QBUFSZ];
    Strcpy(debug_buf_4, "doseduce");

    if (is_cancelled(mon) || mon->mspec_used) {
        pline("%s acts as though %s has got a %sheadache.", Monnam(mon),
              mhe(mon), is_cancelled(mon) ? "severe " : "");
        return 0;
    }
    if (unconscious()) {
        pline("%s seems dismayed at your lack of response.", Monnam(mon));
        return 0;
    }
    seewho = canseemon(mon);
    if (!seewho)
        pline("Someone caresses you...");
    else
        You_feel("very attracted to %s.", mon_nam(mon));
    /* cache the seducer's name in a local buffer */
    Strcpy(Who, (!seewho ? (fem ? "She" : "He") : Monnam(mon)));

    /* if in the process of putting armor on or taking armor off,
       interrupt that activity now */
    (void) stop_donning((struct obj *) 0);
    /* don't try to take off gloves if cursed weapon blocks them */
    if (welded(uwep, &youmonst))
        tried_gloves = 1;

    for (ring = invent; ring; ring = nring) {
        nring = ring->nobj;
        if (ring->otyp != RIN_ADORNMENT)
            continue;
        if (fem) {
            if (ring->owornmask && uarmg) {
                /* don't take off worn ring if gloves are in the way */
                if (!tried_gloves++)
                    mayberem(mon, Who, uarmg, "gloves");
                if (uarmg)
                    continue; /* next ring might not be worn */
            }
            /* confirmation prompt when charisma is high bypassed if deaf */
            if (!Deaf && rn2(20) < ACURR(A_CHA)) {
                (void) safe_qbuf(qbuf, "\"That ",
                                 " looks pretty.  May I have it?\"", ring,
                                 xname, simpleonames, "ring");
                makeknown(RIN_ADORNMENT);
                if (yn_query(qbuf) == 'n')
                    continue;
            } else
                pline("%s decides she'd like %s, and takes it.",
                      Who, yname(ring));
            makeknown(RIN_ADORNMENT);
            /* might be in left or right ring slot or weapon/alt-wep/quiver */
            if (ring->owornmask)
                remove_worn_item(ring, FALSE);
            freeinv(ring);
            (void) mpickobj(mon, ring);
            play_sfx_sound(SFX_STEAL_GOLD);
        } else {
            if (uleft && uright && uleft->otyp == RIN_ADORNMENT
                && uright->otyp == RIN_ADORNMENT)
                break;
            if (ring == uleft || ring == uright)
                continue;
            if (uarmg) {
                /* don't put on ring if gloves are in the way */
                if (!tried_gloves++)
                    mayberem(mon, Who, uarmg, "gloves");
                if (uarmg)
                    break; /* no point trying further rings */
            }
            /* confirmation prompt when charisma is high bypassed if deaf */
            if (!Deaf && rn2(20) < ACURR(A_CHA)) {
                (void) safe_qbuf(qbuf, "\"That ",
                                 " looks pretty.  Would you wear it for me?\"",
                                 ring, xname, simpleonames, "ring");
                makeknown(RIN_ADORNMENT);
                if (yn_query(qbuf) == 'n')
                    continue;
            } else {
                pline("%s decides you'd look prettier wearing %s,",
                      Who, yname(ring));
                pline("and puts it on your finger.");
            }
            makeknown(RIN_ADORNMENT);
            if (!uright) {
                pline("%s puts %s on your right %s.",
                      Who, the(xname(ring)), body_part(HAND));
                setworn(ring, RIGHT_RING);
            } else if (!uleft) {
                pline("%s puts %s on your left %s.",
                      Who, the(xname(ring)), body_part(HAND));
                setworn(ring, LEFT_RING);
            } else if (uright && uright->otyp != RIN_ADORNMENT) {
                /* note: the "replaces" message might be inaccurate if
                   hero's location changes and the process gets interrupted,
                   but trying to figure that out in advance in order to use
                   alternate wording is not worth the effort */
                pline("%s replaces %s with %s.",
                      Who, yname(uright), yname(ring));
                Ring_gone(uright);
                /* ring removal might cause loss of levitation which could
                   drop hero onto trap that transports hero somewhere else */
                if (u.utotype || distu(mon->mx, mon->my) > 2)
                    return 1;
                setworn(ring, RIGHT_RING);
            } else if (uleft && uleft->otyp != RIN_ADORNMENT) {
                /* see "replaces" note above */
                pline("%s replaces %s with %s.",
                      Who, yname(uleft), yname(ring));
                Ring_gone(uleft);
                if (u.utotype || distu(mon->mx, mon->my) > 2)
                    return 1;
                setworn(ring, LEFT_RING);
            } else
                impossible("ring replacement");
            Ring_on(ring);
            prinv((char *) 0, ring, 0L);
        }
    }

    naked = (!uarmc && !uarmf && !uarmg && !uarms && !uarmh && !uarmu && !uarmo && !uarmb);
    pline("%s %s%s.", Who,
          Deaf ? "seems to murmur into your ear"
               : naked ? "murmurs sweet nothings into your ear"
                       : "murmurs in your ear",
          naked ? "" : ", while helping you undress");
    if (uarmc)
        mayberem(mon, Who, uarmc, cloak_simple_name(uarmc));

    if (!uarmc && uarmo)
        mayberem(mon, Who, uarmo, robe_simple_name(uarmo));
    if (!uarmc && !uarmo && uarm)
        mayberem(mon, Who, uarm, suit_simple_name(uarm));
    if (uarmf)
        mayberem(mon, Who, uarmf, "boots");
    if (!tried_gloves && uarmg)
        mayberem(mon, Who, uarmg, "gloves");
    if (uarmb)
        mayberem(mon, Who, uarmb, "bracers");
    if (uarms)
        mayberem(mon, Who, uarms, "shield");
    if (uarmh)
        mayberem(mon, Who, uarmh, helm_simple_name(uarmh));
    if (!uarmc && !uarmo && !uarm && uarmu)
        mayberem(mon, Who, uarmu, "shirt");

    /* removing armor (levitation boots, or levitation ring to make
       room for adornment ring with incubus case) might result in the
       hero falling through a trap door or landing on a teleport trap
       and changing location, so hero might not be adjacent to seducer
       any more (mayberem() has its own adjacency test so we don't need
       to check after each potential removal) */
    if (u.utotype || distu(mon->mx, mon->my) > 2)
        return 1;

    if (uarm || uarmc) {
        if (!Deaf)
            verbalize("You're such a %s; I wish...",
                      flags.female ? "sweet lady" : "nice guy");
        else if (seewho)
            pline("%s appears to sigh.", Monnam(mon));
        /* else no regret message if can't see or hear seducer */

        if (!tele_restrict(mon))
        {
            (void)rloc2(mon, TRUE, TRUE);
        }
        return 1;
    }
    if (u.ualign.type == A_CHAOTIC)
        adjalign(1);

    /* by this point you have discovered mon's identity, blind or not... */
    pline("Time stands still while you and %s lie in each other's arms...",
          noit_mon_nam(mon));
    /* 3.6.1: a combined total for charisma plus intelligence of 35-1
       used to guarantee successful outcome; now total maxes out at 32
       as far as deciding what will happen; chance for bad outcome when
       Cha+Int is 32 or more is 2/35, a bit over 5.7% */
    attr_tot = ACURR(A_CHA) + ACURR(A_INT);
    if (rn2(35) > min(attr_tot, 32)) {
        /* Don't bother with mspec_used here... it didn't get tired! */
        pline("%s seems to have enjoyed it more than you...",
              noit_Monnam(mon));
        switch (rn2(5)) {
        case 0:
            play_sfx_sound(SFX_LOSE_ENERGY);
            You_feel("drained of energy.");
            u.uen = 0;
            u.ubaseendrain -= rnd(Half_physical_damage ? 5 : 10);
            updatemaxen();
            exercise(A_CON, FALSE);
            if (u.uenmax < 0)
                u.uenmax = 0;
            break;
        case 1:
            play_sfx_sound(SFX_LOSE_ABILITY);
            You("are down in the dumps.");
            (void) adjattrib(A_CON, -1, TRUE);
            exercise(A_CON, FALSE);
            context.botl = 1;
            break;
        case 2:
            play_sfx_sound(SFX_LOSE_ABILITY);
            Your("senses are dulled.");
            (void) adjattrib(A_WIS, -1, TRUE);
            exercise(A_WIS, FALSE);
            context.botl = 1;
            break;
        case 3:
            if (!resists_drli(&youmonst)) {
                You_feel("out of shape.");
                losexp("overexertion");
            } else {
                You("have a curious feeling...");
            }
            exercise(A_CON, FALSE);
            exercise(A_DEX, FALSE);
            exercise(A_WIS, FALSE);
            break;
        case 4: {
            int tmp;

            You_feel("exhausted.");
            exercise(A_STR, FALSE);
            tmp = rn1(10, 6);
            losehp(adjust_damage(tmp, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "exhaustion", KILLED_BY);
            break;
        } /* case 4 */
        } /* switch */
    } 
    else 
    {
        mon->mspec_used = rnd(100); /* monster is worn out */
        You("seem to have enjoyed it more than %s...", noit_mon_nam(mon));
        switch (rn2(5)) {
        case 0:
            You_feel("raised to your full potential.");
            exercise(A_CON, TRUE);
            u.ubaseenmax += rnd(5);
            u.uen = u.ubaseenmax;
            updatemaxen();
            break;
        case 1:
            You_feel("good enough to do it again.");
            (void) adjattrib(A_CON, 1, TRUE);
            exercise(A_CON, TRUE);
            context.botl = 1;
            break;
        case 2:
            You("will always remember %s...", noit_mon_nam(mon));
            (void) adjattrib(A_WIS, 1, TRUE);
            exercise(A_WIS, TRUE);
            context.botl = 1;
            break;
        case 3:
            pline("That was a very educational experience.");
            pluslvl(FALSE);
            exercise(A_WIS, TRUE);
            break;
        case 4:
            You_feel("restored to health!");
            u.uhp = u.uhpmax;
            if (Upolyd)
                u.mh = u.mhmax;
            exercise(A_STR, TRUE);
            context.botl = 1;
            break;
        }
    }

    if (is_tame(mon)) { /* don't charge */
        ;
    } else if (rn2(20) < ACURR(A_CHA)) {
        pline("%s demands that you pay %s, but you refuse...",
              noit_Monnam(mon), noit_mhim(mon));
    } else if (u.umonnum == PM_LEPRECHAUN) {
        pline("%s tries to take your money, but fails...", noit_Monnam(mon));
    } else {
        long cost;
        long umoney = money_cnt(invent);

        if (umoney > (long) LARGEST_INT - 10L)
            cost = (long) rnd(LARGEST_INT) + 500L;
        else
            cost = (long) rnd((int) umoney + 10) + 500L;
        if (is_peaceful(mon))
        {
            cost /= 5L;
            if (!cost)
                cost = 1L;
        }
        if (cost > umoney)
            cost = umoney;
        if (!cost) {
            verbalize("It's on the house!");
        } else {
            pline("%s takes %ld %s for services rendered!", noit_Monnam(mon),
                  cost, currency(cost));
            money2mon(mon, cost);
            context.botl = 1;
        }
    }
    if (!rn2(25))
        mon->mspec_used = max(mon->mspec_used, (10 + rnd(40))); // increase_mon_property(mon, CANCELLED, 10 + rnd(40)); /* monster is worn out */

    if (!tele_restrict(mon))
    {
        (void)rloc2(mon, TRUE, TRUE);
    }
    return 1;
}

STATIC_OVL void
mayberem(mon, seducer, obj, str)
struct monst *mon;
const char *seducer; /* only used for alternate message */
struct obj *obj;
const char *str;
{
    char qbuf[QBUFSZ];

    if (!obj || !obj->owornmask)
        return;
    /* removal of a previous item might have sent the hero elsewhere
       (loss of levitation that leads to landing on a transport trap) */
    if (u.utotype || distu(mon->mx, mon->my) > 2)
        return;

    /* being deaf overrides confirmation prompt for high charisma */
    if (Deaf) {
        pline("%s takes off your %s.", seducer, str);
    } else if (rn2(20) < ACURR(A_CHA)) {
        Sprintf(qbuf, "\"Shall I remove your %s, %s?\"", str,
                (!rn2(2) ? "lover" : !rn2(2) ? "dear" : "sweetheart"));
        if (yn_query(qbuf) == 'n')
            return;
    } else {
        char hairbuf[BUFSZ];

        Sprintf(hairbuf, "let me run my fingers through your %s",
                body_part(HAIR));
        verbalize("Take off your %s; %s.", str,
                  (obj == uarm)
                     ? "let's get a little closer"
                     : (obj == uarmc || obj == uarms || obj == uarmo || obj == uarmb)
                        ? "it's in the way"
                        : (obj == uarmf)
                           ? "let me rub your feet"
                           : (obj == uarmg)
                              ? "they're too clumsy"
                              : (obj == uarmu)
                                 ? "let me massage you"
                                 /* obj == uarmh */
                                 : hairbuf);
    }
    remove_worn_item(obj, TRUE);
}

/* FIXME:
 *  sequencing issue:  a monster's attack might cause poly'd hero
 *  to revert to normal form.  The messages for passive counterattack
 *  would look better if they came before reverting form, but we need
 *  to know whether hero reverted in order to decide whether passive
 *  damage applies.
 */
STATIC_OVL int
passiveum(olduasmon, mtmp, mattk)
struct permonst *olduasmon;
struct monst *mtmp;
struct attack *mattk;
{
    if (!mtmp)
        return 0;

    int i;
    double damage = 0;
    struct attack *oldu_mattk = 0;
    int damagedealt = 0;
    enum hit_tile_types hit_tile = HIT_GENERAL;

    /*
     * mattk      == mtmp's attack that hit you;
     * oldu_mattk == your passive counterattack (even if mtmp's attack
     *               has already caused you to revert to normal form).
     */
    for (i = 0; !oldu_mattk; i++) 
    {
        if (i >= NATTK)
            return 1;
        if (olduasmon->mattk[i].aatyp == AT_PASV
            || olduasmon->mattk[i].aatyp == AT_BOOM)
            oldu_mattk = &olduasmon->mattk[i];
    }

    if (oldu_mattk->damd > 0 || oldu_mattk->damn > 0)
        damage = adjust_damage(
            max(0, d(oldu_mattk->damn > 0 ? oldu_mattk->damn : olduasmon->mlevel / 2 + 2, oldu_mattk->damd > 0 ? oldu_mattk->damd : 6) + oldu_mattk->damp), 
            &youmonst, mtmp, mattk->adtyp, ADFLAGS_NONE);
    else
        damage = max(0, oldu_mattk->damp);

    enum action_tile_types action_before = u.action;
    update_u_action(oldu_mattk->action_tile ? oldu_mattk->action_tile : ACTION_TILE_PASSIVE_DEFENSE);
    play_monster_simple_weapon_sound(&youmonst, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
    u_wait_until_action();

    /* These affect the enemy even if you were "killed" (rehumanized) */
    switch (oldu_mattk->adtyp)
    {
    case AD_ACID:
        hit_tile = HIT_SPLASHED_ACID;
        if (!rn2(2))
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is splashed by %s%s!", Monnam(mtmp),
                  /* temporary? hack for sequencing issue:  "your acid"
                     looks strange coming immediately after player has
                     been told that hero has reverted to normal form */
                  !Upolyd ? "" : "your ", hliquid("acid"));
            if (is_mon_immune_to_acid(mtmp)) 
            {
                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is not affected.", Monnam(mtmp));
                damage = 0;
            }
        } 
        else
            damage = 0;

        if (!rn2(30))
            erode_armor(mtmp, ERODE_CORRODE);

        if (!rn2(6))
            acid_damage(MON_WEP(mtmp));

        goto assess_dmg;
    case AD_STON: /* cockatrice */
    {
        long protector = attk_protection((int) mattk->aatyp),
             wornitems = mtmp->worn_item_flags;

        /* wielded weapon gives same protection as gloves here */
        if (MON_WEP(mtmp) != 0)
            wornitems |= W_ARMG;

        if (!resists_ston(mtmp)
            && (protector == 0L
                || (protector != ~0L
                    && (wornitems & protector) != protector))) 
        {
            if (poly_when_stoned(mtmp->data))
            {
                mon_to_stone(mtmp);
                update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
                return 1;
            }
            play_sfx_sound_at_location(SFX_PETRIFY, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s turns to stone!", Monnam(mtmp));
            display_m_being_hit(mtmp, HIT_PETRIFIED, 0, 0UL, FALSE);
            stoned = 1;
            xkilled(mtmp, XKILL_NOMSG);
            update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
            if (!DEADMONSTER(mtmp))
                return 1;
            return 2;
        }
        update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
        return 1;
    }
    case AD_ENCH: /* KMH -- remove enchantment (disenchanter) */
        if (mon_currwep) 
        {
            /* by_you==True: passive counterattack to hero's action
               is hero's fault */
            (void) drain_item(mon_currwep, TRUE);
            /* No message */
        }
        update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
        return 1;
    default:
        break;
    }

    if (!Upolyd)
    {
        update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
        return 1;
    }

    /* These affect the enemy only if you are still a monster */
    if (rn2(3))
        switch (oldu_mattk->adtyp) 
        {
        case AD_PHYS:
            if (oldu_mattk->aatyp == AT_BOOM) 
            {
                You("explode!");
                /* KMH, balance patch -- this is okay with unchanging */
                rehumanize();
                goto assess_dmg;
            }
            break;
        case AD_PLYS: /* Floating eye */
        {
            int paralyse_duration = (int)ceil(damage);
            if (u.umonnum == PM_FLOATING_EYE)
            {
                if (!rn2(20))
                    paralyse_duration = 24;
                if (!is_blinded(mtmp) && haseyes(mtmp->data) && rn2(3)
                    && (has_see_invisible(mtmp) || !Invis))
                {
                    if (Blind)
                        pline_ex(ATR_NONE, CLR_MSG_WARNING, "As a blind %s, you cannot defend yourself.",
                              mon_monster_name(&youmonst));
                    else
                    {
                        update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
                        if (mon_reflects(mtmp, "Your gaze is reflected by %s %s."))
                        {
                            play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mtmp->mx, mtmp->my);
                            return 1;
                        }
                        hit_tile = HIT_PARALYZED;
                        play_sfx_sound_at_location(SFX_ACQUIRE_PARALYSIS, mtmp->mx, mtmp->my);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is frozen by your gaze!", Monnam(mtmp));
                        paralyze_monst(mtmp, paralyse_duration, FALSE);
                        return 3;
                    }
                }
            } 
            else 
            { /* gelatinous cube */
                hit_tile = HIT_PARALYZED;
                pline("%s is frozen by you.", Monnam(mtmp));
                play_sfx_sound_at_location(SFX_ACQUIRE_PARALYSIS, mtmp->mx, mtmp->my);
                paralyze_monst(mtmp, paralyse_duration, FALSE);
                update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
                return 3;
            }
            return 1;
        }
        case AD_COLD: /* Brown mold or blue jelly */
            if (is_mon_immune_to_cold(mtmp)) 
            {
                m_shieldeff(mtmp);
                pline("%s is mildly chilly.", Monnam(mtmp));
                golemeffects(mtmp, AD_COLD, damage);
                damage = 0;
                break;
            }
            hit_tile = HIT_FROZEN;
            play_sfx_sound_at_location(SFX_MONSTER_COVERED_IN_FROST, mtmp->mx, mtmp->my);
            pline("%s is suddenly very cold!", Monnam(mtmp));
            u.mh += (int)damage / 2;
            if (u.mh - u.mhmax > 0)
                u.basemhmax += u.mh - u.mhmax;
            updatemaxhp();
            if (u.mhmax > ((youmonst.data->mlevel + 1) * 8))
                (void) split_mon(&youmonst, mtmp);
            break;
        case AD_STUN: /* Yellow mold */
            if (!is_stunned(mtmp)) 
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_STUN, mtmp->mx, mtmp->my);
                nonadditive_increase_mon_property(mtmp, STUNNED, 5 + rnd(5));
                pline("%s %s.", Monnam(mtmp),
                      makeplural(stagger(mtmp->data, "stagger")));
            }
            damage = 0;
            break;
        case AD_FIRE: /* Red mold */
            if (is_mon_immune_to_fire(mtmp)) 
            {
                m_shieldeff(mtmp);
                if (flaming(youmonst.data))
                    pline("%s is engulfed in your flames, but they do not burn %s.", Monnam(mtmp), mon_nam(mtmp));
                else
                    pline("%s is mildly warm.", Monnam(mtmp));
                golemeffects(mtmp, AD_FIRE, damage);
                damage = 0;
                break;
            }
            hit_tile = HIT_ON_FIRE;
            play_sfx_sound_at_location(SFX_MONSTER_ON_FIRE, mtmp->mx, mtmp->my);
            if (flaming(youmonst.data))
                pline("%s is engulfed in your flames!", Monnam(mtmp));
            else
                pline("%s is suddenly very hot!", Monnam(mtmp));
            break;
        case AD_ELEC:
            if (is_mon_immune_to_elec(mtmp)) 
            {
                m_shieldeff(mtmp);
                pline("%s is slightly tingled.", Monnam(mtmp));
                golemeffects(mtmp, AD_ELEC, damage);
                damage = 0;
                break;
            }
            hit_tile = HIT_ELECTROCUTED;
            play_sfx_sound_at_location(SFX_MONSTER_GETS_ZAPPED, mtmp->mx, mtmp->my);
            pline("%s is jolted with your electricity!", Monnam(mtmp));
            break;
        default:
            damage = 0;
            break;
        }
    else
    damage = 0;

assess_dmg:

    damagedealt = 0;
    int mhp_before = mtmp->mhp;
    deduct_monster_hp(mtmp, damage);
    int mhp_after = mtmp->mhp;
    damagedealt = mhp_before - mhp_after;

    if (canseemon(mtmp) && damagedealt > 0)
    {
        pline("%s sustains %d damage!", Monnam(mtmp), damagedealt);
        display_m_being_hit(mtmp, hit_tile, damagedealt, 0UL, FALSE);
    }

    if (mtmp->mhp <= 0)
    {
        pline("%s dies!", Monnam(mtmp));
        xkilled(mtmp, XKILL_NOMSG);
        update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
        if (!DEADMONSTER(mtmp))
            return 1;
        return 2;
    }
    update_u_action_core(action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
    return 1;
}

struct monst *
cloneu()
{
    struct monst *mon;
    int mndx = monsndx(youmonst.data);

    if (u.mh <= 1)
        return (struct monst *) 0;
    if (mvitals[mndx].mvflags & MV_EXTINCT)
        return (struct monst *) 0;
    mon = makemon(youmonst.data, u.ux, u.uy, MM_NO_MONSTER_INVENTORY | MM_EDOG | MM_SET_ORIGIN_COORDINATES);
    if (!mon)
        return NULL;
    mon->mcloned = 1;
    mon = christen_monst(mon, plname);
    mon->u_know_mname = TRUE;
    initedog(mon, TRUE);
    mon->m_lev = youmonst.data->mlevel;
    //mon might need mbasehpmax stat
    mon->mbasehpmax = u.basemhmax;
    mon->mbasehpdrain = u.basemhdrain;
    update_mon_maxhp(mon);
    mon->mhp = u.mh / 2;
    u.mh -= mon->mhp;
    context.botl = 1;
    return mon;
}

void
update_m_facing(mtmp, mdx, update_symbol)
struct monst* mtmp;
int mdx;
boolean update_symbol;
{
    /* Update facing */
    if (mdx != 0)
    {
        boolean mtmp_facing_before = mtmp->facing_right;
        //boolean u_facing_before = u.facing_right;

        if (mdx < 0)
        {
            mtmp->facing_right = FALSE;
            //u.facing_right = TRUE;
        }
        else if (mdx > 0)
        {
            mtmp->facing_right = TRUE;
            // u.facing_right = FALSE;
        }

        if (update_symbol && mtmp_facing_before != mtmp->facing_right)
            newsym(mtmp->mx, mtmp->my);
        //if (u_facing_before != u.facing_right)
        //    newsym(u.ux, u.uy);

    }
}

int
get_pm_attack_index(ptr, attk)
struct permonst* ptr;
struct attack* attk;
{
    for (int i = 0; i < NATTK; i++)
    {
        if (&ptr->mattk[i] == attk)
            return i;
    }

    return 0; /* return the first attack as a base case to simplify use; one should never get here if properly used */
}

enum hit_tile_types
get_hit_tile_by_adtyp(adtyp)
int adtyp;
{
    enum hit_tile_types hit_tile = HIT_GENERAL;
    switch (adtyp)
    {
    case AD_FIRE:
        hit_tile = HIT_ON_FIRE;
        break;
    case AD_COLD:
        hit_tile = HIT_FROZEN;
        break;
    case AD_ELEC:
        hit_tile = HIT_ELECTROCUTED;
        break;
    case AD_ACID:
        hit_tile = HIT_SPLASHED_ACID;
        break;
    case AD_DETH:
    case AD_DRAY:
        hit_tile = HIT_DEATH;
        break;
    case AD_STON:
        hit_tile = HIT_PETRIFIED;
        break;
    case AD_DISN:
        hit_tile = HIT_DISINTEGRATED;
        break;
    case AD_SLEE:
        hit_tile = HIT_SLEEP;
        break;
    case AD_DRST:
    case AD_DRCO:
    case AD_DRDX:
        hit_tile = HIT_POISONED;
        break;
    case AD_SLIM:
        hit_tile = HIT_SLIMED;
        break;
    case AD_DISE:
    case AD_PEST:
        hit_tile = HIT_SICK;
        break;
    case AD_FAMN:
        hit_tile = HIT_FAMINE;
        break;
    case AD_BLND:
        hit_tile = HIT_FLASHED_LIGHT;
        break;
    case AD_HALU:
        hit_tile = HIT_HALLUCINATED;
        break;
    default:
        break;
    }

    return hit_tile;
}

boolean
should_display_m_action_tile(mtmp, action)
struct monst* mtmp;
enum action_tile_types action;
{
    if (!mtmp)
        return FALSE;

    if((uchar)action == mtmp->action || !canspotmon(mtmp))
        return FALSE;

    if (action == ACTION_TILE_NO_ACTION || action == ACTION_TILE_DEATH)
        return TRUE;

    unsigned long tile_mflag = (1UL << ((unsigned long)action - 1UL));
    boolean has_action_tile = (mtmp->data->mflags5 & tile_mflag) != 0UL;
    return has_action_tile;
}

void
reset_mhitu(VOID_ARGS)
{
    mon_currwep = 0;
}

/*mhitu.c*/
