/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    dokick.c    $NHDT-Date: 1551920353 2019/03/07 00:59:13 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.131 $ */
/* Copyright (c) Izchak Miller, Mike Stephenson, Steve Linhart, 1989. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#define is_bigfoot(x) ((x) == &mons[PM_SASQUATCH])
#define martial()                                 \
    ((martial_bonus() && (Magical_kicking || !(uarmf && is_metallic(uarmf)))) || is_bigfoot(youmonst.data) \
     || Magical_kicking)

static NEARDATA struct rm *maploc, nowhere;
static NEARDATA const char *gate_str;

/* kickedobj (decl.c) tracks a kicked object until placed or destroyed */

extern boolean notonhead; /* for long worms */

STATIC_DCL void FDECL(kickdmg, (struct monst *, BOOLEAN_P));
STATIC_DCL boolean FDECL(maybe_kick_monster, (struct monst *,
                                              XCHAR_P, XCHAR_P));
STATIC_DCL void FDECL(kick_monster, (struct monst *, XCHAR_P, XCHAR_P));
STATIC_DCL int FDECL(really_kick_object, (XCHAR_P, XCHAR_P, BOOLEAN_P));
STATIC_DCL char *FDECL(kickstr, (char *, const char *));
STATIC_DCL void FDECL(otransit_msg, (struct obj *, BOOLEAN_P, long));
STATIC_DCL void FDECL(drop_to, (coord *, SCHAR_P));

static const char kick_passes_thru[] = "kick passes harmlessly through";

/* kicking damage when not poly'd into a form with a kick attack */
STATIC_OVL void
kickdmg(mon, clumsy)
struct monst *mon;
boolean clumsy;
{
    int mdx, mdy;
    int dmg = 0; // (ACURRSTR + ACURR(A_DEX) + ACURR(A_CON)) / 15;
    int specialdmg;
    boolean trapkilled = FALSE;
    boolean martial_arts_applies = martial_bonus() && (Magical_kicking || !(uarmf && is_metallic(uarmf)));
    boolean jumpkicking = !!Jumping;
    boolean effortlessly = FALSE;

    if (!martial_arts_applies && martial_bonus())
    {
        if(!context.bootkickmsgdisplayed)
        {
            Your("boots are hindering the elegance of your kick.");
            context.bootkickmsgdisplayed = TRUE;
        }
    }
    else
    {
        if (context.bootkickmsgdisplayed)
        {
            effortlessly = TRUE;
            context.bootkickmsgdisplayed = FALSE;
        }
    }

    int basedmg = 0;
    int strdmg = 0;
    int extratenths = Magical_kicking ? 2 : 0;
    if (martial_arts_applies)
    {
        basedmg = rnd(8) + 3;
        strdmg = ((10 + extratenths) * u_str_dmg_bonus()) / 10;
        switch (P_SKILL_LEVEL(P_MARTIAL_ARTS))
        {
        case P_BASIC:
            basedmg = d(2, 4) + 3;
            break;
        case P_SKILLED:
            basedmg = d(2, 5) + 3;
            break;
        case P_EXPERT:
            basedmg = d(2, 6) + 3;
            break;
        case P_MASTER:
            basedmg = d(2, 7) + 3;
            break;
        case P_GRAND_MASTER:
            basedmg = d(2, 8) + 3;
            break;
        default:
            break;
        }
    }
    else
    {
        basedmg = rnd(3);
        strdmg = ((5 + extratenths) * u_str_dmg_bonus()) / 10;
    }
    dmg += basedmg + strdmg;

    if (uarmf)
        dmg += weapon_dmg_value(uarmf, mon, &youmonst, 0);
    
    dmg += weapon_skill_dmg_bonus(uarmf, P_MARTIAL_ARTS, FALSE, FALSE, 0);

    /* excessive wt affects dex, so it affects dmg */
    if (clumsy)
        dmg /= 2;

    /* kicking a dragon or an elephant will not harm it, unless you are a martial artist */
    if (thick_skinned(mon->data) && !Magical_kicking && P_SKILL_LEVEL(P_MARTIAL_ARTS) < P_BASIC)
        dmg = 0;

    /* attacking a shade is normally useless */
    if (is_shade(mon->data))
        dmg = 0;

    long silverhit = 0;
    specialdmg = special_dmgval(&youmonst, mon, W_ARMF, &silverhit);
    dmg += specialdmg;

    if (is_shade(mon->data) && !specialdmg) {
        pline_The("%s.", kick_passes_thru);
        /* doesn't exercise skill or abuse alignment or frighten pet,
           and shades have no passive counterattack */
        return;
    }

    dmg += u.ubasedaminc + u.udaminc; /* add ring(s) of increase damage */


    boolean kicksuccessful = FALSE;

    if (!hugemonst(mon->data) && mon != u.ustuck && !mon->mtrapped)
    {
        if (Magical_kicking)
        {
            if (!rn2(2))
                kicksuccessful = TRUE;
        }
        else if (martial_bonus())
        {
            int skilllevel = P_SKILL_LEVEL(P_MARTIAL_ARTS);
            if (!rn2(7 - skilllevel))
                kicksuccessful = TRUE;
        }
    }


    if (kicksuccessful)
        dmg += basedmg / 2;


    char kickstylebuf[BUFSIZ] = "";
    if (jumpkicking)
        strcpy(kickstylebuf, !rn2(2) ? "jump-" : "fly-");
    else if(martial_arts_applies)
    {
        switch (rn2(10))
        {
        case 0:
            strcpy(kickstylebuf, "jump-");
            break;
        case 1:
            strcpy(kickstylebuf, "fly-");
            break;
        case 2:
            strcpy(kickstylebuf, "axe-");
            break;
        case 3:
            strcpy(kickstylebuf, "hook-");
            break;
        case 4:
            strcpy(kickstylebuf, "front-");
            break;
        case 5:
            strcpy(kickstylebuf, "back-");
            break;
        case 6:
            strcpy(kickstylebuf, "twist-");
            break;
        case 7:
            strcpy(kickstylebuf, "spin-");
            break;
        case 8:
            strcpy(kickstylebuf, "side-");
            break;
        case 9:
            strcpy(kickstylebuf, "");
            break;
        default:
            break;
        }
    }

    char effbuf[BUFSIZ] = "";
    if(effortlessly)
        strcpy(effbuf, "effortlessly ");

    if (dmg > 0)
    {
        You("%s%skick %s for %d damage.", effbuf, kickstylebuf, mon_nam(mon), dmg);
    }
    else
    {
        You("%s%skick %s for no damage.", effbuf, kickstylebuf, mon_nam(mon));
    }
    display_m_being_hit(mon, HIT_GENERAL, dmg, 0UL, TRUE);

    if (silverhit)
        pline("Your silver boots sear %s flesh!", s_suffix(mon_nam(mon)));

    if (M_AP_TYPE(mon))
        seemimic(mon);

    check_caitiff(mon);

    /* squeeze some guilt feelings... */
    if (mon->mtame)
    {
        abuse_dog(mon);
        if (is_tame(mon))
            monflee(mon, (dmg ? rnd(dmg) : 1), FALSE, FALSE);
        else
            mon->mflee = 0;
    }

    double damage = adjust_damage(dmg, &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
    if (dmg > 0)
        deduct_monster_hp(mon, damage);

    play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), BAREFOOTED_ATTACK_NUMBER, uarmf, damage, HMON_MELEE);

    boolean hurtles = FALSE;
    boolean reels = FALSE;

    if (kicksuccessful && !DEADMONSTER(mon))
    {
        int skilllevel = P_SKILL_LEVEL(P_MARTIAL_ARTS) + (Jumping ? 1 : 0);
        if (verysmall(mon->data) || mon->data->msize == MZ_SMALL)
            hurtles = TRUE;
        else if (!bigmonst(mon->data))
        {
            if ((skilllevel == P_BASIC && (rn2(2) || Magical_kicking)) || skilllevel >= P_SKILLED)
                hurtles = TRUE;
            else
                reels = TRUE;
        }
        else if (!hugemonst(mon->data))
        {
            if (skilllevel >= P_MASTER)
                hurtles = TRUE;
            else if(skilllevel >= P_EXPERT && (rn2(2) || Magical_kicking))
                hurtles = TRUE;
            else if (skilllevel == P_SKILLED && Magical_kicking && rn2(2))
                hurtles = TRUE;
            else if (skilllevel >= P_SKILLED)
                reels = TRUE;
        }
    }

    if(hurtles)
    {
        pline("%s hurtles backwards from the force of your kick!", Monnam(mon));
        mhurtle(mon, u.dx, u.dy, 1);
    }
    else if (reels) 
    {
        /* see if the monster has a place to move into */
        mdx = mon->mx + u.dx;
        mdy = mon->my + u.dy;
        if (goodpos(mdx, mdy, mon, 0)) 
        {
            pline("%s reels from the blow.", Monnam(mon));
            if (m_in_out_region(mon, mdx, mdy)) {
                remove_monster(mon->mx, mon->my);
                newsym(mon->mx, mon->my);
                place_monster(mon, mdx, mdy);
                newsym(mon->mx, mon->my);
                set_apparxy(mon);
                if (mintrap(mon) == 2)
                    trapkilled = TRUE;
            }
        }
    }

    (void) passive(mon, uarmf, TRUE, !DEADMONSTER(mon), AT_KICK, FALSE);
    if (DEADMONSTER(mon) && !trapkilled)
        killed(mon);

    use_skill(P_MARTIAL_ARTS, 1);
}

STATIC_OVL boolean
maybe_kick_monster(mon, x, y)
struct monst *mon;
xchar x, y;
{
    if (mon) {
        boolean save_forcefight = context.forcefight;

        bhitpos.x = x;
        bhitpos.y = y;
        if (!is_peaceful(mon) || !canspotmon(mon))
            context.forcefight = TRUE; /* attack even if invisible */
        /* kicking might be halted by discovery of hidden monster,
           by player declining to attack peaceful monster,
           or by passing out due to encumbrance */
        if (attack_checks(mon, (struct obj *) 0) || overexertion())
            mon = 0; /* don't kick after all */
        context.forcefight = save_forcefight;
    }
    return (boolean) (mon != 0);
}

