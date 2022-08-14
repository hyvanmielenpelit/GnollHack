/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    monmove.c    $NHDT-Date: 1557094802 2019/05/05 22:20:02 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.113 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "mfndpos.h"
#include <limits.h> /* for SHRT_MAX */

extern boolean notonhead;

STATIC_DCL void FDECL(watch_on_duty, (struct monst *));
STATIC_DCL int FDECL(disturb, (struct monst *));
STATIC_DCL void FDECL(release_hero, (struct monst *));
STATIC_DCL void FDECL(distfleeck, (struct monst *, int *, int *, int *));
STATIC_DCL int FDECL(m_arrival, (struct monst *));
STATIC_DCL boolean FDECL(stuff_prevents_passage, (struct monst *));
STATIC_DCL int FDECL(vamp_shift, (struct monst *, struct permonst *, BOOLEAN_P));

/* True if mtmp died */
boolean
mb_trapped(mtmp)
struct monst *mtmp;
{
    boolean spef_on = FALSE;
    if (flags.verbose)
    {
        if (cansee(mtmp->mx, mtmp->my) && !Unaware)
        {
            play_special_effect_at(SPECIAL_EFFECT_SMALL_FIERY_EXPLOSION, 0, mtmp->mx, mtmp->my, FALSE);
            play_sfx_sound_at_location_with_minimum_volume(SFX_EXPLOSION_FIERY, mtmp->mx, mtmp->my, 0.15);
            special_effect_wait_until_action(0);
            spef_on = TRUE;
            pline("KABOOM!!  You see a door explode.");
        }
        else if (!Deaf)
        {
            play_sfx_sound_at_location_with_minimum_volume(SFX_EXPLOSION_FIERY, mtmp->mx, mtmp->my, 0.15);
            You_hear("a distant explosion.");
        }
    }
    wake_nearto(mtmp->mx, mtmp->my, 7 * 7);
    increase_mon_property(mtmp, STUNNED, 5 + rnd(10));
    deduct_monster_hp(mtmp, adjust_damage(rnd(15), (struct monst*)0, mtmp, AD_PHYS, ADFLAGS_NONE));
    if (DEADMONSTER(mtmp)) 
    {
        mondied(mtmp);

        if(spef_on)
           special_effect_wait_until_end(0);

        if (!DEADMONSTER(mtmp)) /* lifesaved */
            return FALSE;
        else
            return TRUE;
    }

    if (spef_on)
        special_effect_wait_until_end(0);

    return FALSE;
}

/* check whether a monster is carrying a locking/unlocking tool */
boolean
monhaskey(mon, for_unlocking)
struct monst *mon;
boolean for_unlocking; /* true => credit card ok, false => not ok */
{
    if (for_unlocking && m_carrying(mon, CREDIT_CARD))
        return TRUE;
    return m_carrying(mon, SKELETON_KEY) || m_carrying(mon, LOCK_PICK);
}

void
mon_yells(mon, shout, verb, adverb, usethe)
struct monst *mon;
const char *shout;
const char* verb;
const char* adverb;
boolean usethe;
{
    if (Deaf)
    {
        if (canspotmon(mon))
        {
            /* Sidenote on "A watchman angrily waves her arms!"
             * Female being called watchman is correct (career name).
             */
            char adjbuf[BUFSIZ] = "";
            if (adverb && strcmp(adverb, "") != 0)
                Sprintf(adjbuf, "%s ", adverb);
            
            pline("%s %s%s %s %s!",
                usethe ? Monnam(mon) : Amonnam(mon),
                adjbuf,
                nohands(mon->data) ? "shakes" : "waves",
                mhis(mon),
                nohands(mon->data) ? (has_head(mon->data) ? mbodypart(mon, HEAD) : "body") : makeplural(mbodypart(mon, ARM))
            );
        }

    } 
    else 
    {
        if (canspotmon(mon))
            pline("%s %ss:", usethe ? Monnam(mon) : Amonnam(mon), verb);
        else
            You_hear("someone %s:", verb);
        verbalize1(shout);
    }
}


void
check_mon_talk(mon)
struct monst* mon;
{
    if (!mon || DEADMONSTER(mon) || !mon_can_move(mon) || is_tame(mon) || is_fleeing(mon))
        return;

    if (makes_sound_randomly(mon->data) && !rn2(10))
    {
        play_simple_monster_sound(mon, MONSTER_SOUND_TYPE_RANDOM_SOUND);
        //if (canseemon(mon))
        //    pline("%s laughs.", Monnam(mon));
        return;
    }

    if (!is_speaking_monster(mon->data) || mindless(mon->data)
        || is_stunned(mon) || is_confused(mon))
        return;

    boolean mon_talked = FALSE;

    if (!context.mon_talking)
    {
        if (!mon->notalktimer)
        {
            boolean speaks = FALSE;
            int dist = distmin(mon->mx, mon->my, u.ux, u.uy);
            if (dist <= 1)
                speaks = TRUE;
            else if (dist == 2)
                speaks = !rn2(3);
            else if (dist == 3)
                speaks = !rn2(6);
            else
                speaks = !rn2(3*(dist-1));

            boolean special_talk = !!(mon->data->mflags6 & M6_PEACEFUL_SPECIAL_SHOUT);
            if(m_canseeu(mon) && dist <= (special_talk ? 8 : 4) && speaks)
            { 
                /* Normal peaceful monster talk */
                if(is_peaceful(mon) && (special_talk || (!is_undead(mon->data) && !is_demon(mon->data) && !In_endgame(&u.uz) && !Is_sanctum(&u.uz)
                    && !mon->isshk && !mon->isgd && !mon->ispriest && !mon->issmith && !mon->isnpc && !is_watch(mon->data) && !is_mercenary(mon->data)
                    && !is_lord(mon->data) && !is_prince(mon->data) && mon->m_lev < 10
                    && !(mon->iswiz || mon->data == &mons[PM_MEDUSA] || (mon->data->geno & G_UNIQ) != 0
                        || mon->data->msound == MS_NEMESIS || mon->data->msound == MS_LEADER || mon->data->msound == MS_ORACLE
                        || mon->data->msound == MS_GUARDIAN || mon->data->msound == MS_BRIBE
                        || mon->data == &mons[PM_VLAD_THE_IMPALER] || mon->data == &mons[PM_ALEAX]
                        || (mon->data == &mons[PM_ORACLE]))))
                    )
                {
                    char yellbuf[BUFSZ];
                    int cnt = count_sellable_items(mon);
                    if (special_talk && (!cnt  || dist > 4 || rn2(4)))
                    {
                        if (mon->mnum == PM_PRISONER)
                        {
                            strcpy(yellbuf, "Please help me, adventurer!");
                            mon_yells(mon, yellbuf, "yell", "desperately", TRUE);
                        }
                        mon_talked = TRUE;
                    }
                    else if(cnt > 0)
                    {
                        char itembuf[BUFSZ];
                        char someitembuf[BUFSZ];
                        struct obj* otmp;
                        if (cnt == 1 && !iflags.using_gui_sounds && (otmp = get_first_sellable_item(mon)) != 0)
                        {
                            strcpy(itembuf, otmp->quan == 1 ? acxname(otmp) : cxname(otmp));
                            strcpy(someitembuf, itembuf);
                        }
                        else if (cnt == 1)
                        {
                            strcpy(itembuf, "item");
                            strcpy(someitembuf, "an item");
                        }
                        else
                        {
                            strcpy(itembuf, "items");
                            strcpy(someitembuf, "some items");
                        }

                        if (canseemon(mon))
                            talkeff(mon->mx, mon->my);
                        
                        switch (mon->talkstate_item_trading)
                        {
                        case 0:
                            play_monster_item_trading_line(mon, cnt == 1 ? MONSTER_ITEM_TRADING_LINE_TRADING_HELLO_HOW_ARE_YOU_I_HAVE_AN_ITEM_FOR_SALE : MONSTER_ITEM_TRADING_LINE_TRADING_HELLO_HOW_ARE_YOU_I_HAVE_SOME_ITEMS_FOR_SALE);
                            Sprintf(yellbuf, "Hello, adventurer! How are you? I have %s for sale.", someitembuf);
                            mon_yells(mon, yellbuf, "say", "politely", TRUE);
                            break;
                        case 1:
                            play_monster_item_trading_line(mon, cnt == 1 ? MONSTER_ITEM_TRADING_LINE_TRADING_HELLO_AGAIN_I_STILL_HAVE_AN_ITEM_FOR_SALE : MONSTER_ITEM_TRADING_LINE_TRADING_HELLO_AGAIN_I_STILL_HAVE_SOME_ITEMS_FOR_SALE);
                            Sprintf(yellbuf, "Hello again, I still have %s for sale.", someitembuf);
                            mon_yells(mon, yellbuf, "say", "politely", TRUE);
                            break;
                        case 2:
                            play_monster_item_trading_line(mon, cnt == 1 ? MONSTER_ITEM_TRADING_LINE_TRADING_HOW_IS_YOUR_ADVENTURING_GOING_WOULD_YOU_PERHAPS_HAVE_NOW_INTEREST_IN_BUYING_AN_ITEM : MONSTER_ITEM_TRADING_LINE_TRADING_HOW_IS_YOUR_ADVENTURING_GOING_WOULD_YOU_PERHAPS_HAVE_NOW_INTEREST_IN_BUYING_SOME_ITEMS);
                            Sprintf(yellbuf, "How is your adventuring going? Would you perhaps have now interest in buying %s?", someitembuf);
                            mon_yells(mon, yellbuf, "say", "inquisitively", TRUE);
                            break;
                        case 3:
                            play_monster_item_trading_line(mon, cnt == 1 ? MONSTER_ITEM_TRADING_LINE_TRADING_HELLO_THERE_AGAIN_ANY_INTEREST_IN_MY_ITEM : MONSTER_ITEM_TRADING_LINE_TRADING_HELLO_THERE_AGAIN_ANY_INTEREST_IN_MY_ITEMS);
                            Sprintf(yellbuf, "Hello there again! Any interest in my %s?", itembuf);
                            mon_yells(mon, yellbuf, "say", "politely", TRUE);
                            break;
                        default:
                            break;
                        }
                        mon_talked = TRUE;
                    }
                }
            }
        }
    }

    if (mon_talked)
    {
        if (mon->talkstate_item_trading == 3)
            mon->talkstate_item_trading--;
        else
            mon->talkstate_item_trading++;

        mon->notalktimer = 100 + rnd(200);
        context.mon_talking = TRUE;
    }
}


STATIC_OVL void
watch_on_duty(mtmp)
register struct monst *mtmp;
{
    int x, y;

    if (is_peaceful(mtmp) && in_town(u.ux + u.dx, u.uy + u.dy)
        && !m_cannotsenseu(mtmp) && m_canseeu(mtmp) && !rn2(3)) {
        if (picking_lock(&x, &y) && IS_DOOR(levl[x][y].typ)
            && (levl[x][y].doormask & D_LOCKED)) {
            if (couldsee(mtmp->mx, mtmp->my)) {
                if (levl[x][y].looted & D_WARNED) {
                    context.global_minimum_volume = 0.25f;
                    play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_HALT_THIEF_YOURE_UNDER_ARREST);
                    context.global_minimum_volume = 0.0f;
                    mon_yells(mtmp, "Halt, thief!  You're under arrest!", "yell", "angrily", FALSE);
                    (void) angry_guards(!!Deaf);
                } else {
                    context.global_minimum_volume = 0.25f;
                    play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_HEY_STOP_PICKING_THAT_LOCK);
                    context.global_minimum_volume = 0.0f;
                    mon_yells(mtmp, "Hey, stop picking that lock!", "yell", "angrily", FALSE);
                    levl[x][y].looted |= D_WARNED;
                }
                stop_occupation();
            }
        } else if (is_digging()) {
            /* chewing, wand/spell of digging are checked elsewhere */
            watch_dig(mtmp, context.digging.pos.x, context.digging.pos.y,
                      FALSE);
        }
    }
}

int
dochugw(mtmp)
register struct monst *mtmp;
{
    int x = mtmp->mx, y = mtmp->my;

    boolean couldspotmon = canspotmon(mtmp);
    boolean already_saw_mon = !occupation ? 0 : couldspotmon;

    int rd = dochug(mtmp);

    check_boss_fight(mtmp);

    int x2 = mtmp->mx, y2 = mtmp->my;
    /* a similar check is in monster_nearby() in hack.c */
    /* check whether hero notices monster and stops current activity */
    if (occupation && !rd && /*!Confusion &&*/ ((!is_peaceful(mtmp) && !noattacks(mtmp->data)) || Hallucination)
        /* it's close enough to be a threat */
        && distu(x2, y2) <= (M_SHOOT_RANGE + 1) * (M_SHOOT_RANGE + 1)
        /* and either couldn't see it before, or it was too far away */
        && (!already_saw_mon || !couldsee(x, y)
            || distu(x, y) > (M_SHOOT_RANGE + 1) * (M_SHOOT_RANGE + 1))
        /* can see it now, or sense it and would normally see it */
        && canspotmon(mtmp) && couldsee(x2, y2)
        && mon_can_move(mtmp)
        && !onscary(u.ux, u.uy, mtmp))
        stop_occupation();

    return rd;
}

boolean
onscary(x, y, mtmp)
int x, y;
struct monst *mtmp;
{
    /* creatures who are directly resistant to magical scaring:
     * Rodney, lawful minions, Angels, the Riders, shopkeepers
     * inside their own shop, priests inside their own temple */
    if (mtmp->iswiz || is_lminion(mtmp) || mtmp->data == &mons[PM_ANGEL] || (mtmp->data->geno & G_UNIQ)
        || is_rider(mtmp->data)
        || (mtmp->isshk && inhishop(mtmp))
        || (mtmp->ispriest && inhistemple(mtmp))
        || (mtmp->issmith && inhissmithy(mtmp))
        || (mtmp->isnpc && in_his_npc_room(mtmp))
        )
        return FALSE;

    /* <0,0> is used by musical scaring to check for the above;
     * it doesn't care about scrolls or engravings or dungeon branch */
    if (x == 0 && y == 0)
        return TRUE;

    /* should this still be true for defiled/molochian altars? */
    if (IS_ALTAR(levl[x][y].typ)
        && (mtmp->data->mlet == S_VAMPIRE || is_vampshifter(mtmp)))
        return TRUE;

    /* the scare monster scroll doesn't have any of the below
     * restrictions, being its own source of power */
    if (sobj_at(SCR_SCARE_MONSTER, x, y) && !(mtmp->data->geno & G_UNIQ))
        return TRUE;

    /*
     * Creatures who don't (or can't) fear a written Elbereth:
     * all the above plus shopkeepers (even if poly'd into non-human),
     * vault guards (also even if poly'd), blind or peaceful monsters,
     * humans and elves, and minotaurs.
     *
     * If the player isn't actually on the square OR the player's image
     * isn't displaced to the square, no protection is being granted.
     *
     * Elbereth doesn't work in Gehennom, the Elemental Planes, or the
     * Astral Plane; the influence of the Valar only reaches so far.
     */
    boolean res = (sengr_at(Elbereth_word, x, y, TRUE)
            && ((u.ux == x && u.uy == y)
                || (Displaced && mtmp->mux == x && mtmp->muy == y))
            && !(mtmp->isshk || mtmp->isgd || is_blinded(mtmp)
                 || is_peaceful(mtmp) || mtmp->data->mlet == S_HUMAN
                 || ignores_Elbereth(mtmp->data)
                 || Inhell || In_endgame(&u.uz)));

    return res;
}

boolean
onnopickup(x, y, mtmp)
int x, y;
struct monst* mtmp;
{
    if (onscary(x, y, mtmp))
        return TRUE;

    return (((sengr_at(Gilthoniel_word, x, y, TRUE) && !Inhell) || (sengr_at(Morgoth_word, x, y, TRUE) && Inhell))
        && !(mtmp->isshk || mtmp->isgd || mtmp->iswiz)
        );
}


/* regenerate lost hit points */
void
monster_regeneration_and_timeout(mon, digest_meal)
struct monst *mon;
boolean digest_meal;
{
    int roundstofull = 
        has_divine_regeneration(mon) ? max(1, min(mon->mhpmax / 16, 10)) :
        has_rapidest_regeneration(mon) ? max(1, min(mon->mhpmax / 8, 20)) :
        has_rapider_regeneration(mon) ? max(1, min(mon->mhpmax / 4, 40)) :
        has_rapid_regeneration(mon) ? max(1, min(mon->mhpmax / 2, 80)) :
        has_regeneration(mon) ? max(1, min(mon->mhpmax, 160)) :
        320;
    int fixedhpperround = mon->mhpmax / roundstofull;
    int fractional_hp = (10000 * (mon->mhpmax % roundstofull)) / roundstofull;

    if (is_mummy_rotted(mon))
    {
        if (has_divine_regeneration(mon))
        {
            fixedhpperround = 4;
            fractional_hp = 0;
        }
        else if (has_rapidest_regeneration(mon))
        {
            fixedhpperround = 3;
            fractional_hp = 0;
        }
        else if (has_rapider_regeneration(mon))
        {
            fixedhpperround = 2;
            fractional_hp = 0;
        }
        else if (has_rapid_regeneration(mon))
        {
            fixedhpperround = 1;
            fractional_hp = 0;
        }
        else if (has_regeneration(mon))
        {
            fixedhpperround = 0;
            fractional_hp = 0;
            return;
        }
        else
        {
            roundstofull = 900;
            fixedhpperround = -mon->mhpmax / roundstofull;
            fractional_hp = -(10000 * (mon->mhpmax % roundstofull)) / roundstofull;
        }
    }

    if (mon->mhpmax > 0 && mon->mhp < mon->mhpmax)
    {
        mon->mhp += fixedhpperround;
        mon->mhp_fraction += fractional_hp;
        if (mon->mhp_fraction >= 10000 || mon->mhp_fraction < 0)
        {
            int added_hp = (mon->mhp_fraction / 10000) + (mon->mhp_fraction < 0 ? -1 : 0);
            mon->mhp += added_hp;
            mon->mhp_fraction -= 10000 * added_hp;
        }

        if (mon->mhp >= mon->mhpmax)
        {
            mon->mhp = mon->mhpmax;
            mon->mhp_fraction = 0;
        }
    }

    if (DEADMONSTER(mon))
    {
        if (!resists_sickness(mon))
        {
            if (canseemon(mon))
            {
                play_simple_monster_sound(mon, MONSTER_SOUND_TYPE_DEATH);
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s dies of %s mummy rot!", Monnam(mon), mhis(mon));
            }
            mon->mhp = 0;
            mondied(mon);
            if (mon == u.ustuck)
                u.ustuck = 0;
            if (is_tame(mon) && !canspotmon(mon))
                You_ex(ATR_NONE, CLR_MSG_WARNING, "have a peculiarly sad feeling for a moment, then it passes.");

            return;
        }
    }

    /* Spec_used */
    /* Counters using energy regeneration */
    int enregmult = mon_spec_cooldown_divisor(mon);
    if (mon->mspec_used > enregmult)
        mon->mspec_used -= enregmult;
    else
        mon->mspec_used = 0;

    if (mon->mmagespell_used > enregmult)
        mon->mmagespell_used -= enregmult;
    else
        mon->mmagespell_used = 0;

    if (mon->mmageintermediate_used > enregmult)
        mon->mmageintermediate_used -= enregmult;
    else
        mon->mmageintermediate_used = 0;

    if (mon->mmageultimate_used > enregmult)
        mon->mmageultimate_used -= enregmult;
    else
        mon->mmageultimate_used = 0;

    if (mon->mclericspell_used > enregmult)
        mon->mclericspell_used -= enregmult;
    else
        mon->mclericspell_used = 0;

    if (mon->mclericintermediate_used > enregmult)
        mon->mclericintermediate_used -= enregmult;
    else
        mon->mclericintermediate_used = 0;

    if (mon->mclericultimate_used > enregmult)
        mon->mclericultimate_used -= enregmult;
    else
        mon->mclericultimate_used = 0;

    /* Counters not using energy regeneration */
    if (mon->mdemonsummon_used)
        mon->mdemonsummon_used--;

    if (mon->mspecialsummon_used)
        mon->mspecialsummon_used--;

    if (mon->mspecialsummon2_used)
        mon->mspecialsummon2_used--;

    if (digest_meal)
    {
        if (mon->meating)
        {
            mon->meating--;
            if (mon->meating <= 0)
                finish_meating(mon);
        }
    }
}

/*
 * Possibly awaken the given monster.  Return a 1 if the monster has been
 * jolted awake.
 */
STATIC_OVL int
disturb(mtmp)
register struct monst *mtmp;
{
    /*
     * + Ettins are hard to surprise.
     * + Nymphs, jabberwocks, and leprechauns do not easily wake up.
     *
     * Wake up if:
     *  in direct LOS                                           AND
     *  within 10 squares                                       AND
     *  not stealthy or (mon is an ettin and 9/10)              AND
     *  (mon is not a nymph, jabberwock, or leprechaun) or 1/50 AND
     *  Aggravate or mon is (dog or human) or
     *      (1/7 and mon is not mimicing furniture or object)
     */
    if (couldsee(mtmp->mx, mtmp->my) && distu(mtmp->mx, mtmp->my) <= 100
        && (!Stealth) // || (mtmp->data == &mons[PM_ETTIN] && rn2(10)))
        && (!(mtmp->data->mlet == S_NYMPH
              || mtmp->data == &mons[PM_JABBERWOCK]
#if 0 /* DEFERRED */
              || mtmp->data == &mons[PM_VORPAL_JABBERWOCK]
#endif
              || mtmp->data->mlet == S_LEPRECHAUN) || !rn2(50))
        && (Aggravate_monster
            || (mtmp->data->mlet == S_DOG || mtmp->data->mlet == S_HUMAN)
            || (!rn2(7) && M_AP_TYPE(mtmp) != M_AP_FURNITURE
                && M_AP_TYPE(mtmp) != M_AP_OBJECT))) {
        mtmp->msleeping = 0;
        refresh_m_tile_gui_info(mtmp, TRUE);
        return 1;
    }
    return 0;
}

/* ungrab/expel held/swallowed hero */
STATIC_OVL void
release_hero(mon)
struct monst *mon;
{
    if (mon == u.ustuck) {
        if (u.uswallow) {
            expels(mon, mon->data, TRUE);
        } else if (!sticks(youmonst.data)) {
            unstuck(mon); /* let go */
            You("get released!");
        }
    }
}

#define flees_light(mon) ((mon)->data == &mons[PM_GREMLIN]     \
                          && (uwep && (artifact_light(uwep) || obj_shines_magical_light(uwep) || has_obj_mythic_magical_light(uwep)) && uwep->lamplit))

/* we could include this in the above macro, but probably overkill/overhead */
/*      && (!(which_armor((mon), W_ARMC) != 0                               */
/*            && which_armor((mon), W_ARMH) != 0))                          */

/* monster begins fleeing for the specified time, 0 means untimed flee
 * if first, only adds fleetime if monster isn't already fleeing
 * if fleemsg, prints a message about new flight, otherwise, caller should */