STATIC_OVL void
kick_monster(mon, x, y)
struct monst *mon;
xchar x, y;
{
    boolean clumsy = FALSE;

    /* anger target even if wild miss will occur */
    setmangry(mon, TRUE);

    if (Levitation && !rn2(3) && verysmall(mon->data)
        && !is_flying(mon)) {
        pline("Floating in the air, you miss wildly!");
        exercise(A_DEX, FALSE);
        (void) passive(mon, uarmf, FALSE, 1, AT_KICK, FALSE);
        return;
    }

    /* reveal hidden target even if kick ends up missing (note: being
       hidden doesn't affect chance to hit so neither does this reveal) */
    if (mon->mundetected
        || (M_AP_TYPE(mon) && M_AP_TYPE(mon) != M_AP_MONSTER)) 
    {
        if (M_AP_TYPE(mon))
            seemimic(mon);
        mon->mundetected = 0;
        if (!canspotmon(mon))
            map_invisible(x, y);
        else
            newsym(x, y);
        There("is %s here.",
              canspotmon(mon) ? a_monnam(mon) : "something hidden");
    }

    /* Kick attacks by kicking monsters are normal attacks, not special.
     * This is almost always worthless, since you can either take one turn
     * and do all your kicks, or else take one turn and attack the monster
     * normally, getting all your attacks _including_ all your kicks.
     * If you have >1 kick attack, you get all of them.
     */
    if (Upolyd && attacktype(youmonst.data, AT_KICK)) 
    {
        struct attack *uattk;
        int sum, kickdieroll, armorpenalty, specialdmg,
            attknum = 0,
            tmp = find_roll_to_hit(mon, AT_KICK, uarmf, &attknum, &armorpenalty);

        for (int i = 0; i < NATTK; i++)
        {
            /* first of two kicks might have provoked counterattack
               that has incapacitated the hero (ie, floating eye) */
            if (multi < 0)
                break;

            uattk = &youmonst.data->mattk[i];
            /* we only care about kicking attacks here */
            if (uattk->aatyp != AT_KICK)
                continue;

            update_u_action(ACTION_TILE_KICK);
            play_monster_simple_weapon_sound(&youmonst, BAREFOOTED_ATTACK_NUMBER, uarmf, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();

            kickdieroll = rnd(20);
            specialdmg = special_dmgval(&youmonst, mon, W_ARMF, (long *) 0);
            if (is_shade(mon->data) && !specialdmg) {
                /* doesn't matter whether it would have hit or missed,
                   and shades have no passive counterattack */
                Your("%s %s.", kick_passes_thru, mon_nam(mon));
                break; /* skip any additional kicks */
            } 
            else if (tmp > kickdieroll) 
            {
                You("kick %s.", mon_nam(mon));
                sum = damageum(mon, uattk, uarmf, specialdmg);
                (void) passive(mon, uarmf, (boolean) (sum > 0),
                               (sum != 2), AT_KICK, FALSE);
                if (sum == 2)
                {
                    update_u_action_revert(ACTION_TILE_NO_ACTION);
                    break; /* Defender died */
                }
            } 
            else 
            {
                missum(mon, uattk, (tmp + armorpenalty > kickdieroll));
                (void) passive(mon, uarmf, FALSE, 1, AT_KICK, FALSE);
            }
            update_u_action_revert(ACTION_TILE_NO_ACTION);

        }
        return;
    }

    int multistrike = get_multishot_stats(&youmonst, (struct obj*)0, (struct obj*)0, FALSE, (double*)0);

    for (int strikeindex = 0; strikeindex < multistrike; strikeindex++)
    {
        update_u_action(ACTION_TILE_KICK);
        play_monster_simple_weapon_sound(&youmonst, BAREFOOTED_ATTACK_NUMBER, uarmf, OBJECT_SOUND_TYPE_SWING_MELEE);
        u_wait_until_action();

        char strikebuf[BUFSIZ] = "";
        Sprintf(strikebuf, "You attack");

        if (strikeindex > 0)
            pline("%s %s!", strikebuf, strikeindex == 1 ? "a second time" : strikeindex == 2 ? "a third time" : "once more");


        int armorpenalty = 0, attknum = 0;
        int tmp = find_roll_to_hit(mon, AT_KICK, uarmf, &attknum, &armorpenalty);
        int weight_penalty = -calc_capacity(0);
        int base_penalty = -2;
        tmp += weight_penalty + base_penalty;

        if (Fumbling)
            clumsy = TRUE;
        else if (uarm && objects[uarm->otyp].oc_bulky && ACURR(A_DEX) < rnd(25))
            clumsy = TRUE;

        if (clumsy)
            tmp -= 4;

        int dieroll = rnd(20);
        boolean mhit = !!(tmp > dieroll || u.uswallow);

        if (mhit)
        {
            /* check if mon catches your kick */
            if (!rn2(clumsy ? 3 : 4) && (clumsy || !bigmonst(mon->data))
                && !is_blinded(mon) && !mon->mtrapped && !thick_skinned(mon->data)
                && mon->data->mlet != S_EEL && haseyes(mon->data)
                && !is_stunned(mon) && !is_confused(mon) && mon_can_move(mon)
                && mon->data->mmove >= 12)
            {
                if (!nohands(mon->data) && !rn2(martial() ? 5 : 3))
                {
                    You("try to kick %s.", mon_nam(mon));
                    pline("However, %s blocks your %skick.", mon_nam(mon),
                        clumsy ? "clumsy " : "");
                    (void)passive(mon, uarmf, FALSE, 1, AT_KICK, FALSE);
                    update_u_action_revert(ACTION_TILE_NO_ACTION);
                    return;
                }
                else
                {
                    maybe_mnexto(mon);
                    if (mon->mx != x || mon->my != y)
                    {
                        You("kick %s.", mon_nam(mon));
                        (void)unmap_invisible(x, y);
                        pline("%s %s, %s evading your %skick.", Monnam(mon),
                            (!level.flags.noteleport && has_teleportation(mon))
                            ? "teleports"
                            : is_levitating(mon)
                            ? "floats"
                            : is_flying(mon) ? "swoops"
                            : (nolimbs(mon->data)
                                || slithy(mon->data))
                            ? "slides"
                            : "jumps",
                            clumsy ? "easily" : "nimbly", clumsy ? "clumsy " : "");
                        (void)passive(mon, uarmf, FALSE, 1, AT_KICK, FALSE);
                        update_u_action_revert(ACTION_TILE_NO_ACTION);
                        return;
                    }
                }
            }

            /* now, do the damage */
            kickdmg(mon, clumsy);
        }
        else
        {
            Your("kick misses %s.", mon_nam(mon));
        }

        update_u_action_revert(ACTION_TILE_NO_ACTION);
        
        if (!mon || DEADMONSTER(mon) || m_at(x, y) != mon)
            break;

    }
}

/*
 *  Return TRUE if caught (the gold taken care of), FALSE otherwise.
 *  The gold object is *not* attached to the fobj chain!
 */
boolean
ghitm(mtmp, gold, hitres_ptr)
register struct monst *mtmp;
register struct obj *gold;
uchar* hitres_ptr;
{
    boolean msg_given = FALSE;

    if (!likes_gold(mtmp->data) && !mtmp->isshk && !mtmp->ispriest && !mtmp->issmith && !mtmp->isnpc
        && !mtmp->isgd && !is_mercenary(mtmp->data)) 
    {
        wakeup(mtmp, TRUE);
    }
    else if (!mon_can_move(mtmp))
    {
        /* too light to do real damage */
        if (canseemon(mtmp))
        {
            pline_The("%s harmlessly %s %s.", xname(gold),
                      otense(gold, "hit"), mon_nam(mtmp));
            msg_given = TRUE;
        }
    } 
    else 
    {
        if (hitres_ptr)
            *hitres_ptr = 1;

        long umoney, value = gold->quan * objects[gold->otyp].oc_cost;

        mtmp->msleeping = 0;
        finish_meating(mtmp);
        refresh_m_tile_gui_info(mtmp, TRUE);
        if (!mtmp->isgd && !rn2(4)) /* not always pleasing */
            setmangry(mtmp, TRUE);
        /* greedy monsters catch gold */
        if (cansee(mtmp->mx, mtmp->my))
            pline("%s catches the gold.", Monnam(mtmp));
        (void) mpickobj(mtmp, gold);
        gold = (struct obj *) 0; /* obj has been freed */
        if (mtmp->isshk)
        {
            long robbed = ESHK(mtmp)->robbed;

            if (robbed) 
            {
                robbed -= value;
                if (robbed < 0L)
                    robbed = 0L;
                pline_The("amount %scovers %s recent losses.",
                          !robbed ? "" : "partially ", mhis(mtmp));
                ESHK(mtmp)->robbed = robbed;
                if (!robbed)
                    make_happy_shk(mtmp, FALSE);
            } 
            else 
            {
                if (is_peaceful(mtmp)) 
                {
                    ESHK(mtmp)->credit += value;
                    You("have %ld %s in credit.", ESHK(mtmp)->credit,
                        currency(ESHK(mtmp)->credit));
                }
                else
                {
                    play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_THANK_YOU_SCUM);
                    verbalize("Thank you, scum!");
                }
            }
        } 
        else if (mtmp->ispriest) 
        {
            if (is_peaceful(mtmp))
            {
                play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_THANK_YOU_FOR_YOUR_CONTRIBUTION);
                verbalize("Thank you for your contribution.");
            }
            else
            {
                play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_THANKS_SCUM);
                verbalize("Thanks, scum!");
            }
        }
        else if (mtmp->issmith || mtmp->isnpc) 
        {
            if (is_peaceful(mtmp))
            {
                if (mtmp->issmith)
                    play_monster_special_dialogue_line(mtmp, SMITH_LINE_THANK_YOU_FOR_YOUR_ASSISTANCE);
                else
                    play_monster_special_dialogue_line(mtmp, NPC_LINE_THANK_YOU_FOR_YOUR_ASSISTANCE);

                verbalize("Thank you for your assistance.");
            }
            else
            {
                if(mtmp->issmith)
                    play_monster_special_dialogue_line(mtmp, SMITH_LINE_THANKS_SCUM);
                else
                    play_monster_special_dialogue_line(mtmp, NPC_LINE_THANKS_SCUM);
                verbalize("Thanks, scum!");
            }
        } 
        else if (mtmp->isgd) 
        {
            umoney = money_cnt(invent);
            /* Some of these are iffy, because a hostile guard
               won't become peaceful and resume leading hero
               out of the vault.  If he did do that, player
               could try fighting, then weasle out of being
               killed by throwing his/her gold when losing. */
            play_monster_special_dialogue_line(mtmp, umoney
                ? VAULT_GUARD_LINE_DROP_THE_REST_AND_FOLLOW_ME
                : hidden_gold()
                ? VAULT_GUARD_LINE_YOU_STILL_HAVE_HIDDEN_GOLD_DROP_IT_NOW
                : is_peaceful(mtmp)
                ? VAULT_GUARD_LINE_ILL_TAKE_CARE_OF_THAT_PLEASE_MOVE_ALONG
                : VAULT_GUARD_LINE_ILL_TAKE_THAT_NOW_GET_MOVING);

            verbalize(umoney
                    ? "Drop the rest and follow me."
                    : hidden_gold()
                          ? "You still have hidden gold.  Drop it now."
                          : is_peaceful(mtmp)
                                ? "I'll take care of that; please move along."
                                : "I'll take that; now get moving.");
        } 
        else if (is_mercenary(mtmp->data))
        {
            long goldreqd = 0L;

            if (rn2(3)) {
                if (mtmp->data == &mons[PM_SOLDIER])
                    goldreqd = 100L;
                else if (mtmp->data == &mons[PM_SERGEANT])
                    goldreqd = 250L;
                else if (mtmp->data == &mons[PM_LIEUTENANT])
                    goldreqd = 500L;
                else if (mtmp->data == &mons[PM_CAPTAIN])
                    goldreqd = 750L;

                if (goldreqd) 
                {  
                    umoney = money_cnt(invent);
                    if (value > goldreqd + (umoney + u.ulevel * rn2(5)) / ACURR(A_CHA))
                    {
                        mtmp->mpeaceful = TRUE;
                        newsym(mtmp->mx, mtmp->my);
                    }
                }
            }
            if (is_peaceful(mtmp))
            {
                verbalize("That should do.  Now beat it!");
            }
            else
            {
                verbalize("That's not enough, coward!");
            }
        }
        return TRUE;
    }

    if (!msg_given)
        miss(xname(gold), mtmp);
    return FALSE;
}