void
monflee(mtmp, fleetime, first, fleemsg)
struct monst *mtmp;
int fleetime;
boolean first;
boolean fleemsg;
{
    if (!mtmp)
        return;

    /* shouldn't happen; maybe warrants impossible()? */
    if (DEADMONSTER(mtmp))
        return;

    if (mtmp == u.ustuck)
        release_hero(mtmp); /* expels/unstuck */

    if (!first || !mtmp->mflee)
    {
        /* don't lose untimed scare */
        if (!fleetime)
            mtmp->mflee_timer = 0;
        else if (!mtmp->mflee || mtmp->mflee_timer)
        {
            fleetime += (int) mtmp->mflee_timer;
            /* ensure monster flees long enough to visibly stop fighting */
            if (fleetime == 1)
                fleetime++;
            mtmp->mflee_timer = (short)min(fleetime, SHRT_MAX);
        }
        if (!is_fleeing(mtmp) && fleemsg && canseemon(mtmp)
            && M_AP_TYPE(mtmp) != M_AP_FURNITURE
            && M_AP_TYPE(mtmp) != M_AP_OBJECT) 
        {
            /* unfortunately we can't distinguish between temporary
               sleep and temporary paralysis, so both conditions
               receive the same alternate message */
            /* Added more color below --JG */
            if (!mon_can_move(mtmp) || !mtmp->data->mmove)
            {
                if(is_sleeping(mtmp) && !is_paralyzed(mtmp))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems to flinch.", Adjmonnam(mtmp, "sleeping"));
                else if (is_paralyzed(mtmp) && mtmp->mcanmove)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems to flinch.", Adjmonnam(mtmp, "paralyzed"));
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems to flinch.", Adjmonnam(mtmp, "immobile"));
            } 
            else if (flees_light(mtmp)) {
                if (rn2(10) || Deaf)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flees from the painful light of %s.",
                          Monnam(mtmp), bare_artifactname(uwep));
                else
                    verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Bright light!");
            } else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s turns to flee.", Monnam(mtmp));
        }
        mtmp->mflee = 1;
    }
    /* ignore recently-stepped spaces when made to flee */
    memset(mtmp->mtrack, 0, sizeof(mtmp->mtrack));
}


void
make_mon_fearful(mtmp, fleetime)
struct monst* mtmp;
int fleetime;
{
    /* shouldn't happen; maybe warrants impossible()? */
    if (DEADMONSTER(mtmp))
        return;

    if (mtmp == u.ustuck)
        release_hero(mtmp); /* expels/unstuck */

    if (!fleetime)
        set_mon_property_verbosely(mtmp, FEARFUL, -1);
    else
    {
        if (get_mon_property(mtmp, FEARFUL) + fleetime == 1)
            fleetime++;

        increase_mon_property_verbosely(mtmp, FEARFUL, fleetime);
    }
    memset(mtmp->mtrack, 0, sizeof(mtmp->mtrack));
    refresh_m_tile_gui_info(mtmp, TRUE);
}




STATIC_OVL void
distfleeck(mtmp, inrange, nearby, scared)
register struct monst *mtmp;
int *inrange, *nearby, *scared;
{
    int seescaryx, seescaryy;
    boolean sawscary = FALSE, bravegremlin = (rn2(5) == 0);

    *inrange = (dist2(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy)
                <= (U_THREAT_RANGE * U_THREAT_RANGE));
    *nearby = *inrange && monnear(mtmp, mtmp->mux, mtmp->muy);

    /* Note: if your image is displaced, the monster sees the Elbereth
     * at your displaced position, thus never attacking your displaced
     * position, but possibly attacking you by accident.  If you are
     * invisible, it sees the Elbereth at your real position, thus never
     * running into you by accident but possibly attacking the spot
     * where it guesses you are.
     */
    if (m_cannotsenseu(mtmp)) {
        seescaryx = mtmp->mux;
        seescaryy = mtmp->muy;
    } else {
        seescaryx = u.ux;
        seescaryy = u.uy;
    }

    sawscary = onscary(seescaryx, seescaryy, mtmp);
    if (*nearby && (sawscary
                    || (flees_light(mtmp) && !bravegremlin)
                    || (!is_peaceful(mtmp) && in_your_sanctuary(mtmp, 0, 0)))) {
        *scared = 1;
        monflee(mtmp, rnd(rn2(7) ? 10 : 100), TRUE, TRUE);
    } else
        *scared = 0;
}

#undef flees_light

/* perform a special one-time action for a monster; returns -1 if nothing
   special happened, 0 if monster uses up its turn, 1 if monster is killed */
STATIC_OVL int
m_arrival(mon)
struct monst *mon;
{
    mon->mstrategy &= ~STRAT_ARRIVE; /* always reset */

    return -1;
}

/* returns 1 if monster died moving, 0 otherwise */
/* The whole dochugw/m_move/distfleeck/mfndpos section is serious spaghetti
 * code. --KAA
 */
int
dochug(mtmp)
register struct monst *mtmp;
{
    register struct permonst *mdat;
    register int tmp = 0;
    int inrange, nearby, scared;

    /*  Pre-movement adjustments
     */

    mdat = mtmp->data;

    if (mtmp->mstrategy & STRAT_ARRIVE) 
    {
        int res = m_arrival(mtmp);
        if (res >= 0)
            return res;
    }

    /* check for waitmask status change */
    if ((mtmp->mstrategy & STRAT_WAITFORU)
        && (m_canseeu(mtmp) || mtmp->mhp < mtmp->mhpmax))
        mtmp->mstrategy &= ~STRAT_WAITFORU;

    /* update quest status flags */
    quest_stat_check(mtmp);

    if ((!mon_can_move(mtmp) && !is_sleeping(mtmp)) || (mtmp->mstrategy & STRAT_WAITMASK)) 
    {
        if (Hallucination)
            newsym(mtmp->mx, mtmp->my);
        if (mon_can_move(mtmp) && (mtmp->mstrategy & STRAT_CLOSE)
            && monnear(mtmp, u.ux, u.uy))
            quest_talk(mtmp, TRUE); /* give the leaders a chance to speak */
        return 0;             /* other frozen monsters can't do anything */
    }

    /* there is a chance we will wake it */
    if ((mtmp->msleeping && !disturb(mtmp)) || !mon_can_move(mtmp))
    {
        if (Hallucination)
            newsym(mtmp->mx, mtmp->my);

        check_boss_fight(mtmp);
        return 0;
    }

    /* not frozen or sleeping: wipe out texts written in the dust */
    struct engr* ep = engr_at(mtmp->mx, mtmp->my);
    if(ep && !(ep->engr_type == ENGR_HEADSTONE || ep->engr_type == ENGR_SIGNPOST || (ep->engr_flags & ENGR_FLAGS_NON_SMUDGING) || sengr_at(Gilthoniel_word, mtmp->mx, mtmp->my, TRUE) || sengr_at(Morgoth_word, mtmp->mx, mtmp->my, TRUE)))
        wipe_engr_at(mtmp->mx, mtmp->my, 1, FALSE);

    /* some monsters teleport */
    if (is_fleeing(mtmp) && !rn2(40) && has_teleportation(mtmp) && !mtmp->iswiz
        && !level.flags.noteleport)
    {
        (void) rloc2(mtmp, TRUE, TRUE);
        check_boss_fight(mtmp);
        return 0;
    }
    if (mdat->msound == MS_SHRIEK && !um_dist(mtmp->mx, mtmp->my, 1))
        m_respond(mtmp);

    if (mdat == &mons[PM_MEDUSA] && couldsee(mtmp->mx, mtmp->my))
        m_respond(mtmp);

    if (DEADMONSTER(mtmp))
        return 1; /* m_respond gaze can kill medusa */

    /* fleeing monsters might regain courage */
    if (is_fleeing(mtmp) && !mtmp->mflee_timer && mtmp->mhp == mtmp->mhpmax
        && !rn2(25))
        mtmp->mflee = 0;

    /* cease conflict-induced swallow/grab if conflict has ended */
    if (mtmp == u.ustuck && is_peaceful(mtmp) && !is_confused(mtmp) && !is_crazed(mtmp) && !Conflict)
    {
        release_hero(mtmp);
        return 0; /* uses up monster's turn */
    }

    set_apparxy(mtmp);
    /* Must be done after you move and before the monster does.  The
     * set_apparxy() call in m_move() doesn't suffice since the variables
     * inrange, etc. all depend on stuff set by set_apparxy().
     */

    /* Monsters that want to acquire things */
    /* may teleport, so do it before inrange is set */
    if (is_teleport_heal_caster(mtmp->data) && !is_peaceful(mtmp))
        (void) tactics(mtmp);

    check_boss_fight(mtmp);

    /* check distance and scariness of attacks */
    distfleeck(mtmp, &inrange, &nearby, &scared);

    /* special speeches for quest monsters */
    (void)quest_talk(mtmp, !!nearby);

    if (find_defensive(mtmp)) 
    {
        if (use_defensive(mtmp) != 0)
            return 1;
    } 
    else if (find_misc(mtmp))
    {
        if (use_misc(mtmp) != 0)
            return 1;
    }

#if 0
    /* Demonic Blackmail! */
    if (nearby && mdat->msound == MS_BRIBE && is_peaceful(mtmp) && !is_silenced(mtmp) && !is_tame(mtmp)
        && !u.uswallow)
    {
        if (mtmp->mux != u.ux || mtmp->muy != u.uy) 
        {
            pline("%s whispers at thin air.",
                  cansee(mtmp->mux, mtmp->muy) ? Monnam(mtmp) : "It");

            if (is_demon(youmonst.data)) 
            {
                /* "Good hunting, brother" */
                if (!tele_restrict(mtmp))
                    (void)rloc2(mtmp, TRUE, TRUE);
            }
            else 
            {
                set_mon_property(mtmp, INVISIBILITY, 0);

                /* Why?  For the same reason in real demon talk */
                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_GET_ANGRY);
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s gets angry!", Amonnam(mtmp));
                mtmp->mpeaceful = 0;
                newsym(mtmp->mx, mtmp->my);
                set_mhostility(mtmp);
                update_game_music();
                /* since no way is an image going to pay it off */
            }
        }
        else if (demon_talk(mtmp))
            return 1; /* you paid it off */
    }
#endif

    /* the watch will look around and see if you are up to no good :-) */
    if (is_watch(mdat))
    {
        watch_on_duty(mtmp);

    }
    else if (is_tentacled_one(mdat) && !rn2(20))
    {
        struct monst *m2, *nmon = (struct monst *) 0;

        if (canseemon(mtmp))
            pline("%s concentrates.", Monnam(mtmp));
        if (distu(mtmp->mx, mtmp->my) > TELEPATHY_RANGE * TELEPATHY_RANGE)
        {
            play_sfx_sound(SFX_FAINT_WAVE_OF_PSYCHIC_ENERGY);
            You_ex(ATR_NONE, CLR_MSG_SPELL, "sense a faint wave of psychic energy.");
            goto toofar;
        }
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "A wave of psychic energy pours over you!");
        if (is_peaceful(mtmp)
            && !(is_crazed(mtmp) || (Conflict && !check_ability_resistance_success(mtmp, A_WIS, 0))))
        {
            play_sfx_sound(SFX_FAINT_WAVE_OF_PSYCHIC_ENERGY);
            pline("It feels quite soothing.");
        } 
        else if (!u.uinvulnerable && !Invulnerable && !Mind_shielding) 
        {
            play_sfx_sound(SFX_WAVE_OF_PSYCHIC_ENERGY);
            register boolean m_sen = sensemon(mtmp);
            if (m_sen || ((Blind_telepat || Unblind_telepat) && rn2(2)) || !rn2(10)) 
            {
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "It locks on to your %s!",
                      m_sen ? "telepathy" : (Blind_telepat || Unblind_telepat) ? "latent telepathy"
                                                          : "mind");
                losehp(adjust_damage(rnd(15), mtmp, &youmonst, AD_PSIO, ADFLAGS_SPELL_DAMAGE), "psychic blast", KILLED_BY_AN);
            }
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            You("are unaffected.");
        }

        for (m2 = fmon; m2; m2 = nmon) 
        {
            nmon = m2->nmon;
            if (DEADMONSTER(m2))
                continue;
            if (is_peaceful(m2) == is_peaceful(mtmp))
                continue;
            if (mindless(m2->data))
                continue;
            if (m2 == mtmp)
                continue;
            if (( (has_telepathy(m2) || (is_blinded(m2) && has_blind_telepathy(m2))) && (rn2(2) || is_blinded(m2)))
                || !rn2(10))
            {
                if (cansee(m2->mx, m2->my))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "It locks on to %s.", mon_nam(m2));
                deduct_monster_hp(m2, adjust_damage(rnd(15), mtmp, m2, AD_PSIO, ADFLAGS_NONE));
                //m2->mhp -= rnd(15);
                if (DEADMONSTER(m2))
                    monkilled(m2, "", AD_DRIN);
                else
                {
                    if (m2->msleeping)
                    {
                        m2->msleeping = 0;
                        refresh_m_tile_gui_info(mtmp, TRUE);
                    }
                }
            }
        }
    }
 toofar:

    /* If monster is nearby you, and has to wield a weapon, do so.   This
     * costs the monster a move, of course.
     */
    if ((!is_peaceful(mtmp) || is_crazed(mtmp) || Conflict) && inrange
        && dist2(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy) <= 8
        && attacktype(mdat, AT_WEAP))
    {
        struct obj *mw_tmp;

        /* The scared check is necessary.  Otherwise a monster that is
         * one square near the player but fleeing into a wall would keep
         * switching between pick-axe and weapon.  If monster is stuck
         * in a trap, prefer ranged weapon (wielding is done in thrwmu).
         * This may cost the monster an attack, but keeps the monster
         * from switching back and forth if carrying both.
         */
        mw_tmp = MON_WEP(mtmp);
        if (!(scared && mw_tmp && is_pick(mw_tmp))
            && mtmp->weapon_strategy == NEED_WEAPON
            && !(mtmp->mtrapped && !nearby && select_rwep(mtmp))) 
        {
            mtmp->weapon_strategy = NEED_HTH_WEAPON;
            if (mon_wield_item(mtmp, FALSE) != 0)
                return 0;
        }
    }

    /*  Now the actual movement phase
     */

    if (!nearby || is_fleeing(mtmp) || scared || is_confused(mtmp) || is_stunned(mtmp)
        || (is_invisible(mtmp) && !rn2(3))
        || (mdat->mlet == S_LEPRECHAUN && !findgold(invent)
            && (findgold(mtmp->minvent) || rn2(2)))
        || (is_wanderer(mdat) && !rn2(4)) || ((Conflict || is_crazed(mtmp)) && !mtmp->iswiz)
        || (is_blinded(mtmp) && !rn2(4)) || is_peaceful(mtmp))
    {
        /* Possibly cast an undirected spell if not attacking you */
        /* note that most of the time castmu() will pick a directed
           spell and do nothing, so the monster moves normally */
        /* arbitrary distance restriction to keep monster far away
           from you from having cast dozens of sticks-to-snakes
           or similar spells by the time you reach it */
        if (dist2(mtmp->mx, mtmp->my, u.ux, u.uy) <= 49
            && !mtmp->mspec_used) 
        {
            struct attack *a;

            for (a = &mdat->mattk[0]; a < &mdat->mattk[NATTK]; a++)
            {
                if (a->aatyp == AT_MAGC && !is_cancelled(mtmp) && !is_silenced(mtmp)
                    && (a->adtyp == AD_SPEL || a->adtyp == AD_CLRC))
                {
                    if (castmu(mtmp, a, FALSE, FALSE))
                    {
                        tmp = 3;
                        break;
                    }
                }
            }
        }

        tmp = m_move(mtmp, 0);
        if (tmp != 2)
            distfleeck(mtmp, &inrange, &nearby, &scared); /* recalc */

        switch (tmp) 
        { /* for pets, cases 0 and 3 are equivalent */
        case 0: /* no movement, but it can still attack you */
        case 3: /* absolutely no movement */
            /* vault guard might have vanished */
            if (mtmp->isgd && (DEADMONSTER(mtmp) || mtmp->mx == 0))
                return 1; /* behave as if it died */
            /* During hallucination, monster appearance should
             * still change - even if it doesn't move.
             */
            if (Hallucination)
                newsym(mtmp->mx, mtmp->my);
            break;
        case 1: /* monster moved */
            /* Maybe it stepped on a trap and fell asleep... */
            if (!mon_can_move(mtmp))
                return 0;
            /* Monsters can move and then shoot on same turn;
               our hero can't.  Is that fair? */
            if (!nearby && (ranged_attk(mdat) || find_offensive(mtmp)))
                break;
            /* engulfer/grabber checks */
            if (mtmp == u.ustuck) {
                /* a monster that's digesting you can move at the
                 * same time -dlc
                 */
                if (u.uswallow)
                    return mattacku(mtmp);
                /* if confused grabber has wandered off, let go */
                if (distu(mtmp->mx, mtmp->my) > 2)
                    unstuck(mtmp);
            }
            check_mon_talk(mtmp);
            return 0;
        case 2: /* monster died */
            return 1;
        }
    }

    /*  Now, attack the player if possible - one attack set per monst
     */

    if (!is_peaceful(mtmp) || is_crazed(mtmp) || (Conflict && !check_ability_resistance_success(mtmp, A_WIS, 0)))
    {
        if (inrange && !noattacks(mdat)
            && (Upolyd ? u.mh : u.uhp) > 0 && !scared && tmp != 3)
            if (mattacku(mtmp))
                return 1; /* monster died (e.g. exploded) */

        if (mtmp->wormno)
            wormhitu(mtmp);
    }

    /* talking for normal monsters */
    check_mon_talk(mtmp);

#if 0
    if (0)
    {
        boolean spoke = quest_talk(mtmp, nearby);
        if (spoke && canseemon(mtmp)) // Deactivated for the time being, as probably does not look good.
            talkeff(mtmp->mx, mtmp->my);
    }
#endif

    /* extra emotional attack for vile monsters */
    if (inrange && mtmp->data->msound == MS_CUSS && !is_peaceful(mtmp)
        && couldsee(mtmp->mx, mtmp->my) && !is_invisible(mtmp) && !rn2(5))
    {
        boolean spoke = cuss(mtmp);
        if(spoke && canseemon(mtmp))
            talkeff(mtmp->mx, mtmp->my);
    }

    return (tmp == 2);
}


void
check_boss_fight(mtmp)
struct monst* mtmp;
{
    /* Trigger a boss fight if you can see the monster */
    if ((windowprocs.wincap2 & WC2_SCREEN_TEXT) && is_boss_monster(mtmp->data) && !mtmp->boss_fight_started && !DEADMONSTER(mtmp) && !is_peaceful(mtmp) && canspotmon(mtmp) && couldsee(mtmp->mx, mtmp->my) && !(mtmp->mstrategy & STRAT_WAITFORU) && !mtmp->msleeping && !(mtmp->mon_flags & MON_FLAGS_CLONED_WIZ))
    {
        mtmp->boss_fight_started = 1;
        newsym(mtmp->mx, mtmp->my);
        flush_screen(1);
        cliparound(mtmp->mx, mtmp->my, 2);
        play_sfx_sound(SFX_BOSS_FIGHT);
        display_screen_text(Monnam(mtmp), (char*)0, (char*)0, SCREEN_TEXT_BOSS_FIGHT, ATR_NONE, NO_COLOR, 1UL);
        cliparound(u.ux, u.uy, 2);
    }
}


STATIC_VAR NEARDATA const char practical[] = { WEAPON_CLASS, ARMOR_CLASS,
                                           GEM_CLASS, FOOD_CLASS, MISCELLANEOUS_CLASS, 0 };
STATIC_VAR NEARDATA const char magical[] = { AMULET_CLASS, POTION_CLASS,
                                         SCROLL_CLASS, WAND_CLASS,
                                         RING_CLASS,   SPBOOK_CLASS, MISCELLANEOUS_CLASS, 0 };
STATIC_VAR NEARDATA const char indigestion[] = { BALL_CLASS, ROCK_CLASS, 0 };
STATIC_VAR NEARDATA const char boulder_class[] = { ROCK_CLASS, 0 };
STATIC_VAR NEARDATA const char gem_class[] = { GEM_CLASS, 0 };

boolean
itsstuck(mtmp)
register struct monst *mtmp;
{
    if (sticks(youmonst.data) && mtmp == u.ustuck && !u.uswallow) {
        pline("%s cannot escape from you!", Monnam(mtmp));
        return TRUE;
    }
    return FALSE;
}

/*
 * should_displace()
 *
 * Displacement of another monster is a last resort and only
 * used on approach. If there are better ways to get to target,
 * those should be used instead. This function does that evaluation.
 */
boolean
should_displace(mtmp, poss, info, cnt, gx, gy)
struct monst *mtmp;
coord *poss; /* coord poss[9] */
long *info;  /* long info[9] */
int cnt;
xchar gx, gy;
{
    int shortest_with_displacing = -1;
    int shortest_without_displacing = -1;
    int count_without_displacing = 0;
    register int i, nx, ny;
    int ndist;

    for (i = 0; i < cnt; i++) {
        nx = poss[i].x;
        ny = poss[i].y;
        ndist = dist2(nx, ny, gx, gy);
        if (MON_AT(nx, ny) && (info[i] & ALLOW_MDISP) && !(info[i] & ALLOW_M)
            && !undesirable_disp(mtmp, nx, ny)) {
            if (shortest_with_displacing == -1
                || (ndist < shortest_with_displacing))
                shortest_with_displacing = ndist;
        } else {
            if ((shortest_without_displacing == -1)
                || (ndist < shortest_without_displacing))
                shortest_without_displacing = ndist;
            count_without_displacing++;
        }
    }
    if (shortest_with_displacing > -1
        && (shortest_with_displacing < shortest_without_displacing
            || !count_without_displacing))
        return TRUE;
    return FALSE;
}

boolean
m_digweapon_check(mtmp, nix, niy)
struct monst *mtmp;
xchar nix,niy;
{
    boolean can_tunnel = 0;
    struct obj *mw_tmp = MON_WEP(mtmp);

    if (!Is_really_rogue_level(&u.uz))
        can_tunnel = tunnels(mtmp->data);

    if (can_tunnel && needspick(mtmp->data) && !mwelded(mw_tmp, mtmp)
        && (may_dig(nix, niy) || closed_door(nix, niy))) {
        /* may_dig() is either IS_STWALL or IS_TREE */
        if (closed_door(nix, niy)) {
            if (!mw_tmp
                || !is_pick(mw_tmp)
                || !is_axe(mw_tmp))
                mtmp->weapon_strategy = NEED_PICK_OR_AXE;
        } else if (IS_TREE(levl[nix][niy].typ)) {
            if (!(mw_tmp = MON_WEP(mtmp)) || !is_axe(mw_tmp))
                mtmp->weapon_strategy = NEED_AXE;
        } else if (IS_STWALL(levl[nix][niy].typ)) {
            if (!(mw_tmp = MON_WEP(mtmp)) || !is_pick(mw_tmp))
                mtmp->weapon_strategy = NEED_PICK_AXE;
        }
        if (mtmp->weapon_strategy >= NEED_PICK_AXE && mon_wield_item(mtmp, FALSE))
            return TRUE;
    }
    return FALSE;
}

/* Return values:
 * 0: did not move, but can still attack and do other stuff.
 * 1: moved, possibly can attack.
 * 2: monster died.
 * 3: did not move, and can't do anything else either.
 */