/* container is kicked, dropped, thrown or otherwise impacted by player.
 * Assumes container is on floor.  Checks contents for possible damage. */
void
container_impact_dmg(obj, x, y)
struct obj *obj;
xchar x, y; /* coordinates where object was before the impact, not after */
{
    struct monst *shkp;
    struct obj *otmp, *otmp2;
    long loss = 0L;
    boolean costly, insider, frominv;

    /* only consider normal containers */
    if (!Is_container(obj) || !Has_contents(obj) || Is_mbag(obj))
        return;

    costly = ((shkp = shop_keeper(*in_rooms(x, y, SHOPBASE)))
              && costly_spot(x, y));
    insider = (*u.ushops && inside_shop(u.ux, u.uy)
               && *in_rooms(x, y, SHOPBASE) == *u.ushops);
    /* if dropped or thrown, shop ownership flags are set on this obj */
    frominv = (obj != kickedobj);

    for (otmp = obj->cobj; otmp; otmp = otmp2) {
        const char *result = (char *) 0;

        otmp2 = otmp->nobj;
        if (is_fragile(otmp)
            && otmp->oclass != GEM_CLASS && !obj_resists(otmp, 33, 100)) {
            result = "shatter";
            play_sfx_sound_at_location(SFX_MUFFLED_SHATTER, x, y);
        } else if (otmp->otyp == EGG && !rn2(3)) {
            result = "cracking";
            play_sfx_sound_at_location(SFX_MUFFLED_CRACKING, x, y);
        }
        if (result) {
            You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a muffled %s.", result);
            
            if (otmp->otyp == MIRROR || otmp->otyp == MAGIC_MIRROR)
                change_luck(-2, TRUE);
            else if (otmp->otyp == EGG && (otmp->speflags & SPEFLAGS_YOURS) && otmp->corpsenm >= LOW_PM)
                change_luck(-1, TRUE);
            /* eggs laid by you.  penalty is -1 per egg, max 5,
             * but it's always exactly 1 that breaks */

            if (costly) {
                if (frominv && !otmp->unpaid)
                    otmp->no_charge = 1;
                loss +=
                    stolen_value(otmp, x, y, is_peaceful(shkp), TRUE);
            }
            if (otmp->quan > 1L) {
                useup(otmp);
            } else {
                obj_extract_self(otmp);
                obfree(otmp, (struct obj *) 0);
            }
            /* contents of this container are no longer known */
            obj->cknown = 0;
        }
    }
    if (costly && loss) {
        if (!insider) {
            You("caused %ld %s worth of damage!", loss, currency(loss));
            make_angry_shk(shkp, x, y);
        } else {
            You("owe %s %ld %s for objects destroyed.", mon_nam(shkp), loss,
                currency(loss));
        }
    }
}

/* jacket around really_kick_object */

int
kick_object(x, y, kickobjnam, is_golf_swing)
xchar x, y;
char *kickobjnam;
boolean is_golf_swing;
{
    int res = 0;

    *kickobjnam = '\0';
    /* if a pile, the "top" object gets kicked */
    kickedobj = level.objects[x][y];
    if (kickedobj) {
        /* kick object; if doing is fatal, done() will clean up kickedobj */
        Strcpy(kickobjnam, killer_xname(kickedobj)); /* matters iff res==0 */
        res = really_kick_object(x, y, is_golf_swing);
        kickedobj = (struct obj *) 0;
    }
    return res;
}

/* guts of kick_object */
STATIC_OVL int
really_kick_object(x, y, is_golf_swing)
xchar x, y;
boolean is_golf_swing;
{
    int range;
    struct monst *mon, *shkp = 0;
    struct trap *trap;
    char bhitroom;
    boolean costly, isgold, slide = FALSE;
    const char* verb = is_golf_swing ? "swing at" : "kick";
    const char* noun = is_golf_swing ? "swing" : "kick";

    /* kickedobj should always be set due to conditions of call */
    if (!kickedobj || kickedobj->otyp == BOULDER || kickedobj == uball
        || kickedobj == uchain)
        return 0;

    if ((trap = t_at(x, y)) != 0) 
    {
        if ((is_pit(trap->ttyp) && !Passes_walls) || trap->ttyp == WEB) 
        {
            if (!trap->tseen)
                find_trap(trap);

            play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s that's in a %s!", 
                verb,
                something,
                Hallucination ? "tizzy" : (trap->ttyp == WEB) ? "web" : "pit");
            return 1;
        }
        if (trap->ttyp == STATUE_TRAP) 
        {
            activate_statue_trap(trap, x,y, FALSE);
            return 1;
        }
    }

    if (Fumbling && !rn2(3)) 
    {
        Your("clumsy %s missed.", noun);
        return 1;
    }

    if (!uarmf && kickedobj->otyp == CORPSE
        && touch_petrifies(&mons[kickedobj->corpsenm]) && !Stone_resistance) 
    {
        if (is_golf_swing)
        {
            play_sfx_sound(SFX_THUMP_HIT);
            pline("Thump!");
            return 1;
        }
        else
        {
            You("kick %s with your bare %s.",
                corpse_xname(kickedobj, (const char*)0, CXN_PFX_THE),
                makeplural(body_part(FOOT)));
            if (poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM))
            {
                ; /* hero has been transformed but kick continues */
            }
            else 
            {
                /* normalize body shape here; foot, not body_part(FOOT) */
                Sprintf(killer.name, "kicking %s barefoot",
                    killer_xname(kickedobj));
                killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
                instapetrify(killer.name);
            }
        }
    }

    isgold = (kickedobj->oclass == COIN_CLASS);

    int k_owt = (int) kickedobj->owt;

    /* for non-gold stack, 1 item will be split off below (unless an
        early return occurs, so we aren't moving the split to here);
        calculate the range for that 1 rather than for the whole stack */
    if (kickedobj->quan > 1L && !isgold)
    {
        long save_quan = kickedobj->quan;

        kickedobj->quan = 1L;
        k_owt = weight(kickedobj);
        kickedobj->quan = save_quan;
    }

    /* range < 2 means the object will not move
        (maybe dexterity should also figure here) */
    range = is_golf_swing ? ( ((int)ACURRSTR + ACURR(A_DEX) + (kickedobj->oclass == GEM_CLASS ? (throwing_weapon(kickedobj) ? 18 : 6) : 0)) / max(1, k_owt / 4) ) : ((int) ACURRSTR) / 2 - k_owt / 40;

    if (martial() && !is_golf_swing)
        range += rnd(3);

    if (is_pool(x, y)) 
    {
        /* you're in the water too; significantly reduce range */
        range = range / 3 + 1; /* {1,2}=>1, {3,4,5}=>2, {6,7,8}=>3 */
    } 
    else if (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)) 
    {
        /* you're in air, since is_pool did not match */
        range += rnd(3);
    } 
    else 
    {
        if (is_ice(x, y))
            range += rnd(3), slide = TRUE;
        if (kickedobj->greased)
            range += rnd(3), slide = TRUE;
    }

    /* Mjollnir is magically too heavy to kick */