int
m_move(mtmp, after)
register struct monst *mtmp;
register int after;
{
    register int appr;
    xchar gx, gy, nix, niy, chcnt;
    int chi; /* could be schar except for stupid Sun-2 compiler */
    boolean likegold = 0, likegems = 0, likeobjs = 0, likemagic = 0,
            conceals = 0;
    boolean likerock = 0, can_tunnel = 0;
    boolean can_open = 0, can_unlock = 0, doorbuster = 0;
    boolean uses_items = 0, setlikes = 0;
    boolean avoid = FALSE;
    boolean better_with_displacing = FALSE;
    boolean sawmon = canspotmon(mtmp); /* before it moved */
    struct permonst *ptr;
    struct monst *mtoo;
    schar mmoved = 0; /* not strictly nec.: chi >= 0 will do */
    long info[9];
    long flag;
    int omx = mtmp->mx, omy = mtmp->my;

    if (mtmp->mtrapped)
    {
        int i = mintrap(mtmp);

        if (i >= 2)
        {
            newsym(mtmp->mx, mtmp->my);
            return 2;
        } /* it died */
        if (i == 1)
            return 0; /* still in trap, so didn't move */
    }
    ptr = mtmp->data; /* mintrap() can change mtmp->data -dlc */

    if (mtmp->meating)
    {
        mtmp->meating--;
        if (mtmp->meating <= 0)
            finish_meating(mtmp);
        return 3; /* still eating */
    }
    if (hides_under(ptr) && OBJ_AT(mtmp->mx, mtmp->my) && rn2(10))
        return 0; /* do not leave hiding place */

    set_apparxy(mtmp);
    /* where does mtmp think you are? */
    /* Not necessary if m_move called from this file, but necessary in
     * other calls of m_move (ex. leprechauns dodging)
     */
    if (!Is_really_rogue_level(&u.uz))
        can_tunnel = tunnels(ptr);

    can_open = can_operate_objects(ptr);
    can_unlock = ((can_open && monhaskey(mtmp, TRUE)) || mtmp->iswiz || is_rider(ptr));
    doorbuster = is_giant(ptr);

    if (mtmp->wormno)
        goto not_special;

    /* my dog gets special treatment */
    if (is_tame(mtmp)) 
    {
        mmoved = dog_move(mtmp, after);
        goto postmov;
    }

    /* likewise for shopkeeper */
    if (mtmp->isshk) 
    {
        mmoved = shk_move(mtmp);
        if (mmoved == -2)
            return 2;
        if (mmoved >= 0)
            goto postmov;
        mmoved = 0; /* follow player outside shop */
    }

    /* and for the guard */
    if (mtmp->isgd) 
    {
        mmoved = gd_move(mtmp);
        if (mmoved == -2)
            return 2;
        if (mmoved >= 0)
            goto postmov;
        mmoved = 0;
    }

    /* and the acquisitive monsters get special treatment */
    /* Covetous has been deactivated -- JG */
    if (is_teleport_heal_caster(mtmp->data) && !is_peaceful(mtmp)) // is_covetous(ptr))
    {
        xchar tx = STRAT_GOALX(mtmp->mstrategy),
              ty = STRAT_GOALY(mtmp->mstrategy);
        struct monst *intruder = m_at(tx, ty);
        /*
         * if there's a monster on the object or in possession of it,
         * attack it.
         */
        if ((dist2(mtmp->mx, mtmp->my, tx, ty) < 2) && intruder
            && (intruder != mtmp)) 
        {
            bhitpos.x = tx;
            bhitpos.y = ty;
            notonhead = (intruder->mx != tx || intruder->my != ty);
            if (mattackm(mtmp, intruder) == 2)
                return 2;
            mmoved = 1;
        } else
            mmoved = 0;
        goto postmov;
    }

    /* and for the priest */
    if (mtmp->ispriest) 
    {
        mmoved = pri_move(mtmp);
        if (mmoved == -2)
            return 2;
        if (mmoved >= 0)
            goto postmov;
        mmoved = 0;
    }

    if (mtmp->issmith)
    {
        mmoved = smith_move(mtmp);
        if (mmoved == -2)
            return 2;
        if (mmoved >= 0)
            goto postmov;
        mmoved = 0;
    }

    if (mtmp->isnpc)
    {
        mmoved = npc_move(mtmp);
        if (mmoved == -2)
            return 2;
        if (mmoved >= 0)
            goto postmov;
        mmoved = 0;
    }


    if (ptr == &mons[PM_MAIL_DAEMON])
    {
        if (!Deaf && canseemon(mtmp))
            verbalize("I'm late!");
        mongone(mtmp);
        return 2;
    }

    /* teleport if that lies in our nature */
    if (ptr == &mons[PM_TENGU] && !rn2(5) && !is_cancelled(mtmp)
        && !tele_restrict(mtmp)) 
    {
        if (mtmp->mhp < 7 || is_peaceful(mtmp) || rn2(2))
        {
            (void)rloc2(mtmp, TRUE, TRUE);
        }
        else
            mnexto2(mtmp, TRUE);

        mmoved = 1;
        goto postmov;
    }

 not_special:
    if (u.uswallow && !is_fleeing(mtmp) && u.ustuck != mtmp)
        return 1;
    omx = mtmp->mx;
    omy = mtmp->my;
    gx = mtmp->mux;
    gy = mtmp->muy;
    appr = is_fleeing(mtmp) ? -1 : 1;

    if (is_confused(mtmp) || is_stunned(mtmp) || (u.uswallow && mtmp == u.ustuck))
    {
        appr = 0;
    } 
    else 
    {
        struct obj *lepgold, *ygold;
        boolean should_see = (couldsee(omx, omy)
                              && (levl[gx][gy].lit || !levl[omx][omy].lit)
                              && (dist2(omx, omy, gx, gy) <= 36));

        if ((should_see && m_cannotsenseu(mtmp) && rn2(11))
            || is_obj_mappear(&youmonst,STRANGE_OBJECT)
            || (is_obj_mappear(&youmonst,GOLD_PIECE) && !likes_gold(ptr))
            || (is_peaceful(mtmp) && !mtmp->isshk) /* allow shks to follow */
            || ((mtmp->mnum == PM_STALKER || ptr->mlet == S_BAT
                 || ptr->mlet == S_LIGHT) && !rn2(3)))
            appr = 0;

        if (mtmp->mnum == PM_LEPRECHAUN && (appr == 1)
            && ((lepgold = findgold(mtmp->minvent))
                && (lepgold->quan
                    > ((ygold = findgold(invent)) ? ygold->quan : 0L))))
            appr = -1;

        if (!should_see && can_track(ptr)) 
        {
            register coord *cp;

            cp = gettrack(omx, omy);
            if (cp)
            {
                gx = cp->x;
                gy = cp->y;
            }
        }
    }

    register int pctload = 0;

    if ((!is_peaceful(mtmp) || !rn2(10)) && (!Is_really_rogue_level(&u.uz)))
    {
        int throwrange = throws_rocks(youmonst.data) ? 20 : ACURRSTR / 2 + 1;
        boolean in_line = (lined_up(mtmp, FALSE, 0, FALSE, throwrange) && (distmin(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy) <= throwrange));

        if (appr != 1 || !in_line) 
        {
            /* Monsters in combat won't pick stuff up, avoiding the
             * situation where you toss arrows at it and it has nothing
             * better to do than pick the arrows up.
             */
            pctload =
                (curr_mon_load(mtmp) * 100) / max_mon_load(mtmp);

            /* look for gold or jewels nearby */
            likegold = (likes_gold(ptr) && pctload < 95);
            likegems = (likes_gems(ptr) && pctload < 85);
            uses_items = (!mindless(ptr) && !is_animal(ptr) && pctload < 75);
            likeobjs = (likes_objs(ptr) && pctload < 75);
            likemagic = (likes_magic(ptr) && pctload < 85);
            likerock = (throws_rocks(ptr) && pctload < 50 && !Sokoban);
            conceals = hides_under(ptr);
            setlikes = TRUE;
        }
    }

#define SQSRCHRADIUS 5

    {
        register int minr = SQSRCHRADIUS; /* not too far away */
        register struct obj *otmp;
        register int xx, yy;
        int oomx, oomy, lmx, lmy;

        /* cut down the search radius if it thinks character is closer. */
        if (distmin(mtmp->mux, mtmp->muy, omx, omy) < SQSRCHRADIUS
            && !is_peaceful(mtmp))
            minr--;
        /* guards shouldn't get too distracted */

        if (!is_peaceful(mtmp) && is_mercenary(ptr))
            minr = 1;

        if ((likegold || likegems || likeobjs || likemagic || likerock
             || conceals) && mon_can_reach_floor(mtmp) && (!*in_rooms(omx, omy, SHOPBASE)
                              || (!rn2(25) && !mtmp->isshk)))
        {
 look_for_obj:
            oomx = min(COLNO - 1, omx + minr);
            oomy = min(ROWNO - 1, omy + minr);
            lmx = max(1, omx - minr);
            lmy = max(0, omy - minr);
            for (otmp = fobj; otmp; otmp = otmp->nobj) 
            {
                /* monsters may pick rocks up, but won't go out of their way
                   to grab them; this might hamper sling wielders, but it cuts
                   down on move overhead by filtering out most common item */
                if (is_rock(otmp))
                    continue;
                xx = otmp->ox;
                yy = otmp->oy;
                /* Nymphs take everything.  Most other creatures should not
                 * pick up corpses except as a special case like in
                 * searches_for_item().  We need to do this check in
                 * mpickstuff() as well.
                 */
                if (xx >= lmx && xx <= oomx && yy >= lmy && yy <= oomy) 
                {
                    /* don't get stuck circling around object that's
                       underneath an immobile or hidden monster;
                       paralysis victims excluded */
                    if ((mtoo = m_at(xx, yy)) != 0
                        && (mtoo->msleeping || mtoo->mundetected
                            || (mtoo->mappearance && !mtoo->iswiz)
                            || !mtoo->data->mmove))
                        continue;
                    /* the mfndpos() test for whether to allow a move to a
                       water location accepts flyers, but they can't reach
                       underwater objects, so being able to move to a spot
                       is insufficient for deciding whether to do so */
                    if ((is_pool(xx, yy) && !is_swimmer(ptr))
                        || (is_lava(xx, yy) && !likes_lava(ptr)))
                        continue;

                    if (((likegold && otmp->oclass == COIN_CLASS)
                         || (likeobjs && (index(practical, otmp->oclass))
                             && (otmp->otyp != CORPSE
                                 || (ptr->mlet == S_NYMPH
                                     && !is_rider(&mons[otmp->corpsenm]))))
                         || (likemagic && index(magical, otmp->oclass))
                         || (uses_items && searches_for_item(mtmp, otmp))
                         || (likerock && otmp->otyp == BOULDER)
                         || (likegems && otmp->oclass == GEM_CLASS
                             && objects[otmp->otyp].oc_material != MAT_MINERAL)
                         || (conceals && !cansee(otmp->ox, otmp->oy))
                         || (slurps_items(ptr)
                             && !index(indigestion, otmp->oclass)
                             && !(otmp->otyp == CORPSE
                                  && touch_petrifies(&mons[otmp->corpsenm]))))
                        && touch_artifact(otmp, mtmp))
                    {
                        if (can_carry(mtmp, otmp) > 0
                            && (throws_rocks(ptr) || !sobj_at(BOULDER, xx, yy))
                            && (!is_unicorn(ptr)
                                || objects[otmp->otyp].oc_material == MAT_GEMSTONE)
                            /* Don't get stuck circling an Elbereth or Gilthoniel */
                            && !onnopickup(xx, yy, mtmp) && !is_obj_no_pickup(otmp)) 
                        {
                            minr = distmin(omx, omy, xx, yy);
                            oomx = min(COLNO - 1, omx + minr);
                            oomy = min(ROWNO - 1, omy + minr);
                            lmx = max(1, omx - minr);
                            lmy = max(0, omy - minr);
                            gx = otmp->ox;
                            gy = otmp->oy;
                            if (gx == omx && gy == omy) {
                                mmoved = 3; /* actually unnecessary */
                                goto postmov;
                            }
                        }
                    }
                }
            }
        } 
        else if (likegold)
        {
            /* don't try to pick up anything else, but use the same loop */
            uses_items = 0;
            likegems = likeobjs = likemagic = likerock = conceals = 0;
            goto look_for_obj;
        }

        if (minr < SQSRCHRADIUS && appr == -1) 
        {
            if (distmin(omx, omy, mtmp->mux, mtmp->muy) <= 3) 
            {
                gx = mtmp->mux;
                gy = mtmp->muy;
            } else
                appr = 1;
        }
    }

    /* don't tunnel if hostile and close enough to prefer a weapon */
    if (can_tunnel && needspick(ptr)
        && ((!is_peaceful(mtmp) || Conflict || is_crazed(mtmp))
            && dist2(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy) <= 8))
        can_tunnel = FALSE;

    nix = omx;
    niy = omy;
    flag = 0L;
    if (is_peaceful(mtmp) && !(is_crazed(mtmp) || (Conflict && !check_ability_resistance_success(mtmp, A_WIS, 0))))
        flag |= (ALLOW_SANCT | ALLOW_SSM);
    else
    {
        flag |= ALLOW_U;
        flag |= ALLOW_TM;
    }
    if (is_minion(ptr) || is_rider(ptr))
        flag |= ALLOW_SANCT;
    /* unicorn may not be able to avoid hero on a noteleport level */
    if (is_unicorn(ptr) && !level.flags.noteleport)
        flag |= NOTONL;
    if (passes_walls(ptr))
        flag |= (ALLOW_WALL | ALLOW_ROCK);
    if (has_pitwalk(mtmp->data))
        flag |= ALLOW_PITS;
    if (passes_bars(ptr))
        flag |= ALLOW_BARS;
    if (can_tunnel)
        flag |= ALLOW_DIG;
    if (is_human(ptr) || ignores_Elbereth(ptr))
        flag |= ALLOW_SSM;
    if ((is_undead(ptr) && ptr->mlet != S_GHOST) || is_vampshifter(mtmp))
        flag |= NOGARLIC;
    if (throws_rocks(ptr))
        flag |= ALLOW_ROCK;
    if (can_open)
        flag |= OPENDOOR;
    if (can_unlock)
        flag |= UNLOCKDOOR;
    if (doorbuster)
        flag |= BUSTDOOR;
    {
        register int i, j, nx, ny, nearer;
        int jcnt, cnt;
        int ndist, nidist;
        register coord *mtrk;
        coord poss[9];

        cnt = mfndpos(mtmp, poss, info, flag);
        chcnt = 0;
        jcnt = min(MTSZ, cnt - 1);
        chi = -1;
        nidist = dist2(nix, niy, gx, gy);
        /* allow monsters be shortsighted on some levels for balance */
        if (!is_peaceful(mtmp) && level.flags.shortsighted
            && nidist > (couldsee(nix, niy) ? 144 : 36) && appr == 1)
            appr = 0;
        if (is_unicorn(ptr) && level.flags.noteleport)
        {
            /* on noteleport levels, perhaps we cannot avoid hero */
            for (i = 0; i < cnt; i++)
                if (!(info[i] & NOTONL))
                    avoid = TRUE;
        }
        better_with_displacing =
            should_displace(mtmp, poss, info, cnt, gx, gy);
        for (i = 0; i < cnt; i++)
        {
            if (avoid && (info[i] & NOTONL))
                continue;
            nx = poss[i].x;
            ny = poss[i].y;

            if (MON_AT(nx, ny) && (info[i] & ALLOW_MDISP)
                && !(info[i] & ALLOW_M) && !better_with_displacing)
                continue;
            if (appr != 0) {
                mtrk = &mtmp->mtrack[0];
                for (j = 0; j < jcnt; mtrk++, j++)
                    if (nx == mtrk->x && ny == mtrk->y)
                        if (rn2(4 * (cnt - j)))
                            goto nxti;
            }

            nearer = ((ndist = dist2(nx, ny, gx, gy)) < nidist);

            if ((appr == 1 && nearer) || (appr == -1 && !nearer)
                || (!appr && !rn2(++chcnt)) || !mmoved) 
            {
                nix = nx;
                niy = ny;
                nidist = ndist;
                chi = i;
                mmoved = 1;
            }
 nxti:
            ;
        }
    }

    if (mmoved)
    {
        register int j;

        if (mmoved == 1 && (u.ux != nix || u.uy != niy) && itsstuck(mtmp))
            return 3;

        if (mmoved == 1 && m_digweapon_check(mtmp, nix,niy))
            return 3;

        /* If ALLOW_U is set, either it's trying to attack you, or it
         * thinks it is.  In either case, attack this spot in preference to
         * all others.
         */
        /* Actually, this whole section of code doesn't work as you'd expect.
         * Most attacks are handled in dochug().  It calls distfleeck(), which
         * among other things sets nearby if the monster is near you--and if
         * nearby is set, we never call m_move unless it is a special case
         * (confused, stun, etc.)  The effect is that this ALLOW_U (and
         * mfndpos) has no effect for normal attacks, though it lets a
         * confused monster attack you by accident.
         */
        if (info[chi] & ALLOW_U)
        {
            nix = mtmp->mux;
            niy = mtmp->muy;
        }
        if (nix == u.ux && niy == u.uy) 
        {
            mtmp->mux = u.ux;
            mtmp->muy = u.uy;
            return 0;
        }
        /* The monster may attack another based on 1 of 2 conditions:
         * 1 - It may be confused.
         * 2 - It may mistake the monster for your (displaced) image.
         * Pets get taken care of above and shouldn't reach this code.
         * Conflict gets handled even farther away (movemon()).
         */
        if ((info[chi] & (ALLOW_M | ALLOW_TM)) || (nix == mtmp->mux && niy == mtmp->muy))
        {
            struct monst* mtmp2 = m_at(nix, niy);
            int mstatus;

            if(mtmp2)
            {
                bhitpos.x = nix;
                bhitpos.y = niy;
                notonhead = mtmp2 && (nix != mtmp2->mx || niy != mtmp2->my);
                /* note: mstatus returns 0 if mtmp2 is nonexistent */
                mstatus = mattackm(mtmp, mtmp2);

                if (mstatus & MM_AGR_DIED) /* aggressor died */
                    return 2;

                if ((mstatus & MM_HIT) && !(mstatus & MM_DEF_DIED) && rn2(4)
                    && mtmp2->movement >= NORMAL_SPEED) 
                {
                    mtmp2->movement -= NORMAL_SPEED;
                    bhitpos.x = mtmp->mx;
                    bhitpos.y = mtmp->my;
                    notonhead = 0;
                    mstatus = mattackm(mtmp2, mtmp); /* return attack */
                    if (mstatus & MM_DEF_DIED)
                        return 2;
                }
            }
            return 3;
        }

        if ((info[chi] & ALLOW_MDISP)) 
        {
            struct monst *mtmp2;
            int mstatus;

            mtmp2 = m_at(nix, niy);
            mstatus = mdisplacem(mtmp, mtmp2, FALSE);
            if ((mstatus & MM_AGR_DIED) || (mstatus & MM_DEF_DIED))
                return 2;
            if (mstatus & MM_HIT)
                return 1;
            return 3;
        }

        if (!m_in_out_region(mtmp, nix, niy))
            return 3;

        remove_monster(omx, omy);
        place_monster(mtmp, nix, niy);
        play_movement_sound(mtmp, CLIMBING_TYPE_NONE);
        update_m_facing(mtmp, nix - omx, FALSE);

        for (j = MTSZ - 1; j > 0; j--)
            mtmp->mtrack[j] = mtmp->mtrack[j - 1];
        mtmp->mtrack[0].x = omx;
        mtmp->mtrack[0].y = omy;
        /* Place a segment at the old position. */
        if (mtmp->wormno)
            worm_move(mtmp);
    }
    else
    {
        if (is_unicorn(ptr) && rn2(2) && !tele_restrict(mtmp)) 
        {
            (void)rloc2(mtmp, TRUE, TRUE);
            return 1;
        }
        if (mtmp->wormno)
            worm_nomove(mtmp);
    }

 postmov:
    if (mmoved == 1 || mmoved == 3) 
    {
        boolean canseeit = cansee(mtmp->mx, mtmp->my);

        if (mmoved == 1) 
        {
            /* normal monster move will already have <nix,niy>,
               but pet dog_move() with 'goto postmov' won't */
            nix = mtmp->mx, niy = mtmp->my;
            /* sequencing issue:  when monster movement decides that a
               monster can move to a door location, it moves the monster
               there before dealing with the door rather than after;
               so a vampire/bat that is going to shift to fog cloud and
               pass under the door is already there but transformation
               into fog form--and its message, when in sight--has not
               happened yet; we have to move monster back to previous
               location before performing the vamp_shift() to make the
               message happen at right time, then back to the door again
               [if we did the shift above, before moving the monster,
               we would need to duplicate it in dog_move()...] */
            if (is_vampshifter(mtmp) && !amorphous(mtmp->data)
                && IS_DOOR(levl[nix][niy].typ)
                && ((levl[nix][niy].doormask & (D_LOCKED | D_CLOSED)) != 0)
                && can_fog(mtmp)) 
            {
                if (sawmon) 
                {
                    remove_monster(nix, niy);
                    place_monster(mtmp, omx, omy);
                    newsym(nix, niy), newsym(omx, omy);
                }

                if (vamp_shift(mtmp, &mons[PM_FOG_CLOUD], sawmon)) 
                {
                    ptr = mtmp->data; /* update cached value */
                }

                if (sawmon) 
                {
                    remove_monster(omx, omy);
                    place_monster(mtmp, nix, niy);
                    newsym(omx, omy), newsym(nix, niy);
                }
            }

            newsym(omx, omy); /* update the old position */
            if (mintrap(mtmp) >= 2) 
            {
                if (mtmp->mx)
                    newsym(mtmp->mx, mtmp->my);
                return 2; /* it died */
            }
            ptr = mtmp->data; /* in case mintrap() caused polymorph */

            /* open a door, or crash through it, if 'mtmp' can */
            if (IS_DOOR(levl[mtmp->mx][mtmp->my].typ)
                && !passes_walls(ptr) /* doesn't need to open doors */
                && !can_tunnel) {     /* taken care of below */
                struct rm *here = &levl[mtmp->mx][mtmp->my];
                boolean btrapped = (here->doormask & D_TRAPPED) != 0,
                        observeit = canseeit && canspotmon(mtmp);

                /* if mon has MKoT, disarm door trap; no message given */
                if (btrapped && has_magic_key(mtmp)) 
                {
                    /* BUG: this lets a vampire or blob or a doorbuster
                       holding the Key disarm the trap even though it isn't
                       using that Key when squeezing under or smashing the
                       door.  Not significant enough to worry about; perhaps
                       the Key's magic is more powerful for monsters? */
                    here->doormask &= ~D_TRAPPED;
                    btrapped = FALSE;
                }
                if ((here->doormask & (D_LOCKED | D_CLOSED)) != 0
                    && amorphous(ptr))
                {
                    if (flags.verbose && canseemon(mtmp))
                        pline("%s %s under the door.", Monnam(mtmp),
                              (ptr == &mons[PM_FOG_CLOUD]
                               || ptr->mlet == S_LIGHT) ? "flows" : "oozes");
                } 
                else if ((here->doormask & D_LOCKED) != 0 && can_unlock) 
                {
                    if (btrapped) 
                    {
                        here->doormask &= ~D_MASK;
                        if (is_door_destroyed_by_booby_trap_at_ptr(here))
                        {
                            here->doormask |= D_NODOOR;
                        }
                        else
                        {
                            here->doormask |= D_ISOPEN;
                        }
                        unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my); /* vision */
                        newsym(mtmp->mx, mtmp->my);
                        if (mb_trapped(mtmp))
                            return 2;
                    }
                    else
                    {
                        if (flags.verbose)
                        {
                            if (observeit)
                                pline("%s unlocks and opens a door.",
                                      Monnam(mtmp));
                            else if (canseeit)
                                You_see("a door unlock and open.");
                            else if (!Deaf)
                                You_hear("a door unlock and open.");
                        }
                        here->doormask &= ~D_MASK;
                        here->doormask |= D_ISOPEN;
                        /* newsym(mtmp->mx, mtmp->my); */
                        unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my); /* vision */
                    }
                } 
                else if ((here->doormask & D_CLOSED) != 0 && can_open) 
                {
                    if (btrapped) 
                    {
                        here->doormask &= ~D_MASK;
                        if (is_door_destroyed_by_booby_trap_at_ptr(here))
                        {
                            here->doormask |= D_NODOOR;
                        }
                        else
                        {
                            here->doormask |= D_ISOPEN;
                        }
                        unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my); /* vision */
                        newsym(mtmp->mx, mtmp->my);
                        if (mb_trapped(mtmp))
                            return 2;
                    } 
                    else 
                    {
                        if (flags.verbose) 
                        {
                            play_simple_location_sound(mtmp->mx, mtmp->my, LOCATION_SOUND_TYPE_OPEN);
                            if (observeit)
                                pline("%s opens a door.", Monnam(mtmp));
                            else if (canseeit)
                                You_see("a door open.");
                            else if (!Deaf)
                                You_hear("a door open.");
                        }
                        here->doormask &= ~D_MASK;
                        here->doormask |= D_ISOPEN;
                        /* newsym(mtmp->mx, mtmp->my); */  /* done below */
                        unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my); /* vision */
                    }
                } 
                else if (here->doormask & (D_LOCKED | D_CLOSED))
                {
                    boolean door_intact = TRUE;
                    /* mfndpos guarantees this must be a doorbuster */
                    if (btrapped) 
{
                        here->doormask &= ~D_TRAPPED;
                        if (is_door_destroyed_by_booby_trap_at_ptr(here))
                        {
                            here->doormask &= ~D_MASK;
                            here->doormask |= D_NODOOR;
                            door_intact = FALSE;
                            unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my); /* vision */
                            newsym(mtmp->mx, mtmp->my);
                        }
                        if (mb_trapped(mtmp))
                        {
                            if (*in_rooms(mtmp->mx, mtmp->my, SHOPBASE))
                                add_damage(mtmp->mx, mtmp->my, 0L);
                            return 2;
                        }
                    }

                    if (door_intact && is_door_destroyed_by_monsters_at_ptr(here))
                    {
                        if (flags.verbose)
                        {
                            play_simple_location_sound(mtmp->mx, mtmp->my, LOCATION_SOUND_TYPE_BREAK);
                            if (observeit)
                                pline("%s smashes down a door.",
                                    Monnam(mtmp));
                            else if (canseeit)
                                You_see("a door crash open.");
                            else if (!Deaf)
                                You_hear("a door crash open.");
                        }
                        if ((here->doormask & D_LOCKED) != 0 && !rn2(2))
                        {
                            here->doormask &= ~D_MASK;
                            here->doormask |= D_NODOOR;
                        }
                        else
                        {
                            here->doormask &= ~D_MASK;
                            here->doormask |= D_BROKEN;
                        }
                        /* newsym(mtmp->mx, mtmp->my); */  /* done below */
                        unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my); /* vision */
                        door_intact = FALSE;
                    }
                    /* if it's a shop door, schedule repair */
                    if (!door_intact && *in_rooms(mtmp->mx, mtmp->my, SHOPBASE))
                        add_damage(mtmp->mx, mtmp->my, 0L);
                }
            } 
            else if (levl[mtmp->mx][mtmp->my].typ == IRONBARS) 
            {
                /* 3.6.2: was using may_dig() but it doesn't handle bars */
                if (!(levl[mtmp->mx][mtmp->my].wall_info & W_NONDIGGABLE)
                    && (dmgtype(ptr, AD_RUST) || dmgtype(ptr, AD_CORR))) 
                {
                    if (canseemon(mtmp))
                        pline("%s eats through the iron bars.", Monnam(mtmp));
                    dissolve_bars(mtmp->mx, mtmp->my);
                    return 3;
                }
                else if (flags.verbose && canseemon(mtmp))
                    Norep("%s %s %s the iron bars.", Monnam(mtmp),
                          /* pluralization fakes verb conjugation */
                          makeplural(locomotion(ptr, "pass")),
                          passes_walls(ptr) ? "through" : "between");
            }

            /* possibly dig */
            if (can_tunnel && mdig_tunnel(mtmp))
                return 2; /* mon died (position already updated) */

            /* set also in domove(), hack.c */
            if (u.uswallow && mtmp == u.ustuck
                && (mtmp->mx != omx || mtmp->my != omy))
            {
                /* If the monster moved, then update */
                u.ux0 = u.ux;
                u.uy0 = u.uy;
                u.ux = mtmp->mx;
                u.uy = mtmp->my;
                swallowed(0);
            }
            else
                newsym(mtmp->mx, mtmp->my);
        }

        struct obj* objhere = o_at(mtmp->mx, mtmp->my);
        if (OBJ_AT(mtmp->mx, mtmp->my) && mon_can_move(mtmp) && !mtmp->issummoned && mon_can_reach_floor(mtmp) && !onnopickup(mtmp->mx, mtmp->my, mtmp)
            && !(couldsee(mtmp->mx, mtmp->my) && objhere && objhere->was_thrown)) //Do not pick up ammo or other stuff that the player shoots / throws (this checks just the first item, but that's probably good enough)
        {
            /* recompute the likes tests, in case we polymorphed
             * or if the "likegold" case got taken above */
            if (setlikes) 
            {
                pctload = (curr_mon_load(mtmp) * 100) / max_mon_load(mtmp);

                /* look for gold or jewels nearby */
                likegold = (likes_gold(ptr) && pctload < 95);
                likegems = (likes_gems(ptr) && pctload < 85);
                uses_items =
                    (!mindless(ptr) && !is_animal(ptr) && pctload < 75);
                likeobjs = (likes_objs(ptr) && pctload < 75);
                likemagic = (likes_magic(ptr) && pctload < 85);
                likerock = (throws_rocks(ptr) && pctload < 50 && !Sokoban);
                conceals = hides_under(ptr);
            }

            /* Maybe a rock mole just ate some metal object */
            if (metallivorous(ptr)) 
            {
                if (meatmetal(mtmp) == 2)
                    return 2; /* it died */
            }

            if (g_at(mtmp->mx, mtmp->my) && likegold)
                mpickgold(mtmp);

            /* Maybe a cube ate just about anything */
            if (slurps_items(ptr)) 
            {
                if (meatobj(mtmp) == 2)
                    return 2; /* it died */
            }

            if (!*in_rooms(mtmp->mx, mtmp->my, SHOPBASE) || !rn2(25)) 
            {
                boolean picked = FALSE;

                if (likeobjs)
                    picked |= mpickstuff(mtmp, practical);
                if (likemagic)
                    picked |= mpickstuff(mtmp, magical);
                if (likerock)
                    picked |= mpickstuff(mtmp, boulder_class);
                if (likegems)
                    picked |= mpickstuff(mtmp, gem_class);
                if (uses_items)
                    picked |= mpickstuff(mtmp, (char *) 0);
                if (picked)
                    mmoved = 3;
            }

            if (is_invisible(mtmp)) 
            {
                newsym(mtmp->mx, mtmp->my);
                if (mtmp->wormno)
                    see_wsegs(mtmp);
            }
        }

        if (hides_under(ptr) || ptr->mlet == S_EEL) 
        {
            /* Always set--or reset--mundetected if it's already hidden
               (just in case the object it was hiding under went away);
               usually set mundetected unless monster can't move.  */
            if (mtmp->mundetected
                || (mon_can_move(mtmp) && rn2(5)))
                (void) hideunder(mtmp);
            newsym(mtmp->mx, mtmp->my);
        }

        if (mtmp->isshk) 
        {
            after_shk_move(mtmp);
        }
    }
    check_boss_fight(mtmp);
    return mmoved;
}

void
dissolve_bars(x, y)
register int x, y;
{
    boolean floor_is_room = (Is_special(&u.uz) || *in_rooms(x, y, 0));
    levl[x][y].typ = levl[x][y].floortyp ? levl[x][y].floortyp : floor_is_room ? ROOM : CORR;
    levl[x][y].subtyp = levl[x][y].floorsubtyp ? levl[x][y].floorsubtyp : get_initial_location_subtype(levl[x][y].typ);
    levl[x][y].vartyp = levl[x][y].floorvartyp ? levl[x][y].floorvartyp : get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
    levl[x][y].special_quality = 0;
    levl[x][y].flags = 0;
    newsym(x, y);
}

boolean
closed_door(x, y)
register int x, y;
{
    return (boolean) (IS_DOOR(levl[x][y].typ)
                      && (levl[x][y].doormask & (D_LOCKED | D_CLOSED)));
}

boolean
accessible(x, y)
register int x, y;
{
    int levtyp = levl[x][y].typ;

    /* use underlying terrain in front of closed drawbridge */
    if (levtyp == DRAWBRIDGE_UP)
        levtyp = db_under_typ(levl[x][y].drawbridgemask);

    return (boolean) (ACCESSIBLE(levtyp) && !closed_door(x, y));
}

/* decide where the monster thinks you are standing */
void
set_apparxy(mtmp)
register struct monst *mtmp;
{
    boolean notseen, gotu;
    register int disp, mx = mtmp->mux, my = mtmp->muy;
    long umoney = money_cnt(invent);

    /*
     * do cheapest and/or most likely tests first
     */

    /* pet knows your smell; grabber still has hold of you */
    if (is_tame(mtmp) || mtmp == u.ustuck)
        goto found_you;

    /* monsters which know where you are don't suddenly forget,
       if you haven't moved away */
    if (mx == u.ux && my == u.uy)
        goto found_you;

    notseen = m_cannotsenseu(mtmp);

    /* add cases as required.  eg. Displacement ... */
    if (notseen || Underwater) {
        /* Xorns can smell quantities of valuable metal
            like that in solid gold coins, treat as seen */
        if ((mtmp->data == &mons[PM_XORN]) && umoney && !Underwater)
            disp = 0;
        else
            disp = 1;
    } else if (Displaced) {
        disp = couldsee(mx, my) ? 2 : 1;
    } else
        disp = 0;
    if (!disp)
        goto found_you;

    /* without something like the following, invisibility and displacement
       are too powerful */
    gotu = notseen ? !rn2(3) : Displaced ? !rn2(4) : FALSE;

    if (!gotu) {
        register int try_cnt = 0;

        do {
            if (++try_cnt > 200)
                goto found_you; /* punt */
            mx = u.ux - disp + rn2(2 * disp + 1);
            my = u.uy - disp + rn2(2 * disp + 1);
        } while (!isok(mx, my)
                 || (disp != 2 && mx == mtmp->mx && my == mtmp->my)
                 || ((mx != u.ux || my != u.uy) && !passes_walls(mtmp->data)
                     && !(accessible(mx, my)
                          || (closed_door(mx, my)
                              && (can_ooze(mtmp) || can_fog(mtmp)))))
                 || !couldsee(mx, my));
    } else {
 found_you:
        mx = u.ux;
        my = u.uy;
    }

    mtmp->mux = mx;
    mtmp->muy = my;
}