//    if (kickedobj->oartifact == ART_MJOLLNIR)
//        range = 1;

    /* see if the object has a place to move into */
    if (!ZAP_POS(levl[x + u.dx][y + u.dy].typ)
        || closed_door(x + u.dx, y + u.dy))
        range = 1;

    costly = (!(kickedobj->no_charge && !Has_contents(kickedobj))
              && (shkp = shop_keeper(*in_rooms(x, y, SHOPBASE))) != 0
              && costly_spot(x, y));

    if (IS_ROCK(levl[x][y].typ) || closed_door(x, y)) 
    {
        if ((!martial() && rn2(20) > ACURR(A_DEX))
            || IS_ROCK(levl[u.ux][u.uy].typ) || closed_door(u.ux, u.uy)) 
        {
            if (Blind)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "It doesn't come loose.");
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %sn't come loose.",
                      The(distant_name(kickedobj, xname)),
                      otense(kickedobj, "do"));
            return (!rn2(3) || martial());
        }
        if (Blind)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "It comes loose.");
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s loose.", The(distant_name(kickedobj, xname)),
                  otense(kickedobj, "come"));
        kickedobj->speflags &= ~SPEFLAGS_CAUGHT_IN_LEAVES;
        obj_extract_self(kickedobj);
        newsym(x, y);
        if (costly && (!costly_spot(u.ux, u.uy)
                       || !index(u.urooms, *in_rooms(x, y, SHOPBASE))))
            addtobill(kickedobj, FALSE, FALSE, FALSE);
        if (!flooreffects(kickedobj, u.ux, u.uy, "fall")) 
        {
            place_object(kickedobj, u.ux, u.uy);
            stackobj(kickedobj);
            newsym(u.ux, u.uy);
        }
        return 1;
    }

    /* a box gets a chance of breaking open here */
    if (Is_box(kickedobj)) 
    {
        if (is_golf_swing)
        {
            pline("Thump!");
            return 1;
        }

        boolean otrp = kickedobj->otrapped;

        if (range < 2)
        {
            pline("THUD!");
        }
        container_impact_dmg(kickedobj, x, y);
        if (kickedobj->olocked) 
        {
            if (!rn2(5) || (martial() && !rn2(2))) 
            {
                if (kickedobj->keyotyp == STRANGE_OBJECT || kickedobj->keyotyp == NON_PM || kickedobj->keyotyp == SKELETON_KEY)
                {
                    play_simple_container_sound(kickedobj, CONTAINER_SOUND_TYPE_BREAK_LOCK);
                    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "break open the lock!");
                    breakchestlock(kickedobj, FALSE);
                    if (otrp)
                        (void) chest_trap(kickedobj, LEG, FALSE);
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_HINT, "It feels that the lock of %s cannot be broken by kicking.", the(cxname(kickedobj)));
                }
                return 1;
            }
        }
        else 
        {
            if (!rn2(3) || (martial() && !rn2(2))) 
            {
                play_simple_container_sound(kickedobj, CONTAINER_SOUND_TYPE_LID_SLAM);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "lid slams open, then falls shut.");
                kickedobj->lknown = 1;
                if (otrp)
                    (void) chest_trap(kickedobj, LEG, FALSE);
                return 1;
            }
        }
        if (range < 2)
            return 1;
        /* else let it fall through to the next cases... */
    }
    else if (Is_container(kickedobj) && (objects[kickedobj->otyp].oc_flags4 & O4_CONTAINER_MAY_CONTAIN_MONSTER))
    {
        pline("THUD!");
        container_impact_dmg(kickedobj, x, y);
        if (maybe_disturb_container_monster(kickedobj))
        {
            return 1;
        }
    }

    /* fragile objects should not be kicked */
    if (hero_breaks(kickedobj, kickedobj->ox, kickedobj->oy, FALSE))
        return 1;

    /* too heavy to move.  range is calculated as potential distance from
     * player, so range == 2 means the object may move up to one square
     * from its current position
     */
    if (range < 2) 
    {
        if (!Is_box(kickedobj))
        {
            pline("Thump!");
        }
        return (!rn2(3) || martial());
    }

    if (kickedobj->quan > 1L) 
    {
        if (!isgold) 
        {
            kickedobj = splitobj(kickedobj, 1L);
        }
        else
        {
            if (rn2(20)) 
            {
                static NEARDATA const char *const flyingcoinmsg[] = {
                    "scatter the coins", "knock coins all over the place",
                    "send coins flying in all directions",
                };

                pline("Thwwpingg!");
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s!", flyingcoinmsg[rn2(SIZE(flyingcoinmsg))]);
                (void) scatter(x, y, rn2(3) + 1, VIS_EFFECTS | MAY_HIT,
                               kickedobj);
                newsym(x, y);
                return 1;
            }
            if (kickedobj->quan > 300L) 
            {
                play_sfx_sound(SFX_THUMP_HIT);
                pline("Thump!");
                return (!rn2(3) || martial());
            }
        }
    }

    if (slide && !Blind)
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Whee!  %s %s across the %s.", Doname2(kickedobj),
              otense(kickedobj, "slide"), surface(x, y));

    if (costly && !isgold)
        addtobill(kickedobj, FALSE, FALSE, TRUE);
    obj_extract_self(kickedobj);
    (void) snuff_candle(kickedobj);
    newsym(x, y);
    mon = bhit(u.dx, u.dy, range, 0, is_golf_swing ? GOLF_SWING : KICKED_WEAPON,
               (int FDECL((*), (MONST_P, OBJ_P, MONST_P))) 0,
               (int FDECL((*), (OBJ_P, OBJ_P, MONST_P))) 0, &kickedobj, &youmonst, TRUE, FALSE);
    if (!kickedobj)
        return 1; /* object broken */

    uchar hitres = 0;
    if (mon) {
        if (mon->isshk && kickedobj->where == OBJ_MINVENT
            && kickedobj->ocarry == mon)
            return 1; /* alert shk caught it */
        notonhead = (mon->mx != bhitpos.x || mon->my != bhitpos.y);
        if (isgold ? ghitm(mon, kickedobj, &hitres)      /* caught? */
                   : thitmonst(mon, kickedobj, is_golf_swing, &hitres)) /* hit && used up? */
            return 1;
    }

    /* the object might have fallen down a hole;
       ship_object() will have taken care of shop billing */
    if (kickedobj->where == OBJ_MIGRATING)
        return 1;

    bhitroom = *in_rooms(bhitpos.x, bhitpos.y, SHOPBASE);
    if (costly && (!costly_spot(bhitpos.x, bhitpos.y)
                   || *in_rooms(x, y, SHOPBASE) != bhitroom)) {
        if (isgold)
            costly_gold(x, y, kickedobj->quan);
        else
            (void) stolen_value(kickedobj, x, y, is_peaceful(shkp),
                                FALSE);
    }

    if (flooreffects(kickedobj, bhitpos.x, bhitpos.y, "fall"))
        return 1;
    if (kickedobj->unpaid)
        subfrombill(kickedobj, shkp);
    place_object(kickedobj, bhitpos.x, bhitpos.y);
    play_object_floor_sound(kickedobj, OBJECT_SOUND_TYPE_DROP_AFTER_THROW, Underwater);
    stackobj(kickedobj);
    newsym(kickedobj->ox, kickedobj->oy);
    return 1;
}

/* cause of death if kicking kills kicker */
STATIC_OVL char *
kickstr(buf, kickobjnam)
char *buf;
const char *kickobjnam;
{
    const char *what;

    if (*kickobjnam)
        what = kickobjnam;
    else if (maploc == &nowhere)
        what = "nothing";
    else if (IS_DOOR(maploc->typ))
        what = "a door";
    else if (IS_TREE(maploc->typ))
        what = "a tree";
    else if (IS_STWALL(maploc->typ))
        what = "a wall";
    else if (IS_ROCK(maploc->typ))
        what = "a rock";
    else if (IS_THRONE(maploc->typ))
        what = "a throne";
    else if (IS_FOUNTAIN(maploc->typ))
        what = "a fountain";
    else if (IS_GRAVE(maploc->typ))
        what = "a headstone";
    else if (IS_SINK(maploc->typ))
        what = "a sink";
    else if (IS_ALTAR(maploc->typ))
        what = "an altar";
    else if (IS_ANVIL(maploc->typ))
        what = "an anvil";
    else if (IS_DRAWBRIDGE(maploc->typ))
        what = "a drawbridge";
    else if (maploc->typ == STAIRS)
        what = "the stairs";
    else if (maploc->typ == LADDER)
        what = "a ladder";
    else if (maploc->typ == IRONBARS)
        what = "an iron bar";
    else
        what = "something weird";
    return strcat(strcpy(buf, "kicking "), what);
}

#ifdef ANDROID
void
autokick()
{
    if (nolimbs(youmonst.data) || slithy(youmonst.data))
        return;
    if (verysmall(youmonst.data))
        return;
    if (u.usteed)
        return;
    if (Wounded_legs)
        return;
    if (near_capacity() > SLT_ENCUMBER)
        return;
    if (youmonst.data->mlet == S_LIZARD)
        return;
    if (u.utrap)
        return;

    if (yn_query("Kick it open?") == 'y')
        dokick_indir(TRUE);
}

int
dokick() {
    return dokick_indir(FALSE);
}