/*
 * mon-to-mon displacement is a deliberate "get out of my way" act,
 * not an accidental bump, so we don't consider mstun or mconf in
 * undesired_disp().
 *
 * We do consider many other things about the target and its
 * location however.
 */
boolean
undesirable_disp(mtmp, x, y)
struct monst *mtmp;
xchar x, y;
{
    if (!mtmp)
        return TRUE;

    boolean is_pet = (mtmp && is_tame(mtmp) && !mtmp->isminion);
    struct trap *trap = t_at(x, y);

    if (is_pet) {
        /* Pets avoid a trap if you've seen it usually. */
        if (trap && trap->tseen && rn2(40))
            return TRUE;
        /* Pets avoid cursed locations */
        if (cursed_object_at(x, y))
            return TRUE;

    /* Monsters avoid a trap if they've seen that type before */
    } else if (trap && rn2(40)
               && (mtmp->mtrapseen & (1 << (trap->ttyp - 1))) != 0) {
        return TRUE;
    }

    return FALSE;
}

/*
 * Inventory prevents passage under door.
 * Used by can_ooze() and can_fog().
 */
STATIC_OVL boolean
stuff_prevents_passage(mtmp)
struct monst *mtmp;
{
    struct obj *chain, *obj;

    if (mtmp == &youmonst) 
    {
        chain = invent;
    } 
    else 
    {
        chain = mtmp->minvent;
    }

    for (obj = chain; obj; obj = obj->nobj) 
    {
        int typ = obj->otyp;

        if (typ == COIN_CLASS && obj->quan > 100L)
            return TRUE;
        if (obj->oclass != GEM_CLASS && !(typ >= ARROW && typ <= BOOMERANG)
            && !(typ >= DAGGER && typ <= CRYSKNIFE) && typ != SLING
            && !is_cloak(obj) && typ != FEDORA && !is_gloves(obj)
            && typ != LEATHER_JACKET && typ != CREDIT_CARD && !is_shirt(obj)
            && !(typ == CORPSE && verysmall(&mons[obj->corpsenm]))
            && typ != FORTUNE_COOKIE && typ != CANDY_BAR && typ != PANCAKE
            && typ != ELVEN_WAYBREAD && typ != LUMP_OF_ROYAL_JELLY
            && obj->oclass != AMULET_CLASS && obj->oclass != MISCELLANEOUS_CLASS && obj->oclass != RING_CLASS
            && obj->oclass != VENOM_CLASS && typ != SACK && typ != ORIENTAL_SILK_SACK && typ != LEATHER_BAG && typ != BACKPACK
            && typ != EXPENSIVE_HANDBAG
            && typ != BAG_OF_HOLDING && typ != BAG_OF_WIZARDRY && typ != BAG_OF_TREASURE_HAULING && typ != BAG_OF_THE_GLUTTON && typ != BAG_OF_TRICKS
            && !is_candle(obj) && typ != OILSKIN_SACK && typ != LEASH
            && typ != STETHOSCOPE && typ != BLINDFOLD && typ != TOWEL
            && typ != TIN_WHISTLE && typ != MAGIC_WHISTLE
            && typ != MAGIC_MARKER && typ != TIN_OPENER && typ != SKELETON_KEY
            && typ != LOCK_PICK)
            return TRUE;
        if (Is_container(obj) && obj->cobj)
            return TRUE;
    }
    return FALSE;
}