int
dokick_indir(has_dir)
boolean has_dir;
{
#else
int
dokick() {
#endif
    int x, y;
    int avrg_attrib;
    int dmg = 0, glyph, oldglyph = -1;
    register struct monst *mtmp;
    boolean no_kick = FALSE;
    char buf[BUFSZ], kickobjnam[BUFSZ];

    kickobjnam[0] = '\0';
    if (nolimbs(youmonst.data) || slithy(youmonst.data)) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "have no legs to kick with.");
        no_kick = TRUE;
    }
    else if (verysmall(youmonst.data))
    {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are too small to do any kicking.");
        no_kick = TRUE;
    } 
    else if (u.usteed) 
    {
        if (yn_function_es(YN_STYLE_GENERAL, ATR_NONE, CLR_MSG_ATTENTION, (const char*)0, "Kick your steed?", ynchars, 'y', yndescs, (const char*)0) == 'y')
        {
            You("kick %s.", mon_nam(u.usteed));
            kick_steed();
            return 1;
        } 
        else 
        {
            return 0;
        }
    } 
    else if (Wounded_legs) 
    {
        /* note: jump() has similar code */
        long wl = (EWounded_legs & BOTH_SIDES);
        const char *bp = body_part(LEG);

        if (wl == BOTH_SIDES)
            bp = makeplural(bp);
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "%s%s %s in no shape for kicking.",
             (wl == LEFT_SIDE) ? "left " : (wl == RIGHT_SIDE) ? "right " : "",
             bp, (wl == BOTH_SIDES) ? "are" : "is");
        no_kick = TRUE;
    } 
    else if (near_capacity() > SLT_ENCUMBER)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "load is too heavy to balance yourself for a kick.");
        no_kick = TRUE;
    }
    else if (youmonst.data->mlet == S_LIZARD) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "legs cannot kick effectively.");
        no_kick = TRUE;
    } 
    else if (u.uinwater && !rn2(2)) 
    {
        //play_monster_simple_weapon_sound(&youmonst, BAREFOOTED_ATTACK_NUMBER, uarmf, OBJECT_SOUND_TYPE_SWING_MELEE);
        Your("slow motion kick doesn't hit anything.");
        no_kick = TRUE;
    }
    else if (u.utrap)
    {
        no_kick = TRUE;
        switch (u.utraptype)
        {
        case TT_PIT:
            if (!Passes_walls)
            {
                play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
                pline_ex(ATR_NONE, CLR_MSG_FAIL, "There's not enough room to kick down here.");
            }
            else
                no_kick = FALSE;
            break;
        case TT_WEB:
        case TT_BEARTRAP:
            play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "move your %s!", body_part(LEG));
            break;
        default:
            break;
        }
    }

    if (no_kick) 
    {
        /* ignore direction typed before player notices kick failed */
        display_nhwindow(WIN_MESSAGE, TRUE); /* --More-- */
        return 0;
    }

    if (
#ifdef ANDROID
        !has_dir &&
#endif
        !getdir((char *) 0)
        )
        return 0;
    if (!u.dx && !u.dy)
        return 0;

    update_u_facing(TRUE);

    x = u.ux + u.dx;
    y = u.uy + u.dy;
    bhitpos.x = x;
    bhitpos.y = y;

    /* KMH -- Kicking boots always succeed */
    /* JG  -- Martial artists always succeed */
    if (Magical_kicking)
        avrg_attrib = 99;
    else if (martial_bonus())
        avrg_attrib = 99;
    else
        avrg_attrib = (ACURRSTR + ACURR(A_DEX) + ACURR(A_CON)) / 3;

    if (u.uswallow) 
    {
        switch (rn2(3)) 
        {
        case 0:
            play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "move your %s!", body_part(LEG));
            break;
        case 1:
            if (is_animal(u.ustuck->data))
            {
                play_sfx_sound(SFX_BURP);
                pline("%s burps loudly.", Monnam(u.ustuck));
                break;
            }
            /*FALLTHRU*/
        default:
            update_u_action(ACTION_TILE_KICK);
            play_monster_simple_weapon_sound(&youmonst, BAREFOOTED_ATTACK_NUMBER, uarmf, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(u.ustuck), NATTK, uarmf, 0.0, HMON_MELEE);
            Your("feeble kick has no effect.");
            break;
        }
        update_u_action_revert(ACTION_TILE_NO_ACTION);
        return 1;
    } 
    else if (u.utrap && u.utraptype == TT_PIT) 
    {
        /* must be Passes_walls */
        struct trap* t = t_at(u.ux, u.uy);
        update_u_action(ACTION_TILE_KICK);
        play_monster_simple_weapon_sound(&youmonst, BAREFOOTED_ATTACK_NUMBER, uarmf, OBJECT_SOUND_TYPE_SWING_MELEE);
        u_wait_until_action();
        You("kick at the side of the pit.");
        if (t)
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_TRAP, trap_to_any(t), BAREFOOTED_ATTACK_NUMBER, uarmf, 5.0, HMON_MELEE);
        update_u_action_revert(ACTION_TILE_NO_ACTION);
        return 1;
    }
    if (Levitation) 
    {
        int xx, yy;

        xx = u.ux - u.dx;
        yy = u.uy - u.dy;
        /* doors can be opened while levitating, so they must be
         * reachable for bracing purposes
         * Possible extension: allow bracing against stuff on the side?
         */
        if (isok(xx, yy) && !IS_ROCK(levl[xx][yy].typ)
            && !IS_DOOR(levl[xx][yy].typ)
            && (!Is_airlevel(&u.uz) || !OBJ_AT(xx, yy))) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "have nothing to brace yourself against.");
            return 0;
        }
    }

    mtmp = isok(x, y) ? m_at(x, y) : 0;
    /* might not kick monster if it is hidden and becomes revealed,
       if it is peaceful and player declines to attack, or if the
       hero passes out due to encumbrance with low hp; context.move
       will be 1 unless player declines to kick peaceful monster */
    if (mtmp)
    {
        oldglyph = glyph_at(x, y);
        if (!maybe_kick_monster(mtmp, x, y))
        {
            return context.move;
        }
    }

    wake_nearby();
    u_wipe_engr(2);

    if (!isok(x, y)) 
    {
        maploc = &nowhere;
        goto ouch;
    }
    maploc = &levl[x][y];

    /*
     * The next five tests should stay in their present order:
     * monsters, pools, objects, non-doors, doors.
     *
     * [FIXME:  Monsters who are hidden underneath objects or
     * in pools should lead to hero kicking the concealment
     * rather than the monster, probably exposing the hidden
     * monster in the process.  And monsters who are hidden on
     * ceiling shouldn't be kickable (unless hero is flying?);
     * kicking toward them should just target whatever is on
     * the floor at that spot.]
     */

    if (mtmp)
    {
        /* save mtmp->data (for recoil) in case mtmp gets killed */
        struct permonst *mdat = mtmp->data;

        kick_monster(mtmp, x, y);
        glyph = glyph_at(x, y);
        /* see comment in attack_checks() */
        if (DEADMONSTER(mtmp)) { /* DEADMONSTER() */
            /* if we mapped an invisible monster and immediately
               killed it, we don't want to forget what we thought
               was there before the kick */
            if (glyph != oldglyph && glyph_is_invisible(glyph))
                clear_current_and_show_glyph(x, y, oldglyph);
        } else if (!canspotmon(mtmp)
                   /* check <x,y>; monster that evades kick by jumping
                      to an unseen square doesn't leave an I behind */
                   && mtmp->mx == x && mtmp->my == y
                   && !glyph_is_invisible(glyph)
                   && !(u.uswallow && mtmp == u.ustuck)) {
            map_invisible(x, y);
        }
        /* recoil if floating */
        if ((Is_airlevel(&u.uz) || Levitation) && context.move) {
            int range;

            range =
                ((int) youmonst.data->cwt + (weight_cap() + inv_weight()));
            if (range < 1)
                range = 1; /* divide by zero avoidance */
            range = (3 * (int) mdat->cwt) / range;

            if (range < 1)
                range = 1;
            hurtle(-u.dx, -u.dy, range, TRUE);
        }
        return 1;
    }

    update_u_action(ACTION_TILE_KICK);
    play_monster_simple_weapon_sound(&youmonst, BAREFOOTED_ATTACK_NUMBER, uarmf, OBJECT_SOUND_TYPE_SWING_MELEE);
    u_wait_until_action();

    (void) unmap_invisible(x, y);
    if (is_pool(x, y) ^ !!u.uinwater) {
        /* objects normally can't be removed from water by kicking */
        play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 1.0, HMON_MELEE);
        You("splash some %s around.", hliquid("water"));
        update_u_action_revert(ACTION_TILE_NO_ACTION);
        return 1;
    }

    if (OBJ_AT(x, y) && (!Levitation || Is_airlevel(&u.uz)
                         || Is_waterlevel(&u.uz) || sobj_at(BOULDER, x, y))) 
    {
        play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_OBJECT, obj_to_any(level.objects[x][y]), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
        if (kick_object(x, y, kickobjnam, FALSE))
        {
            if (Is_airlevel(&u.uz))
                hurtle(-u.dx, -u.dy, 1, TRUE); /* assume it's light */

            update_u_action_revert(ACTION_TILE_NO_ACTION);
            return 1;
        }
        goto ouch;
    }

    if (!IS_DOOR(maploc->typ)) 
    {
        if (maploc->typ == SDOOR) 
        {
            if (is_door_kickable_at_ptr(maploc) && !Levitation && rn2(30) < avrg_attrib)
            {
                play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
                cvt_sdoor_to_door(x, y); /* ->typ = DOOR */
                pline("Crash!  %s a secret door!",
                      /* don't "kick open" when it's locked
                         unless it also happens to be trapped */
                      ((maploc->doormask & (D_LOCKED | D_TRAPPED)) & D_MASK) == D_LOCKED
                          ? "Your kick uncovers"
                          : "You kick open");
                exercise(A_DEX, TRUE);
                if (maploc->doormask & D_TRAPPED)
                {
                    b_trapped(get_door_name_at_ptr(maploc), FOOT, x, y);
                    maploc->doormask &= ~D_TRAPPED;
                    if (is_door_destroyed_by_booby_trap_at_ptr(maploc))
                    {
                        maploc->doormask &= ~D_MASK;
                        maploc->doormask |= D_NODOOR;
                    }
                }
                else if ((maploc->doormask & D_MASK) != D_NODOOR && (maploc->doormask & D_MASK) != D_PORTCULLIS
                    && !(maploc->doormask & D_LOCKED))
                {
                    maploc->doormask &= ~D_MASK;
                    maploc->doormask |= D_ISOPEN;
                }
                feel_newsym(x, y); /* we know it's gone */
                if ((maploc->doormask & D_MASK) == D_ISOPEN
                    || (maploc->doormask & D_MASK) == D_NODOOR || (maploc->doormask & D_MASK) == D_PORTCULLIS)
                    unblock_vision_and_hearing_at_point(x, y); /* vision */

                update_hearing_array_and_ambient_sounds_if_point_within_hearing_range(x, y);
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            } 
            else
            {
                play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
                goto ouch;
            }
        }
        if (maploc->typ == SCORR) 
        {
            if (!Levitation && rn2(30) < avrg_attrib)
            {
                play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
                pline("Crash!  You kick open a secret passage!");
                exercise(A_DEX, TRUE);
                create_basic_floor_location(x, y, levl[x][y].floortyp ? levl[x][y].floortyp : CORR, 0, 0, FALSE);
                feel_newsym(x, y); /* we know it's gone */
                unblock_vision_and_hearing_at_point(x, y); /* vision */
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            } 
            else
            {
                play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
                goto ouch;
            }
        }
        if (IS_THRONE(maploc->typ)) 
        {
            register int i;
            if (Levitation)
                goto dumb;
            if ((Luck < 0 || maploc->flags) && !rn2(3)) 
            {
                create_basic_floor_location(x, y, maploc->floortyp ? maploc->floortyp : ROOM, maploc->floorsubtyp ? maploc->floorsubtyp : 0, 0, FALSE);
                (void) mkgold((long) rnd(200), x, y);
                play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
                if (Blind)
                    pline("CRASH!  You destroy it.");
                else {
                    pline("CRASH!  You destroy the throne.");
                    newsym(x, y);
                }
                exercise(A_DEX, TRUE);
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            } else if (Luck > 0 && !rn2(3) && !maploc->looted) {
                (void) mkgold((long) rn1(201, 300), x, y);
                i = Luck + 1;
                if (i > 6)
                    i = 6;
                while (i--)
                    (void) mksobj_at(
                        rnd_class(DILITHIUM_CRYSTAL, LUCKSTONE - 1), x, y,
                        FALSE, TRUE);

                play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
                if (Blind)
                    You("kick %s loose!", something);
                else {
                    You("kick loose some ornamental coins and gems!");
                    newsym(x, y);
                }
                /* prevent endless milking */
                maploc->looted = T_LOOTED;
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            } else if (!rn2(4)) {
                if (dunlev(&u.uz) < dunlevs_in_dungeon(&u.uz)) {
                    fall_through(FALSE);
                    update_u_action_revert(ACTION_TILE_NO_ACTION);
                    return 1;
                } else
                    goto ouch;
            }
            goto ouch;
        }
        if (IS_ALTAR(maploc->typ)) {
            if (Levitation)
                goto dumb;
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
            You("kick %s.", (Blind ? something : "the altar"));
            if (!rn2(3))
                goto ouch;
            altar_wrath(x, y);
            exercise(A_DEX, TRUE);
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            return 1;
        }
        if (IS_ANVIL(maploc->typ)) {
            if (Levitation)
                goto dumb;
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
            You("kick %s.", (Blind ? something : "the anvil"));
            goto ouch;
        }
        if (IS_BRAZIER(maploc->typ)) {
            if (Levitation)
                goto dumb;
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
            You("kick %s.", (Blind ? something : "the brazier"));
            goto ouch;
        }
        if (IS_SIGNPOST(maploc->typ)) {
            if (Levitation)
                goto dumb;
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
            You("kick %s.", (Blind ? something : "the signpost"));
            goto ouch;
        }
        if (IS_FOUNTAIN(maploc->typ)) {
            if (Levitation)
                goto dumb;
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
            You("kick %s.", (Blind ? something : "the fountain"));
            if (!rn2(3))
                goto ouch;
            /* make metal boots rust */
            if (uarmf && rn2(3))
                if (water_damage(uarmf, "metal boots", TRUE) == ER_NOTHING) {
                    Your("boots get wet.");
                    /* could cause short-lived fumbling here */
                }
            exercise(A_DEX, TRUE);
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            return 1;
        }
        if (IS_GRAVE(maploc->typ)) {
            if (Levitation)
                goto dumb;
            if (rn2(4))
                goto ouch;
            exercise(A_WIS, FALSE);
            if (Role_if(PM_ARCHAEOLOGIST) || Role_if(PM_SAMURAI)
                || ((u.ualign.type == A_LAWFUL) && (u.ualign.record > -10))) {
                adjalign(-sgn(u.ualign.type));
            }
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
            create_current_floor_location(x, y, 0, NO_GLYPH, FALSE);
            (void) mksobj_at(ROCK, x, y, TRUE, FALSE);
            del_engr_at(x, y);
            if (Blind)
                pline("Crack!  %s broke!", Something);
            else {
                pline_The("headstone topples over and breaks!");
                newsym(x, y);
            }
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            return 1;
        }
        if (maploc->typ == IRONBARS)
        {
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
            goto ouch;
        }
        
        if (IS_TREE(maploc->typ)) 
        {
            struct obj *treefruit;
            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);

            /* nothing, fruit or trouble? 75:23.5:1.5% */
            if (rn2(3)) 
            {
                if (/*!rn2(6)*/ (levl[x][y].flags & TREE_HAS_BEE_HIVE) && !(mvitals[PM_KILLER_BEE].mvflags & MV_GONE))
                    You_hear("a low buzzing."); /* a warning */
                goto ouch;
            }

            if (tree_subtype_definitions[maploc->subtyp].fruit_type > STRANGE_OBJECT && rn2(100) < tree_subtype_definitions[maploc->subtyp].fruit_kick_drop_chance) //!(maploc->looted & TREE_LOOTED) && (treefruit = rnd_treefruit_at(x, y)))
            {
                if (maploc->special_quality > 0)
                {
                    long nfruit = (long)min(maploc->special_quality, max(0, d(tree_subtype_definitions[maploc->subtyp].fruit_drop_d, tree_subtype_definitions[maploc->subtyp].fruit_drop_n) + tree_subtype_definitions[maploc->subtyp].fruit_drop_p));
                    long nfall;
                    if (nfruit > 0)
                    {
                        maploc->special_quality -= (short)nfruit;
                        treefruit = mksobj_at(tree_subtype_definitions[maploc->subtyp].fruit_type, x, y, TRUE, FALSE); //rnd_treefruit_at(mtmp->mx, mtmp->my);
                        treefruit->quan = nfruit;
                        treefruit->owt = weight(treefruit);

                        short frtype = treefruit->otyp;

                        if (is_plural(treefruit))
                            pline("Some %s fall from the tree!", xname(treefruit));
                        else
                            pline("%s falls from the tree!", An(xname(treefruit)));

                        nfall = scatter(x, y, 2, MAY_HIT | ADD_CAUGHT_IN_LEAVES_FLAG, treefruit);
                        if (nfall != nfruit)
                        {
                            /* scatter left some in the tree, but treefruit
                             * may not refer to the correct object */
                            struct obj caughtfruit = zeroobj;
                            caughtfruit.otyp = frtype;
                            caughtfruit.oclass = objects[frtype].oc_class;
                            caughtfruit.quan = nfruit - nfall;
                            //treefruit = mksobj(frtype, TRUE, FALSE, FALSE);
                            //treefruit->quan = nfruit - nfall;
                            pline("%ld %s got caught in the branches.",
                                nfruit - nfall, xname(&caughtfruit));
                            //dealloc_obj(treefruit);
                        }
                        exercise(A_DEX, TRUE);
                        exercise(A_WIS, TRUE); /* discovered a new food source! */
                        newsym(x, y);
                        //maploc->looted |= TREE_LOOTED;
                        update_u_action_revert(ACTION_TILE_NO_ACTION);
                    }
                    return 1;
                }
                else
                {
                    struct obj pseudo = zeroobj;
                    pseudo.otyp = tree_subtype_definitions[maploc->subtyp].fruit_type;
                    pseudo.oclass = objects[pseudo.otyp].oc_class;
                    pseudo.quan = 2; /* Plural */
                    pline("The tree seems to be low on %s!", xname(&pseudo));
                    return 1;
                }
            } 
            else if ((levl[x][y].flags & TREE_HAS_BEE_HIVE) && !(maploc->looted & TREE_SWARM))
            {
                int cnt = rnl(4) + 2;
                int made = 0;
                coord mm;

                mm.x = x;
                mm.y = y;
                while (cnt--) {
                    if (enexto(&mm, mm.x, mm.y, &mons[PM_KILLER_BEE])
                        && makemon(&mons[PM_KILLER_BEE], mm.x, mm.y,
                                   MM_ANGRY))
                        made++;
                }
                if (made)
                    pline("You've attracted the tree's former occupants!");
                else
                    You("smell stale honey.");
                maploc->looted |= TREE_SWARM;
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            }
            goto ouch;
        }
        if (IS_SINK(maploc->typ)) {
            int gend = poly_gender();
            short washerndx = PM_INCUBUS;
            boolean washerfemale = (gend == 1 || (gend == 2 && rn2(2)))
                ? 0
                : 1;

            if (Levitation)
                goto dumb;

            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
            
            if (rn2(5)) 
            {
                play_sfx_sound(SFX_SINK_KLUNK);
                if (!Deaf)
                    pline("Klunk!  The pipes vibrate noisily.");
                else
                    pline("Klunk!");
                exercise(A_DEX, TRUE);
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            } else if (!(maploc->looted & S_LPUDDING) && !rn2(3)
                       && !(mvitals[PM_BLACK_PUDDING].mvflags & MV_GONE)) {
                if (Blind)
                    You_hear("a gushing sound.");
                else
                    pline("A %s ooze gushes up from the drain!",
                          hcolor(NH_BLACK));
                (void) makemon(&mons[PM_BLACK_PUDDING], x, y, NO_MM_FLAGS);
                exercise(A_DEX, TRUE);
                newsym(x, y);
                maploc->looted |= S_LPUDDING;
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            } else if (!(maploc->looted & S_LDWASHER) && !rn2(3)
                       && !(mvitals[washerndx].mvflags & MV_GONE)) {
                /* can't resist... */
                pline("%s returns!", (Blind ? Something : "The dish washer"));
                if (makemon(&mons[washerndx], x, y, washerfemale ? MM_FEMALE : NO_MM_FLAGS))
                    newsym(x, y);
                maploc->looted |= S_LDWASHER;
                exercise(A_DEX, TRUE);
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            } else if (!rn2(3)) {
                pline("Flupp!  %s.",
                      (Blind ? "You hear a sloshing sound"
                             : "Muddy waste pops up from the drain"));
                if (!(maploc->looted & S_LRING)) { /* once per sink */
                    if (!Blind)
                        You_see("a ring shining in its midst.");
                    (void) mkobj_at(RING_CLASS, x, y, TRUE);
                    newsym(x, y);
                    exercise(A_DEX, TRUE);
                    exercise(A_WIS, TRUE); /* a discovery! */
                    maploc->looted |= S_LRING;
                }
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            }
            goto ouch;
        }
        if (maploc->typ == STAIRS || maploc->typ == LADDER
            || IS_STWALL(maploc->typ)) 
        {
            if (!IS_STWALL(maploc->typ) && maploc->ladder == LA_DOWN)
                goto dumb;

            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
 ouch:

            play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
            pline("Ouch!  That hurts!");
            exercise(A_DEX, FALSE);
            exercise(A_STR, FALSE);
            if (isok(x, y)) {
                if (Blind)
                    feel_location(x, y); /* we know we hit it */
                if (is_drawbridge_wall(x, y) >= 0) {
                    play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, x, y);
                    pline_The("drawbridge is unaffected.");
                    /* update maploc to refer to the drawbridge */
                    (void) find_drawbridge(&x, &y);
                    maploc = &levl[x][y];
                }
            }
            if (!rn2(3))
                set_wounded_legs(RIGHT_SIDE, 5 + rnd(5));
            dmg = rnd(ACURR(A_CON) > 15 ? 3 : 5);
            losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), kickstr(buf, kickobjnam), KILLED_BY);
            if (Is_airlevel(&u.uz) || Levitation)
                hurtle(-u.dx, -u.dy, rn1(2, 4), TRUE); /* assume it's heavy */
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            return 1;
        }
        goto dumb;
    }

    if ((maploc->doormask & D_MASK) == D_ISOPEN || (maploc->doormask & D_MASK) == D_BROKEN
        || (maploc->doormask & D_MASK) == D_NODOOR || (maploc->doormask & D_MASK) == D_PORTCULLIS) {
 dumb:
        exercise(A_DEX, FALSE);
        if (martial() || ACURR(A_DEX) >= 16 || rn2(3)) {
            You("kick at empty space.");
            if (Blind)
                feel_location(x, y);
        } else {
            play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
            pline("Dumb move!  You strain a muscle.");
            exercise(A_STR, FALSE);
            set_wounded_legs(RIGHT_SIDE, 5 + rnd(5));
        }
        if ((Is_airlevel(&u.uz) || Levitation) && rn2(2))
            hurtle(-u.dx, -u.dy, 1, TRUE);
        update_u_action_revert(ACTION_TILE_NO_ACTION);
        return 1; /* uses a turn */
    }

    /* not enough leverage to kick open doors while levitating */
    if (Levitation)
        goto ouch;

    exercise(A_DEX, TRUE);

    /* door is known to be CLOSED or LOCKED */
    play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(x, y), NATTK, (struct obj*)0, 5.0, HMON_MELEE);
    boolean roll_success = (rnl(35) < avrg_attrib + (!martial() ? 0 : ACURR(A_DEX)));
    if (roll_success && is_door_kickable_at_ptr(maploc))
    {
        boolean shopdoor = *in_rooms(x, y, SHOPBASE) ? TRUE : FALSE;

        /* break the door */
        if (maploc->doormask & D_TRAPPED) 
        {
            if (flags.verbose)
                You("kick the %s.", get_door_name_at(x, y));
            exercise(A_STR, FALSE);
            b_trapped(get_door_name_at_ptr(maploc), FOOT, x, y);
            maploc->doormask &= ~D_TRAPPED;

            if (!is_door_indestructible_at_ptr(maploc))
            {
                maploc->doormask &= ~D_MASK;
                maploc->doormask |= D_NODOOR;
                if (!rn2(2))
                {
                    struct obj* otmp = mksobj_at(PIECE_OF_WOOD, x, y, FALSE, FALSE);
                    otmp->quan = 1;
                    otmp->owt = weight(otmp);
                }
            }
        }
        else if (ACURR(A_STR) > 18 && !rn2(5) && !shopdoor)
        {
            play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_WHAM);
            play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_BREAK);
            pline("As you kick the %s, it shatters to pieces!", get_door_name_at(x, y));
            exercise(A_STR, TRUE);
            maploc->doormask &= ~D_MASK;
            maploc->doormask |= D_NODOOR;

            struct obj* otmp = mksobj_at(PIECE_OF_WOOD, x, y, FALSE, FALSE);
            otmp->quan = 1;
            otmp->owt = weight(otmp);

        } 
        else
        {
            play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_WHAM);
            play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_BREAK);
            pline("As you kick the %s, it crashes open!", get_door_name_at(x, y));
            exercise(A_STR, TRUE);
            maploc->doormask &= ~D_MASK;
            maploc->doormask |= D_BROKEN;
        }

        feel_newsym(x, y); /* we know we broke it */
        unblock_vision_and_hearing_at_point(x, y); /* vision */

        if (shopdoor)
        {
            add_damage(x, y, SHOP_DOOR_COST);
            pay_for_damage("break", FALSE);
        }

        if (in_town(x, y))
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
            {
                if (DEADMONSTER(mtmp))
                    continue;
                if (is_watch(mtmp->data) && couldsee(mtmp->mx, mtmp->my)
                    && is_peaceful(mtmp)) 
                {
                    context.global_minimum_volume = 0.25f;
                    play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_HALT_THIEF_YOURE_UNDER_ARREST);
                    context.global_minimum_volume = 0.0f;
                    mon_yells(mtmp, "Halt, thief!  You're under arrest!", "yell", "angrily", FALSE);
                    (void) angry_guards(FALSE);
                    break;
                }
            }
    }
    else
    {
        if (Blind)
            feel_location(x, y); /* we know we hit it */
        exercise(A_STR, TRUE);
        play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_WHAM);
        pline("WHAMMM!!!");
        if(roll_success)
            pline_ex(ATR_NONE, CLR_MSG_HINT, "The %s feels too hard to be broken by kicking.", get_door_name_at(x, y));
        if (in_town(x, y))
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
            {
                if (DEADMONSTER(mtmp))
                    continue;
                if (is_watch(mtmp->data) && is_peaceful(mtmp)
                    && couldsee(mtmp->mx, mtmp->my)) 
                {
                    if (levl[x][y].looted & D_WARNED)
                    {
                        context.global_minimum_volume = 0.25f;
                        play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_HALT_VANDAL_YOURE_UNDER_ARREST);
                        context.global_minimum_volume = 0.0f;
                        mon_yells(mtmp,
                                  "Halt, vandal!  You're under arrest!", "yell", "angrily", FALSE);
                        (void) angry_guards(FALSE);
                    } else {
                        context.global_minimum_volume = 0.25f;
                        play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_HEY_STOP_DAMAGING_THAT_DOOR);
                        context.global_minimum_volume = 0.0f;
                        mon_yells(mtmp, "Hey, stop damaging that door!", "yell", "angrily", FALSE);
                        levl[x][y].looted |= D_WARNED;
                    }
                    break;
                }
            }
    }
    update_u_action_revert(ACTION_TILE_NO_ACTION);
    return 1;
}

STATIC_OVL void
drop_to(cc, loc)
coord *cc;
schar loc;
{
    /* cover all the MIGR_xxx choices generated by down_gate() */
    switch (loc) {
    case MIGR_RANDOM: /* trap door or hole */
        if (Is_stronghold(&u.uz)) {
            cc->x = valley_level.dnum;
            cc->y = valley_level.dlevel;
            break;
        } else if (In_endgame(&u.uz) || Is_botlevel(&u.uz)) {
            cc->y = cc->x = 0;
            break;
        }
        /*FALLTHRU*/
    case MIGR_STAIRS_UP:
    case MIGR_LADDER_UP:
        cc->x = u.uz.dnum;
        cc->y = u.uz.dlevel + 1;
        break;
    case MIGR_SSTAIRS:
        cc->x = sstairs.tolev.dnum;
        cc->y = sstairs.tolev.dlevel;
        break;
    default:
    case MIGR_NOWHERE:
        /* y==0 means "nowhere", in which case x doesn't matter */
        cc->y = cc->x = 0;
        break;
    }
}

/* player or missile impacts location, causing objects to fall down */
void
impact_drop(missile, x, y, dlev)
struct obj *missile; /* caused impact, won't drop itself */
xchar x, y;          /* location affected */
xchar dlev;          /* if !0 send to dlev near player */
{
    schar toloc;
    register struct obj *obj, *obj2;
    register struct monst *shkp;
    long oct, dct, price, debit, robbed;
    boolean angry, costly, isrock;
    coord cc;

    if (!OBJ_AT(x, y))
        return;

    toloc = down_gate(x, y);
    drop_to(&cc, toloc);
    if (!cc.y)
        return;

    if (dlev) {
        /* send objects next to player falling through trap door.
         * checked in obj_delivery().
         */
        toloc = MIGR_WITH_HERO;
        cc.y = dlev;
    }

    costly = costly_spot(x, y);
    price = debit = robbed = 0L;
    angry = FALSE;
    shkp = (struct monst *) 0;
    /* if 'costly', we must keep a record of ESHK(shkp) before
     * it undergoes changes through the calls to stolen_value.
     * the angry bit must be reset, if needed, in this fn, since
     * stolen_value is called under the 'silent' flag to avoid
     * unsavory pline repetitions.
     */
    if (costly) {
        if ((shkp = shop_keeper(*in_rooms(x, y, SHOPBASE))) != 0) {
            debit = ESHK(shkp)->debit;
            robbed = ESHK(shkp)->robbed;
            angry = !is_peaceful(shkp);
        }
    }

    isrock = (missile && is_rock(missile));
    oct = dct = 0L;
    for (obj = level.objects[x][y]; obj; obj = obj2) {
        obj2 = obj->nexthere;
        if (obj == missile)
            continue;
        /* number of objects in the pile */
        oct += obj->quan;
        if (obj == uball || obj == uchain)
            continue;
        /* boulders can fall too, but rarely & never due to rocks */
        if ((isrock && obj->otyp == BOULDER)
            || rn2(obj->otyp == BOULDER ? 30 : 3))
            continue;
        obj_extract_self(obj);

        if (costly) {
            price += stolen_value(obj, x, y,
                                  (costly_spot(u.ux, u.uy)
                                   && index(u.urooms,
                                            *in_rooms(x, y, SHOPBASE))),
                                  TRUE);
            /* set obj->no_charge to 0 */
            if (Has_contents(obj))
                picked_container(obj); /* does the right thing */
            if (obj->oclass != COIN_CLASS)
                obj->no_charge = 0;
        }

        add_to_migration(obj);
        obj->ox = cc.x;
        obj->oy = cc.y;
        obj->owornmask = (long) toloc;

        /* number of fallen objects */
        dct += obj->quan;
    }

    if (dct && cansee(x, y)) { /* at least one object fell */
        const char *what = (dct == 1L ? "object falls" : "objects fall");

        if (missile)
            pline("From the impact, %sother %s.",
                  dct == oct ? "the " : dct == 1L ? "an" : "", what);
        else if (oct == dct)
            pline("%s adjacent %s %s.", dct == 1L ? "The" : "All the", what,
                  gate_str);
        else
            pline("%s adjacent %s %s.",
                  dct == 1L ? "One of the" : "Some of the",
                  dct == 1L ? "objects falls" : what, gate_str);
    }

    if (costly && shkp && price) {
        if (ESHK(shkp)->robbed > robbed) {
            You("removed %ld %s worth of goods!", price, currency(price));
            if (cansee(shkp->mx, shkp->my)) {
                if (ESHK(shkp)->customer[0] == 0)
                    (void) strncpy(ESHK(shkp)->customer, plname, PL_NSIZ);
                if (angry)
                    pline("%s is infuriated!", Monnam(shkp));
                else
                {
                    if (iflags.using_gui_sounds)
                    {
                        play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_YOU_THIEF);
                        verbalize("You thief!");
                    }
                    else
                        verbalize("%s, you are a thief!", plname);
                }
                /* should use verbalize here? */
            }
            else
            {
                play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_THIEF);
                You_hear("a scream, \"Thief!\"");
            }
            hot_pursuit(shkp);
            (void) angry_guards(FALSE);
            return;
        }
        if (ESHK(shkp)->debit > debit) {
            long amt = (ESHK(shkp)->debit - debit);
            You("owe %s %ld %s for goods lost.", Monnam(shkp), amt,
                currency(amt));
        }
    }
}