boolean
can_ooze(mtmp)
struct monst *mtmp;
{
    if (!amorphous(mtmp->data) || stuff_prevents_passage(mtmp))
        return FALSE;
    return TRUE;
}

/* monster can change form into a fog if necessary */
boolean
can_fog(mtmp)
struct monst *mtmp;
{
    if (!(mvitals[PM_FOG_CLOUD].mvflags & MV_GENOCIDED) && is_vampshifter(mtmp)
        && !Protection_from_shape_changers && !stuff_prevents_passage(mtmp))
        return TRUE;
    return FALSE;
}

STATIC_OVL int
vamp_shift(mon, ptr, domsg)
struct monst *mon;
struct permonst *ptr;
boolean domsg;
{
    if (!mon)
        return 0;

    int reslt = 0;
    char oldmtype[BUFSZ];

    /* remember current monster type before shapechange */
    Strcpy(oldmtype, domsg ? noname_monnam(mon, ARTICLE_THE) : "");

    if (mon->data == ptr) {
        /* already right shape */
        reslt = 1;
        domsg = FALSE;
    } else if (is_vampshifter(mon)) {
        reslt = newcham(mon, ptr, FALSE, FALSE);
    }

    if (reslt && domsg) {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You %s %s where %s was.",
              !canseemon(mon) ? "now detect" : "observe",
              noname_monnam(mon, ARTICLE_A), oldmtype);
        /* this message is given when it turns into a fog cloud
           in order to move under a closed door */
        display_nhwindow(WIN_MESSAGE, FALSE);
    }

    return reslt;
}

boolean
m_findtravelpath(mon, mode, mon_dx_ptr, mon_dy_ptr, allowflags)
struct monst* mon;
int mode;
xchar* mon_dx_ptr;
xchar* mon_dy_ptr;
long allowflags;
{
    if (!mon || !mon_dx_ptr || !mon_dy_ptr)
        return FALSE;

    xchar target_x = u.ux;
    xchar target_y = u.uy;

    if (isok(mon->yell_x, mon->yell_y))
    {
        target_x = mon->yell_x;
        target_y = mon->yell_y;
    }

    /* if travel to adjacent, reachable location, use normal movement rules */
    if ((mode == TRAVP_TRAVEL || mode == TRAVP_VALID) && mon->mcomingtou
        && distmin(mon->mx, mon->my, target_x, target_y) == 1
        && !(mon->mx != target_x && mon->my != target_y && NODIAG(u.umonnum))) 
    {
        //context.run = 0;
        if (m_test_move(mon, mon->mx, mon->my, target_x - mon->mx, target_y - mon->my, TEST_MOVE, allowflags)) 
        {
            if (mode == TRAVP_TRAVEL) 
            {
                *mon_dx_ptr = target_x - mon->mx;
                *mon_dy_ptr = target_y - mon->my;
                //nomul(0);
                //iflags.travelcc.x = iflags.travelcc.y = 0;
            }
            return TRUE;
        }

        //if (mode == TRAVP_TRAVEL)
         //   context.run = 8;
    }

    if (target_x != mon->mx || target_y != mon->my) 
    {
        xchar travel[COLNO][ROWNO];
        xchar travelstepx[2][COLNO * ROWNO];
        xchar travelstepy[2][COLNO * ROWNO];
        xchar tx, ty, ux, uy;
        int n = 1;      /* max offset in travelsteps */
        int set = 0;    /* two sets current and previous */
        int radius = 1; /* search radius */
        int i;

        /* If guessing, first find an "obvious" goal location.  The obvious
         * goal is the position the player knows of, or might figure out
         * (couldsee) that is closest to the target on a straight path.
         */
        if (mode == TRAVP_GUESS || mode == TRAVP_VALID)
        {
            tx = mon->mx;
            ty = mon->my;
            ux = target_x;
            uy = target_y;
        }
        else 
        {
            tx = target_x;
            ty = target_y;
            ux = mon->mx;
            uy = mon->my;
        }

    noguess:
        (void)memset((genericptr_t)travel, 0, sizeof travel);
        travelstepx[0][0] = tx;
        travelstepy[0][0] = ty;
        const int ordered[] = { 0, 2, 4, 6, 1, 3, 5, 7 };

        while (n != 0)
        {
            int nn = 0;

            for (i = 0; i < n; i++) 
            {
                int dir;
                int x = travelstepx[set][i];
                int y = travelstepy[set][i];
                /* no diagonal movement for grid bugs */
                int dirmax = NODIAG(mon->mnum) ? 4 : 8;
                boolean alreadyrepeated = FALSE;

                for (dir = 0; dir < dirmax; ++dir) 
                {
                    int nx = x + xdir[ordered[dir]];
                    int ny = y + ydir[ordered[dir]];

                    /*
                     * When guessing and trying to travel as close as possible
                     * to an unreachable target space, don't include spaces
                     * that would never be picked as a guessed target in the
                     * travel matrix describing hero-reachable spaces.
                     * This stops travel from getting confused and moving
                     * the hero back and forth in certain degenerate
                     * configurations of sight-blocking obstacles, e.g.
                     *
                     *  T         1. Dig this out and carry enough to not be
                     *   ####       able to squeeze through diagonal gaps.
                     *   #--.---    Stand at @ and target travel at space T.
                     *    @.....
                     *    |.....
                     *
                     *  T         2. couldsee() marks spaces marked a and x
                     *   ####       as eligible guess spaces to move the hero
                     *   a--.---    towards.  Space a is closest to T, so it
                     *    @xxxxx    gets chosen.  Travel system moves @ right
                     *    |xxxxx    to travel to space a.
                     *
                     *  T         3. couldsee() marks spaces marked b, c and x
                     *   ####       as eligible guess spaces to move the hero
                     *   a--c---    towards.  Since findtravelpath() is called
                     *    b@xxxx    repeatedly during travel, it doesn't
                     *    |xxxxx    remember that it wanted to go to space a,
                     *              so in comparing spaces b and c, b is
                     *              chosen, since it seems like the closest
                     *              eligible space to T. Travel system moves @
                     *              left to go to space b.
                     *
                     *            4. Go to 2.
                     *
                     * By limiting the travel matrix here, space a in the
                     * example above is never included in it, preventing
                     * the cycle.
                     */
                    if (!isok(nx, ny)
                        /* || ((mode == TRAVP_GUESS) && !couldsee(nx, ny)) */)
                        continue;

                    if ((!does_pass_walls(mon) && !can_ooze(mon)
                        && closed_door(x, y)) || sobj_at(BOULDER, x, y)
                        || m_test_move(mon, x, y, nx - x, ny - y, TEST_TRAP, allowflags))
                    {
                        /* closed doors and boulders usually
                         * cause a delay, so prefer another path */
                        if (travel[x][y] > radius - 3)
                        {
                            if (!alreadyrepeated) {
                                travelstepx[1 - set][nn] = x;
                                travelstepy[1 - set][nn] = y;
                                /* don't change travel matrix! */
                                nn++;
                                alreadyrepeated = TRUE;
                            }
                            continue;
                        }
                    }
                    if (m_test_move(mon, x, y, nx - x, ny - y, TEST_TRAV, allowflags)
                        /*&& (levl[nx][ny].seenv
                            || (!Blind && couldsee(nx, ny))) */ ) 
                    {
                        if (nx == ux && ny == uy) 
                        {
                            if (mode == TRAVP_TRAVEL || mode == TRAVP_VALID) 
                            {
                                *mon_dx_ptr = x - ux;
                                *mon_dy_ptr = y - uy;
                                if (mode == TRAVP_TRAVEL
                                    && x == target_x && y == target_y) 
                                {
                                    //nomul(0);
                                    /* reset run so domove run checks work */
                                    //context.run = 8;
                                    //iflags.travelcc.x = iflags.travelcc.y = 0;
                                }
                                return TRUE;
                            }
                        }
                        else if (!travel[nx][ny]) 
                        {
                            travelstepx[1 - set][nn] = nx;
                            travelstepy[1 - set][nn] = ny;
                            travel[nx][ny] = radius;
                            nn++;
                        }
                    }
                }
            }

#ifdef DEBUG
            if (iflags.trav_debug) 
            {
                /* Use of warning glyph is arbitrary. It stands out. */
                tmp_at(DISP_ALL, warning_to_glyph(1));
                for (i = 0; i < nn; ++i) 
                {
                    tmp_at(travelstepx[1 - set][i], travelstepy[1 - set][i]);
                }
                adjusted_delay_output();
                tmp_at(DISP_END, 0);
            }
#endif /* DEBUG */

            n = nn;
            set = 1 - set;
            radius++;
        }

        /* if guessing, find best location in travel matrix and go there */
        if (mode == TRAVP_GUESS) 
        {
            int px = tx, py = ty; /* pick location */
            int dist, nxtdist, d2, nd2;

            dist = distmin(ux, uy, tx, ty);
            d2 = dist2(ux, uy, tx, ty);
            for (tx = 1; tx < COLNO; ++tx)
                for (ty = 0; ty < ROWNO; ++ty)
                    if (travel[tx][ty]) 
                    {
                        nxtdist = distmin(ux, uy, tx, ty);
                        if (nxtdist == dist /*&& couldsee(tx, ty) */) 
                        {
                            nd2 = dist2(ux, uy, tx, ty);
                            if (nd2 < d2) 
                            {
                                /* prefer non-zigzag path */
                                px = tx;
                                py = ty;
                                d2 = nd2;
                            }
                        }
                        else if (nxtdist < dist /*&& couldsee(tx, ty)*/) 
                        {
                            px = tx;
                            py = ty;
                            dist = nxtdist;
                            d2 = dist2(ux, uy, tx, ty);
                        }
                    }

            if (px == mon->mx && py == mon->my) 
            {
                /* no guesses, just go in the general direction */
                *mon_dx_ptr = sgn(target_x - mon->mx);
                *mon_dy_ptr = sgn(target_y - mon->my);
                if (m_test_move(mon, mon->mx, mon->my, *mon_dx_ptr, *mon_dy_ptr, TEST_MOVE, allowflags))
                    return TRUE;
                goto found;
            }
#ifdef DEBUG
            if (iflags.trav_debug)
            {
                /* Use of warning glyph is arbitrary. It stands out. */
                tmp_at(DISP_ALL, warning_to_glyph(2));
                tmp_at(px, py);
                adjusted_delay_output();
                tmp_at(DISP_END, 0);
            }
#endif /* DEBUG */
            tx = px;
            ty = py;
            ux = mon->mx;
            uy = mon->my;
            set = 0;
            n = radius = 1;
            mode = TRAVP_TRAVEL;
            goto noguess;
        }
        return FALSE;
    }

found:
    *mon_dx_ptr = 0;
    *mon_dy_ptr = 0;
    return FALSE;
}

boolean
m_test_move(mon, mx, my, dx, dy, mode, allowflags)
struct monst* mon;
xchar mx, my, dx, dy;
int mode;
long allowflags;
{
    coord poss[9];
    long info[9];
    xchar test_x = mx + dx;
    xchar test_y = my + dy;

    if (!isok(test_x, test_y))
        return FALSE;

    allowflags |= ALLOW_U | ALLOW_M | ALLOW_TM; /* Monsters do not block pathing */

    if (mode == TEST_TRAP)
        allowflags |= ALLOW_TRAPS;

    int cnt = mfndpos_xy(mon, mx, my, poss, info, allowflags);
    if (!cnt)
        return FALSE;

    for (int i = 0; i < cnt; i++)
    {
        if (poss[i].x == test_x && poss[i].y == test_y)
            return TRUE;
    }

    return FALSE;
}

/*monmove.c*/