/* NOTE: ship_object assumes otmp was FREED from fobj or invent.
 * <x,y> is the point of drop.  otmp is _not_ an <x,y> resident:
 * otmp is either a kicked, dropped, or thrown object.
 */
boolean
ship_object(otmp, x, y, shop_floor_obj)
xchar x, y;
struct obj *otmp;
boolean shop_floor_obj;
{
    schar toloc;
    xchar ox, oy;
    coord cc;
    struct obj *obj;
    struct trap *t;
    boolean nodrop, unpaid, container, impact = FALSE;
    long n = 0L;

    if (!otmp)
        return FALSE;
    if ((toloc = down_gate(x, y)) == MIGR_NOWHERE)
        return FALSE;
    drop_to(&cc, toloc);
    if (!cc.y)
        return FALSE;

    /* objects other than attached iron ball always fall down ladder,
       but have a chance of staying otherwise */
    nodrop = (otmp == uball) || (otmp == uchain)
             || (toloc != MIGR_LADDER_UP && rn2(3));

    container = Has_contents(otmp);
    unpaid = is_unpaid(otmp);

    if (OBJ_AT(x, y)) {
        for (obj = level.objects[x][y]; obj; obj = obj->nexthere)
            if (obj != otmp)
                n += obj->quan;
        if (n)
            impact = TRUE;
    }
    /* boulders never fall through trap doors, but they might knock
       other things down before plugging the hole */
    if (otmp->otyp == BOULDER && ((t = t_at(x, y)) != 0)
        && is_hole(t->ttyp)) {
        if (impact)
            impact_drop(otmp, x, y, 0);
        return FALSE; /* let caller finish the drop */
    }

    if (cansee(x, y))
        otransit_msg(otmp, nodrop, n);

    if (nodrop) {
        if (impact)
            impact_drop(otmp, x, y, 0);
        return FALSE;
    }

    if (unpaid || shop_floor_obj) {
        if (unpaid) {
            (void) stolen_value(otmp, u.ux, u.uy, TRUE, FALSE);
        } else {
            ox = otmp->ox;
            oy = otmp->oy;
            (void) stolen_value(
                otmp, ox, oy,
                (costly_spot(u.ux, u.uy)
                 && index(u.urooms, *in_rooms(ox, oy, SHOPBASE))),
                FALSE);
        }
        /* set otmp->no_charge to 0 */
        if (container)
            picked_container(otmp); /* happens to do the right thing */
        if (otmp->oclass != COIN_CLASS)
            otmp->no_charge = 0;
    }

    if (otmp->owornmask)
        remove_worn_item(otmp, TRUE);

    /* some things break rather than ship */
    if (breaktest(otmp)) {
        const char *result;

        if (is_fragile(otmp)
            || otmp->otyp == EXPENSIVE_CAMERA)
        {
            play_sfx_sound_at_location(SFX_MUFFLED_CRASH, x, y);
            result = "crash";
            You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a muffled %s.", result);
            if (otmp->otyp == MIRROR || otmp->otyp == MAGIC_MIRROR)
                change_luck(-2, TRUE);
        } else {
            /* penalty for breaking eggs laid by you */
            play_sfx_sound_at_location(SFX_MUFFLED_SPLAT, x, y);
            result = "splat";
            You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a muffled %s.", result);
            if (otmp->otyp == EGG && (otmp->speflags & SPEFLAGS_YOURS) && otmp->corpsenm >= LOW_PM)
                change_luck(-1* (int)min(otmp->quan, 5L), TRUE);
        }
        obj_extract_self(otmp);
        obfree(otmp, (struct obj *) 0);
        return TRUE;
    }

    add_to_migration(otmp);
    otmp->ox = cc.x;
    otmp->oy = cc.y;
    otmp->owornmask = (long) toloc;
    /* boulder from rolling boulder trap, no longer part of the trap */
    if (otmp->otyp == BOULDER)
        otmp->otrapped = 0;

    if (impact) {
        /* the objs impacted may be in a shop other than
         * the one in which the hero is located.  another
         * check for a shk is made in impact_drop.  it is, e.g.,
         * possible to kick/throw an object belonging to one
         * shop into another shop through a gap in the wall,
         * and cause objects belonging to the other shop to
         * fall down a trap door--thereby getting two shopkeepers
         * angry at the hero in one shot.
         */
        impact_drop(otmp, x, y, 0);
        newsym(x, y);
    }
    return TRUE;
}

void
obj_delivery(near_hero)
boolean near_hero;
{
    register struct obj *otmp, *otmp2;
    register int nx, ny;
    int where;
    boolean nobreak, noscatter;

    for (otmp = migrating_objs; otmp; otmp = otmp2) {
        otmp2 = otmp->nobj;
        if (otmp->ox != u.uz.dnum || otmp->oy != u.uz.dlevel)
            continue;

        where = (int) (otmp->owornmask & 0x7fffL); /* destination code */
        if ((where & MIGR_TO_SPECIES) != 0)
            continue;

        nobreak = (where & MIGR_NOBREAK) != 0;
        noscatter = (where & MIGR_WITH_HERO) != 0;
        where &= ~(MIGR_NOBREAK | MIGR_NOSCATTER);

        if (!near_hero ^ (where == MIGR_WITH_HERO))
            continue;

        obj_extract_self(otmp);
        otmp->owornmask = 0L;

        switch (where) {
        case MIGR_STAIRS_UP:
            nx = xupstair, ny = yupstair;
            break;
        case MIGR_LADDER_UP:
            nx = xupladder, ny = yupladder;
            break;
        case MIGR_SSTAIRS:
            nx = sstairs.sx, ny = sstairs.sy;
            break;
        case MIGR_WITH_HERO:
            nx = u.ux, ny = u.uy;
            break;
        default:
        case MIGR_RANDOM:
            nx = ny = 0;
            break;
        }
        if (nx > 0) {
            place_object(otmp, nx, ny);
            if (!nobreak && !IS_SOFT(levl[nx][ny].typ)) {
                if (where == MIGR_WITH_HERO) {
                    if (breaks(otmp, nx, ny))
                        continue;
                } else if (breaktest(otmp)) {
                    /* assume it broke before player arrived, no messages */
                    delobj(otmp);
                    continue;
                }
            }
            stackobj(otmp);
            if (!noscatter)
                (void) scatter(nx, ny, rnd(2), 0, otmp);
            else
                newsym(nx, ny);
        } else { /* random location */
            /* set dummy coordinates because there's no
               current position for rloco() to update */
            otmp->ox = otmp->oy = 0;
            if (rloco(otmp) && !nobreak && breaktest(otmp)) {
                /* assume it broke before player arrived, no messages */
                delobj(otmp);
            }
        }
    }
}

void
deliver_obj_to_mon(mtmp, cnt, deliverflags)
int cnt;
struct monst *mtmp;
unsigned long deliverflags;
{
    struct obj *otmp, *otmp2;
    int where, maxobj = 1;
    boolean at_crime_scene = In_mines(&u.uz);

    if ((deliverflags & DF_RANDOM) && cnt > 1)
        maxobj = rnd(cnt);
    else if (deliverflags & DF_ALL)
        maxobj = 0;
    else
        maxobj = 1;

    cnt = 0;
    for (otmp = migrating_objs; otmp; otmp = otmp2) {
        otmp2 = otmp->nobj;
        where = (int) (otmp->owornmask & 0x7fffL); /* destination code */
        if ((where & MIGR_TO_SPECIES) == 0)
            continue;

        if ((mtmp->data->mflags2 & otmp->corpsenm) != 0) {
            obj_extract_self(otmp);
            otmp->owornmask = 0L;
            otmp->ox = otmp->oy = 0;

            /* special treatment for orcs and their kind */
            if ((otmp->corpsenm & M2_ORC) != 0 && has_oname(otmp)) {
                if (!has_mname(mtmp)) {
                    if (at_crime_scene || !rn2(2))
                        mtmp = christen_orc(mtmp,
                                            at_crime_scene ? ONAME(otmp)
                                                           : (char *) 0,
                                            /* bought the stolen goods */
                                            " the Fence");
                }
                free_oname(otmp);
            }
            otmp->corpsenm = 0;
            (void) add_to_minv(mtmp, otmp);
            cnt++;
            if (maxobj && cnt >= maxobj)
                break;
            /* getting here implies DF_ALL */
        }
    }
}

STATIC_OVL void
otransit_msg(otmp, nodrop, num)
register struct obj *otmp;
register boolean nodrop;
long num;
{
    char *optr = 0, obuf[BUFSZ], xbuf[BUFSZ];

    if (otmp->otyp == CORPSE) {
        /* Tobjnam() calls xname() and would yield "The corpse";
           we want more specific "The newt corpse" or "Medusa's corpse" */
        optr = upstart(corpse_xname(otmp, (char *) 0, CXN_PFX_THE));
    } else {
        optr = Tobjnam(otmp, (char *) 0);
    }
    Strcpy(obuf, optr);

    if (num) { /* means: other objects are impacted */
        /* 3.6.2: use a separate buffer for the suffix to avoid risk of
           overrunning obuf[] (let pline() handle truncation if necessary) */
        Sprintf(xbuf, " %s %s object%s", otense(otmp, "hit"),
                (num == 1L) ? "another" : "other", (num > 1L) ? "s" : "");
        if (nodrop)
            Sprintf(eos(xbuf), ".");
        else
            Sprintf(eos(xbuf), " and %s %s.", otense(otmp, "fall"), gate_str);
        pline("%s%s", obuf, xbuf);
    } else if (!nodrop)
        pline("%s %s %s.", obuf, otense(otmp, "fall"), gate_str);
}

/* migration destination for objects which fall down to next level */
schar
down_gate(x, y)
xchar x, y;
{
    struct trap *ttmp;

    gate_str = 0;
    /* this matches the player restriction in goto_level() */
    if (on_level(&u.uz, &qstart_level) && !ok_to_quest())
        return MIGR_NOWHERE;

    if ((xdnstair == x && ydnstair == y)
        || (sstairs.sx == x && sstairs.sy == y && !sstairs.up)) {
        gate_str = "down the stairs";
        return (xdnstair == x && ydnstair == y) ? MIGR_STAIRS_UP
                                                : MIGR_SSTAIRS;
    }
    if (xdnladder == x && ydnladder == y) {
        gate_str = "down the ladder";
        return MIGR_LADDER_UP;
    }

    if (((ttmp = t_at(x, y)) != 0 && ttmp->tseen)
        && is_hole(ttmp->ttyp)) {
        gate_str = (ttmp->ttyp == TRAPDOOR) ? "through the trap door"
                                            : "through the hole";
        return MIGR_RANDOM;
    }
    return MIGR_NOWHERE;
}

/*dokick.c*/
