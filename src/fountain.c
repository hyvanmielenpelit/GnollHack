/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    fountain.c    $NHDT-Date: 1544442711 2018/12/10 11:51:51 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.60 $ */
/*      Copyright Scott R. Turner, srt@ucla, 10/27/86 */
/* GnollHack may be freely redistributed.  See license for details. */

/* Code for drinking from fountains. */

#include "hack.h"

STATIC_DCL void NDECL(dowatersnakes);
STATIC_DCL void NDECL(dowaterdemon);
STATIC_DCL void NDECL(dowaternymph);
STATIC_PTR void FDECL(gush, (int, int, genericptr_t));
STATIC_DCL void NDECL(dofindgem);

/* used when trying to dip in or drink from fountain or sink or pool while
   levitating above it, or when trying to move downwards in that state */
void
floating_above(what)
const char *what;
{
    const char *umsg = "are floating high above the %s.";

    if (u.utrap && (u.utraptype == TT_INFLOOR || u.utraptype == TT_LAVA)) {
        /* when stuck in floor (not possible at fountain or sink location,
           so must be attempting to move down), override the usual message */
        umsg = "are trapped in the %s.";
        what = surface(u.ux, u.uy); /* probably redundant */
    }
    play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
    You_ex(ATR_NONE, CLR_MSG_FAIL, umsg, what);
}

/* Fountain of snakes! */
STATIC_OVL void
dowatersnakes()
{
    register int num = rn1(5, 2);
    struct monst *mtmp;

    if (!(mvitals[PM_WATER_MOCCASIN].mvflags & MV_GONE)) {
        if (!Blind)
            pline("An endless stream of %s pours forth!",
                  Hallucination ? makeplural(rndmonnam(NULL)) : "snakes");
        else
            You_hear("%s hissing!", something);
        while (num-- > 0)
            if ((mtmp = makemon(&mons[PM_WATER_MOCCASIN], u.ux, u.uy,
                                NO_MM_FLAGS)) != 0
                && t_at(mtmp->mx, mtmp->my))
                (void) mintrap(mtmp);
    } else
        pline_The("fountain bubbles furiously for a moment, then calms.");
}

/* Water demon */
STATIC_OVL void
dowaterdemon()
{
    struct monst *mtmp;

    if (!(mvitals[PM_WATER_DEMON].mvflags & MV_GONE)) {
        if ((mtmp = makemon(&mons[PM_WATER_DEMON], u.ux, u.uy,
                            NO_MM_FLAGS)) != 0) {
            if (!Blind)
                You("unleash %s!", a_monnam(mtmp));
            else
                You_feel("the presence of evil.");

            /* Give those on low levels a (slightly) better chance of survival
             */
            if (rnd(100) > (80 + level_difficulty())) {
                pline("Grateful for %s release, %s grants you a wish!",
                      mhis(mtmp), mhe(mtmp));
                /* give a wish and discard the monster (mtmp set to null) */
                mongrantswish(&mtmp);
            } else if (t_at(mtmp->mx, mtmp->my))
                (void) mintrap(mtmp);
        }
    } else
        pline_The("fountain bubbles furiously for a moment, then calms.");
}

/* Water Nymph */
STATIC_OVL void
dowaternymph()
{
    register struct monst *mtmp;

    if (!(mvitals[PM_WATER_NYMPH].mvflags & MV_GONE)
        && (mtmp = makemon(&mons[PM_WATER_NYMPH], u.ux, u.uy,
                           NO_MM_FLAGS)) != 0) {
        if (!Blind)
            You("attract %s!", a_monnam(mtmp));
        else
            You_hear("a seductive voice.");
        mtmp->msleeping = 0;
        if (t_at(mtmp->mx, mtmp->my))
            (void) mintrap(mtmp);
    } else if (!Blind)
        pline("A large bubble rises to the surface and pops.");
    else
        You_hear("a loud pop.");
}

/* Gushing forth along LOS from (u.ux, u.uy) */
void
dogushforth(drinking)
int drinking;
{
    int madepool = 0;

    do_clear_area(u.ux, u.uy, 7, gush, (genericptr_t) &madepool);
    if (!madepool) {
        if (drinking)
            Your("thirst is quenched.");
        else
            pline("Water sprays all over you.");
    }
}

STATIC_PTR void
gush(x, y, poolcnt)
int x, y;
genericptr_t poolcnt;
{
    register struct monst *mtmp;
    register struct trap *ttmp;

    if (((x + y) % 2) || (x == u.ux && y == u.uy)
        || (rn2(1 + distmin(u.ux, u.uy, x, y))) || (levl[x][y].typ != ROOM && levl[x][y].typ != GRASS && levl[x][y].typ != GROUND)
        || (sobj_at(BOULDER, x, y)) || nexttodoor(x, y))
        return;

    if ((ttmp = t_at(x, y)) != 0 && !delfloortrap(ttmp))
        return;

    if (!((*(int *) poolcnt)++))
        pline("Water gushes forth from the overflowing fountain!");

    /* Put a pool at x, y */
    create_simple_location(x, y, POOL, 0, 0, 0, 0, levl[x][y].typ, levl[x][y].subtyp, levl[x][y].vartyp, FALSE);

    /* No kelp! */
    del_engr_at(x, y);
    water_damage_chain(level.objects[x][y], TRUE);

    if ((mtmp = m_at(x, y)) != 0)
        (void) minliquid(mtmp);
    else
        newsym(x, y);
}

/* Find a gem in the sparkling waters. */
STATIC_OVL void
dofindgem()
{
    if (!Blind)
        You("spot a gem in the sparkling waters!");
    else
        You_feel("a gem here!");
    (void) mksobj_at(rnd_class(DILITHIUM_CRYSTAL, LUCKSTONE - 1), u.ux, u.uy,
                     FALSE, FALSE);
    SET_FOUNTAIN_LOOTED(u.ux, u.uy);
    newsym(u.ux, u.uy);
    exercise(A_WIS, TRUE); /* a discovery! */
}

void
dryup(x, y, isyou)
xchar x, y;
boolean isyou;
{
    int ftyp = levl[x][y].subtyp; // (levl[x][y].fountainmask& FOUNTAIN_TYPE_MASK);

    if (IS_FOUNTAIN(levl[x][y].typ)
        && (!rn2(ftyp == FOUNTAIN_MAGIC ? 3 : ftyp == FOUNTAIN_POWER ? 3 : 6) || FOUNTAIN_IS_WARNED(x, y)))
    {
        if (isyou && in_town(x, y) && !FOUNTAIN_IS_WARNED(x, y))
        {
            struct monst *mtmp;

            SET_FOUNTAIN_WARNED(x, y);
            /* Warn about future fountain use. */
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
                if (DEADMONSTER(mtmp))
                    continue;
                if (is_watch(mtmp->data) && couldsee(mtmp->mx, mtmp->my)
                    && is_peaceful(mtmp)) {
                    if (!Deaf) {
                        pline("%s yells:", Amonnam(mtmp));
                        context.global_minimum_volume = 0.25f;
                        play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_HEY_STOP_USING_THAT_FOUNTAIN);
                        context.global_minimum_volume = 0.0f;
                        verbalize_angry1("Hey, stop using that fountain!");
                    } else {
                        pline("%s earnestly %s %s %s!",
                              Amonnam(mtmp),
                              nolimbs(mtmp->data) ? "shakes" : "waves",
                              mhis(mtmp),
                              nolimbs(mtmp->data)
                                      ? mbodypart(mtmp, HEAD)
                                      : makeplural(mbodypart(mtmp, ARM)));
                    }
                    break;
                }
            }
            /* You can see or hear this effect */
            if (!mtmp)
                pline_The1("flow reduces to a trickle.");
            return;
        }
        if (isyou && wizard) {
            if (yn_query("Dry up fountain?") == 'n')
                return;
        }
        /* replace the fountain with ordinary floor */
        play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_DRY_UP);
        create_current_floor_location(x, y, 0, back_to_broken_glyph(x, y), TRUE);
        if (cansee(x, y))
            pline_The1("fountain dries up!");
        /* The location is seen if the hero/monster is invisible
           or felt if the hero is blind. */
        if (isyou && in_town(x, y))
            (void) angry_guards(FALSE);
    }
}

void
drinkfountain()
{
    /* What happens when you drink from a fountain? */
    register boolean mgkftn = (levl[u.ux][u.uy].blessedftn == 1);
    register int fate = rnd(30);

    register int zlevel;
    zlevel = level_difficulty();
    boolean fountain_blessed = levl[u.ux][u.uy].blessedftn;
    int ftyp = levl[u.ux][u.uy].subtyp; // (levl[u.ux][u.uy].fountainmask & FOUNTAIN_TYPE_MASK);

    if (Levitation && !Levitation_control) 
    {
        floating_above("fountain");
        return;
    }

    play_sfx_sound(SFX_QUAFF);

    if (ftyp == FOUNTAIN_HEALING || ftyp == FOUNTAIN_POWER)
    {
        int num = d(2, 6);
        int added_hp = num + ((rnd(6) + 5) * 5 * (Upolyd ? u.mhmax : u.uhpmax)) / 100;
        int added_max = 0;

        if ((Upolyd ? u.mhmax == u.mh : u.uhpmax == u.uhp))
            added_max += d(1, 3);

        if (fountain_blessed)
        {
            added_hp *= 2;
            added_max += d(1, 3);
        }
        healup(added_hp, added_max,
            !!fountain_blessed, !fountain_blessed, FALSE, FALSE, FALSE);

        if (ftyp == FOUNTAIN_HEALING)
        {
            play_sfx_sound(SFX_HEALING);
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "The water makes you feel better.");
            if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
            {
                pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of healing.");
                SET_FOUNTAIN_KNOWN(u.ux, u.uy);
            }
        }

    }
    if (ftyp == FOUNTAIN_MANA || ftyp == FOUNTAIN_POWER)
    {
        int num = d(2, 6);
        int added_mana = num + ((rnd(6) + 5) * 5 * u.uenmax) / 100;
        int added_max_mana = 0;

        int mana_before = u.uen;
        int max_mana_before = u.uenmax;

        if(u.uen == u.uenmax)
            added_max_mana += d(1, 3);

        if (fountain_blessed)
        {
            added_mana *= 2;
            added_max_mana += d(1, 3);
        }
        if (u.ubaseendrain < 0)
        {
            u.ubaseendrain += added_max_mana;
            if (u.ubaseendrain > 0)
            {
                added_max_mana = u.ubaseendrain;
                u.ubaseendrain = 0;
            }
            else
            {
                added_max_mana = 0;
            }
        }
        u.uen += added_mana;
        u.ubaseenmax += added_max_mana;
        updatemaxen();
        if (u.uenmax <= 0)
            u.uenmax = 0;
        if (u.uen > u.uenmax)
            u.uen = u.uenmax;
        else if (u.uen <= 0)
            u.uen = 0;

        int mana_after = u.uen;
        int mana_gain = mana_after - mana_before;
        int max_mana_after = u.uenmax;
        int max_mana_gain = max_mana_after - max_mana_before;
        if (max_mana_gain > 0 && isok(u.ux, u.uy))
        {
            char fbuf[BUFSZ];
            Sprintf(fbuf, "+%d max mana", max_mana_gain);
            display_floating_text(u.ux, u.uy, fbuf, FLOATING_TEXT_ATTRIBUTE_GAIN, ATR_NONE, NO_COLOR, 0UL);
        }
        if (mana_gain > 0 && isok(u.ux, u.uy))
        {
            char fbuf[BUFSZ];
            Sprintf(fbuf, "+%d", mana_gain);
            display_floating_text(u.ux, u.uy, fbuf, FLOATING_TEXT_MANA_GAIN, ATR_NONE, NO_COLOR, 0UL);
        }

        context.botl = 1;

        context.botl = 1;

        if (ftyp == FOUNTAIN_MANA)
        {
            play_sfx_sound(SFX_GAIN_ENERGY);
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "The water makes magical energies course through your body.");
            if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
            {
                pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of mana.");
                SET_FOUNTAIN_KNOWN(u.ux, u.uy);
            }
        }
    }
    if (ftyp == FOUNTAIN_POWER)
    {
        play_sfx_sound(SFX_GAIN_ENERGY);
        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "The water fills your body with new energy.");
        if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
        {
            pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of power.");
            SET_FOUNTAIN_KNOWN(u.ux, u.uy);
        }
    }

    if (ftyp == FOUNTAIN_WATER)
    {
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "tasty water refreshes you.");
        u.uhunger += rnd(10); /* don't choke on water */
        update_hunger_status(FALSE);
        if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
        {
            pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of spring water.");
            SET_FOUNTAIN_KNOWN(u.ux, u.uy);
        }
    }

    if (ftyp == FOUNTAIN_NATURAL)
    {
        pline_The("tasty spring water refreshes you.");
        u.uhunger += rnd(10); /* don't choke on water */
        update_hunger_status(FALSE);
        if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
        {
            pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a natural spring.");
            SET_FOUNTAIN_KNOWN(u.ux, u.uy);
        }
    }

    if (ftyp == FOUNTAIN_POISON)
    {
        play_sfx_sound(SFX_MONSTER_IS_POISONED);
        pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "water tastes foul! It was poisoned!");
        if (Poison_resistance) 
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "However, you are unaffected!");
        }
        else
        {
            losestr(rn1(4, 2));
            losehp(adjust_damage(rnd(10), (struct monst*)0, &youmonst, AD_DRST, ADFLAGS_NONE), "poisonous water", KILLED_BY);
        }
        if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
        {
            pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of poison.");
            SET_FOUNTAIN_KNOWN(u.ux, u.uy);
        }
        standard_hint("You can test fountains by dipping potions into them. Get poison resistance before drinking from unknown fountains.", &u.uhint.poisoned_by_fountain);
    }

    /* Exit if not normal NetHack / magic fountain */
    if (ftyp != FOUNTAIN_MAGIC)
    {
        dryup(u.ux, u.uy, TRUE);
        return;
    }

    /* Normal NetHack magic fountain here */
    if (mgkftn && u.uluck >= 0 && fate >= 10)
    {
        int i, ii, littleluck = (u.uluck < 4);

        play_sfx_sound(SFX_RESTORE_ABILITY);
        pline("Wow!  The water makes you feel great!");
        /* blessed restore ability */
        for (ii = 0; ii < A_MAX; ii++)
            if (ABASE(ii) < AMAX(ii)) {
                ABASE(ii) = AMAX(ii);
                context.botl = 1;
            }
        /* gain ability, blessed if "natural" luck is high */
        i = rn2(A_MAX); /* start at a random attribute */
        for (ii = 0; ii < A_MAX; ii++) {
            if (adjattrib(i, 1, littleluck ? -1 : 0) && littleluck)
                break;
            if (++i >= A_MAX)
                i = 0;
        }
        display_nhwindow(WIN_MESSAGE, FALSE);
        pline("A wisp of vapor escapes the fountain...");
        exercise(A_WIS, TRUE);
        levl[u.ux][u.uy].blessedftn = 0;
        if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
        {
            pline_ex(ATR_NONE, CLR_MSG_HINT, "That was an enchanted magic fountain.");
            SET_FOUNTAIN_KNOWN(u.ux, u.uy);
        }
        return;
    }

    if (fate < 10)
    {
        pline_The("cool draught refreshes you.");
        u.uhunger += rnd(10); /* don't choke on water */
        update_hunger_status(FALSE);
        if (mgkftn)
            return;
    } 
    else 
    {
        switch (fate) 
        {
        case 19: /* Self-knowledge */
            pline_The_ex(ATR_NONE, CLR_MSG_POSITIVE, "water tingles in your mouth.");
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "self-knowledgeable...");
            display_nhwindow(WIN_MESSAGE, FALSE);
            enlightenment(MAGICENLIGHTENMENT, ENL_GAMEINPROGRESS);
            exercise(A_WIS, TRUE);
            pline_The("feeling subsides.");
            break;
        case 20: /* Foul water */
            pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "water is foul!  You gag and vomit.");
            morehungry(rn1(20, 11));
            vomit();
            break;
        case 21: /* Poisonous */
            pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "water is contaminated!");
            if (Poison_resistance) {
                pline("Perhaps it is runoff from the nearby %s farm.",
                      fruitname(FALSE));
                losehp(adjust_damage(rnd(4), (struct monst*)0, &youmonst, AD_DRST, ADFLAGS_NONE), "unrefrigerated sip of juice", KILLED_BY_AN);
                break;
            }
            losestr(rn1(4, 3));
            losehp(adjust_damage(rnd(10), (struct monst*)0, &youmonst, AD_DRST, ADFLAGS_NONE), "contaminated water", KILLED_BY);
            exercise(A_CON, FALSE);
            break;
        case 22: /* Fountain of snakes! */
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "water tastes a bit slimy.");
            if (zlevel > 8) {
                dowatersnakes();
            }
            else
            {
                You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a snake hissing.");
            }
            break;
        case 23: /* Water demon */
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "water tastes bitter.");
            if (zlevel > 12) {
                if (item_prevents_summoning(PM_WATER_DEMON)) 
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You have a passing sensation of relief.");
                }
                else
                {
                    dowaterdemon();
                }
            }
            else
            {
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel as if something evil is watching you.");
            }
            break;
        case 24: /* Curse an item */
        {
            register struct obj *obj;

            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "This water's no good!");
            morehungry(rn1(20, 11));
            exercise(A_CON, FALSE);
            for (obj = invent; obj; obj = obj->nobj)
                if (!rn2(5))
                    curse(obj);
            break;
        }
        case 25: /* See invisible */
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "water is tasteless.");
            if (Blind) {
                if (Invisib) {
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel transparent.");
                } else {
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel very self-conscious.");
                    pline("Then it passes.");
                }
            } else {
                You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "an image of someone stalking you.");
                pline("But it disappears.");
            }
            HSee_invisible |= FROM_ACQUIRED;
            newsym(u.ux, u.uy);
            exercise(A_WIS, TRUE);
            break;
        case 26: /* See Monsters */
            pline_The_ex(ATR_NONE, CLR_MSG_POSITIVE, "water tingles in your mouth.");
            (void) monster_detect((struct obj *) 0, 0);
            exercise(A_WIS, TRUE);
            break;
        case 27: /* Find a gem in the sparkling waters. */
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "water is hard.");
            if (!FOUNTAIN_IS_LOOTED(u.ux, u.uy)) {
                dofindgem();
                break;
            }
            /*FALLTHRU*/
        case 28: /* Water Nymph */
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "water tastes very sweet.");
            if (zlevel > 4) {
                dowaternymph();
            }
            else
            {
                if (Blind) {
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel as if somebody is watching you.");
                    pline("But then it passes disappears.");
                }
                else {
                    You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "an image of someone stalking you.");
                    pline("But it disappears.");
                }
            }
            break;
        case 29: /* Scare */
        {
            register struct monst *mtmp;

            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "This %s gives you bad breath!",
                  hliquid("water"));
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
                if (DEADMONSTER(mtmp))
                    continue;
                monflee(mtmp, 0, FALSE, FALSE);
            }
            break;
        }
        case 30: /* Gushing forth in this room */
            pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "water suddenly starts bubbling.");
            dogushforth(TRUE);
            break;
        default:
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "This tepid %s is tasteless.",
                  hliquid("water"));
            break;
        }

        if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
        {
            pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a magic fountain.");
            SET_FOUNTAIN_KNOWN(u.ux, u.uy);
        }

    }
    dryup(u.ux, u.uy, TRUE);
}

/* Monty Python and the Holy Grail ;) */
static const char* const excalmsgs[] = {
    "had Excalibur thrown at them by some watery tart",
    "received Excalibur from a strange woman laying about in a pond",
    "signified by divine providence, was chosen to carry Excalibur",
    "has been given Excalibur, and now enjoys supreme executive power",
    "endured an absurd aquatic ceremony, and now wields Excalibur"
};

void
dipfountain(obj)
register struct obj *obj;
{
    if (Levitation && !Levitation_control)
    {
        floating_above("fountain");
        return;
    }

    register int zlevel;
    zlevel = level_difficulty();
    boolean nowaterdamage = FALSE;
    boolean nodryup = FALSE;
    boolean effecthappened = FALSE;

    int ftyp = levl[u.ux][u.uy].subtyp; //(levl[u.ux][u.uy].fountainmask & FOUNTAIN_TYPE_MASK);

    /* Don't grant Excalibur when there's more than one object.  */
    /* (quantity could be > 1 if merged daggers got polymorphed) */
    if ((ftyp != FOUNTAIN_POISON)
        && obj->oclass == WEAPON_CLASS && objects[obj->otyp].oc_subtyp == WEP_LONG_SWORD && !is_demon_obj(obj)
        && obj->quan == 1L && u.ulevel >= 5 && !rn2(6)
        && !obj->oartifact
        && !exist_artifact(LONG_SWORD, artiname(ART_EXCALIBUR)))
    {
        if (u.ualign.type != A_LAWFUL) 
        {
            /* Ha!  Trying to cheat her. */
            pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "A freezing mist rises from the %s and envelopes the sword.",
                  hliquid("water"));
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "fountain disappears!");
            curse(obj);
            if (obj->enchantment > -6 && !rn2(3))
                obj->enchantment--;
            obj->oerodeproof = FALSE;
            exercise(A_WIS, FALSE);
            livelog_printf(LL_ARTIFACT, "was denied Excalibur! The Lady of the Lake has deemed %s unworthy", uhim());
        }
        else 
        {
            /* The lady of the lake acts! - Eric Backus */
            /* Be *REAL* nice */
            pline_ex(ATR_NONE, CLR_MSG_MYSTICAL,
              "From the murky depths, a hand reaches up to bless the sword.");
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "As the hand retreats, the fountain disappears!");
            obj->otyp = LONG_SWORD;
            obj->material = objects[obj->otyp].oc_material;
            obj = oname(obj, artiname(ART_EXCALIBUR));
            discover_artifact(ART_EXCALIBUR);
            bless(obj);
            obj->oeroded = obj->oeroded2 = 0;
            obj->oerodeproof = TRUE;
            exercise(A_WIS, TRUE);
            livelog_printf(LL_ARTIFACT, "%s", excalmsgs[rn2(SIZE(excalmsgs))]);
        }
        update_inventory();
        create_current_floor_location(u.ux, u.uy, 0, NO_GLYPH, TRUE);
        if (in_town(u.ux, u.uy))
            (void) angry_guards(FALSE);
        return;
    } 

    if (ftyp == FOUNTAIN_HEALING)
    {
        boolean identified = FALSE;
        if (obj && is_weapon(obj) && is_poisonable(obj) && obj->opoisoned)
        {
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "A coating wears off %s.", the(xname(obj)));
            obj->opoisoned = 0;
            identified = TRUE;
            effecthappened = TRUE;
        }
        else if (obj && (obj->otyp == POT_SICKNESS || obj->otyp == POT_POISON))
        {
            if (carried(obj))
            {
                if(objects[obj->otyp].oc_name_known)
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s purified.", Yobjnam2(obj, "are"));
                else 
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s.", Yobjnam2(obj, "clear"));
            }
            obj->otyp = POT_WATER;
            obj->dknown = 0;
            obj->blessed = obj->cursed = 0;
            obj->odiluted = 0;
            if (carried(obj))
                update_inventory();
            identified = TRUE;
            nowaterdamage = TRUE;
            effecthappened = TRUE;
        }
        else if (obj && obj->oclass == POTION_CLASS)
        {
            int oldotyp = obj->otyp;
            char oldnameturns[BUFSZ];
            strcpy(oldnameturns, Tobjnam(obj, "turn"));
            char oldnameare[BUFSZ];
            strcpy(oldnameare, Tobjnam(obj, "are"));
            unsigned int olddiluted = obj->odiluted;

            switch (obj->otyp)
            {
            case POT_GAIN_ENERGY:
            case POT_LESSER_REGENERATION:
            case POT_LESSER_REJUVENATION:
            case POT_SPEED:
            case POT_EXTRA_HEALING:
                obj->otyp = POT_EXTRA_HEALING;
                break;
            case POT_EXTRA_ENERGY:
            case POT_REGENERATION:
            case POT_REJUVENATION:
            case POT_GREATER_SPEED:
            case POT_GREATER_HEALING:
                obj->otyp = POT_GREATER_HEALING;
                break;
            case POT_GREATER_ENERGY:
            case POT_GREATER_REGENERATION:
            case POT_GREATER_REJUVENATION:
            case POT_LIGHTNING_SPEED:
                obj->otyp = POT_FULL_HEALING;
                break;
            case POT_FULL_ENERGY:
            case POT_FULL_HEALING:
                obj->otyp = POT_FULL_HEALING;
                break;
            default:
                obj->otyp = POT_HEALING;
                break;
            }
            obj->dknown = 0;
            obj->odiluted = 0;
            nowaterdamage = TRUE;

            if (obj->otyp != oldotyp)
            {
                identified = TRUE;
                effecthappened = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s imbued with healing energies of the fountain.", oldnameare);
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s into %s.", oldnameturns, doname(obj));
            }
            else if (obj->odiluted != olddiluted)
            {
                effecthappened = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s undiluted.", Tobjnam(obj, "become"));
            }

            if (carried(obj))
            {
                update_inventory();
            }

        }

        if (identified)
        {
            if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
            {
                pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of healing.");
                SET_FOUNTAIN_KNOWN(u.ux, u.uy);
            }
        }
    }
    else if (ftyp == FOUNTAIN_MANA)
    {
        boolean identified = FALSE;

        if (obj && objects[obj->otyp].oc_charged)
        {
            recharge(obj, 0, TRUE, "Fountain of Mana", FALSE);
            identified = TRUE;
            nowaterdamage = TRUE;
            effecthappened = TRUE;
        }
        else if (obj && obj->oclass == POTION_CLASS)
        {
            int oldotyp = obj->otyp;
            char oldnameturns[BUFSZ];
            strcpy(oldnameturns, Tobjnam(obj, "turn"));
            char oldnameare[BUFSZ];
            strcpy(oldnameare, Tobjnam(obj, "are"));
            unsigned int olddiluted = obj->odiluted;

            switch (obj->otyp)
            {
            case POT_HEALING:
            case POT_LESSER_REGENERATION:
            case POT_LESSER_REJUVENATION:
            case POT_EXTRA_ENERGY:
                obj->otyp = POT_EXTRA_ENERGY;
                break;
            case POT_EXTRA_HEALING:
            case POT_REGENERATION:
            case POT_REJUVENATION:
            case POT_GREATER_SPEED:
            case POT_GREATER_ENERGY:
                obj->otyp = POT_GREATER_ENERGY;
                break;
            case POT_GREATER_HEALING:
            case POT_GREATER_REGENERATION:
            case POT_GREATER_REJUVENATION:
            case POT_LIGHTNING_SPEED:
            case POT_FULL_ENERGY:
                obj->otyp = POT_FULL_ENERGY;
                break;
            case POT_FULL_HEALING:
                obj->otyp = POT_FULL_ENERGY;
                break;
            default:
                obj->otyp = POT_GAIN_ENERGY;
                break;
            }
            obj->dknown = 0;
            obj->odiluted = 0;
            nowaterdamage = TRUE;

            if (obj->otyp != oldotyp)
            {
                identified = TRUE;
                effecthappened = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s imbued with magical energies of the fountain.", oldnameare);
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s into %s.", oldnameturns, doname(obj));
            }
            else if (obj->odiluted != olddiluted)
            {
                effecthappened = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s undiluted.", Tobjnam(obj, "become"));
            }

            if (carried(obj))
            {
                update_inventory();
            }

        }

        if (identified)
        {
            if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
            {
                pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of mana.");
                SET_FOUNTAIN_KNOWN(u.ux, u.uy);
            }
        }
    }
    else if (ftyp == FOUNTAIN_POWER)
    {
        boolean identified = FALSE;

        if (obj && objects[obj->otyp].oc_charged)
        {
            recharge(obj, 0, TRUE, "Fountain of Power", FALSE);
            identified = TRUE;
            nowaterdamage = TRUE;
            effecthappened = TRUE;
        }
        else if (obj && obj->oclass == POTION_CLASS)
        {
            int oldotyp = obj->otyp;
            char oldnameturns[BUFSZ];
            strcpy(oldnameturns, Tobjnam(obj, "turn"));
            char oldnameare[BUFSZ];
            strcpy(oldnameare, Tobjnam(obj, "are"));
            unsigned int olddiluted = obj->odiluted;

            switch (obj->otyp)
            {
            case POT_GAIN_ENERGY:
                obj->otyp = POT_EXTRA_ENERGY;
                break;
            case POT_EXTRA_ENERGY:
                obj->otyp = POT_GREATER_ENERGY;
                break;
            case POT_GREATER_ENERGY:
            case POT_FULL_ENERGY:
                obj->otyp = POT_FULL_ENERGY;
                break;
            case POT_HEALING:
                obj->otyp = POT_EXTRA_HEALING;
                break;
            case POT_EXTRA_HEALING:
                obj->otyp = POT_GREATER_HEALING;
                break;
            case POT_GREATER_HEALING:
            case POT_FULL_HEALING:
                obj->otyp = POT_FULL_HEALING;
                break;
            case POT_RESTORE_ABILITY:
            case POT_GAIN_ABILITY:
                obj->otyp = POT_GAIN_ABILITY;
                break;
            case POT_GAIN_LEVEL:
                obj->otyp = POT_GAIN_LEVEL;
                break;
            case POT_LESSER_REGENERATION:
                obj->otyp = POT_REGENERATION;
                break;
            case POT_REGENERATION:
            case POT_GREATER_REGENERATION:
                obj->otyp = POT_GREATER_REGENERATION;
                break;
            case POT_LESSER_REJUVENATION:
                obj->otyp = POT_REJUVENATION;
                break;
            case POT_REJUVENATION:
            case POT_GREATER_REJUVENATION:
                obj->otyp = POT_GREATER_REJUVENATION;
                break;
            case POT_HEROISM:
            case POT_SUPER_HEROISM:
                obj->otyp = POT_SUPER_HEROISM;
                break;
            case POT_SPEED:
                obj->otyp = POT_GREATER_SPEED;
                break;
            case POT_GREATER_SPEED:
            case POT_LIGHTNING_SPEED:
                obj->otyp = POT_LIGHTNING_SPEED;
                break;
            default:
                obj->otyp = POT_LESSER_REJUVENATION;
                break;
            }
            obj->dknown = 0;
            obj->odiluted = 0;
            nowaterdamage = TRUE;

            if (obj->otyp != oldotyp)
            {
                identified = TRUE;
                effecthappened = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s empowered by the energies of the fountain.", oldnameare);
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s into %s.", oldnameturns, doname(obj));
            }
            else if (obj->odiluted != olddiluted)
            {
                effecthappened = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s undiluted.", Tobjnam(obj, "become"));
            }

            if (carried(obj))
            {
                update_inventory();
            }

        }

        if (identified)
        {
            if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
            {
                pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of power.");
                SET_FOUNTAIN_KNOWN(u.ux, u.uy);
            }
        }
    }
    else if (ftyp == FOUNTAIN_WATER)
    {
        if (!rn2(2))
        { /* no dryup */
            nodryup = TRUE;
        }
    }
    else if (ftyp == FOUNTAIN_NATURAL)
    {
        if (!rn2(8))
        { /* no dryup */
            nodryup = TRUE;
        }
    }
    else if (ftyp == FOUNTAIN_POISON)
    {
        int oldotyp = obj->otyp;
        char oldnameturns[BUFSZ];
        strcpy(oldnameturns, Tobjnam(obj, "turn"));
        char oldnamestart[BUFSZ];
        strcpy(oldnamestart, Tobjnam(obj, "start"));
        unsigned int olddiluted = obj->odiluted;

        nowaterdamage = TRUE;
        boolean identified = FALSE;

        if (obj && is_weapon(obj) && is_poisonable(obj) && !obj->opoisoned)
        {
            play_sfx_sound(SFX_POISON_COATING);
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "The fountain forms a coating on %s.", the(xname(obj)));
            obj->opoisoned = TRUE;
            identified = TRUE;
            effecthappened = TRUE;
        }
        else if (obj && obj->oclass == POTION_CLASS)
        {
            obj->otyp = POT_POISON;
            obj->dknown = 0;
            obj->odiluted = 0;

            if (obj->otyp != oldotyp)
            {
                identified = TRUE;
                effecthappened = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s to smell foul.", oldnamestart);
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s into %s.", oldnameturns, doname(obj));
            }
            else if (obj->odiluted != olddiluted)
            {
                effecthappened = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s undiluted.", Tobjnam(obj, "become"));
            }

            if (carried(obj))
                update_inventory();
            identified = TRUE;
            effecthappened = TRUE;
        }
        else if (obj && obj->oclass == SCROLL_CLASS)
            nowaterdamage = FALSE;

        if (identified)
        {
            if (!FOUNTAIN_IS_KNOWN(u.ux, u.uy))
            {
                pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a fountain of poison.");
                SET_FOUNTAIN_KNOWN(u.ux, u.uy);
            }
        }
    }
    else
    {
        boolean identified = TRUE;
        effecthappened = TRUE;
        switch (rnd(30))
        {
        default:
            identified = FALSE;
            effecthappened = FALSE;
            break;
        case 16: /* Curse the item */
            if (!obj->cursed)
            {
                curse(obj);
                if (!Blind)
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s %s for a moment.", Tobjnam(obj, "glow"), hcolor(NH_BLACK));
                else
                {
                    identified = FALSE;
                    effecthappened = FALSE;    /* This is just apparently so */
                }
            }
            else
            {
                identified = FALSE;
                effecthappened = FALSE;
            }
            break;
        case 17:
        case 18:
        case 19:
        case 20: /* Uncurse the item */
            if (obj->cursed) 
            {
                if (!Blind)
                    pline_The_ex(ATR_NONE, CLR_MSG_SPELL, "%s glows for a moment.", hliquid("water"));
                uncurse(obj);
            } 
            else 
            {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A feeling of loss comes over you.");
            }
            break;
        case 21: /* Water Demon */
            if(zlevel > 12) { 
                if (item_prevents_summoning(PM_WATER_DEMON))
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You have a passing sensation of relief.");
                }
                else
                {
                    dowaterdemon();
                }
            } else
            {
                You_ex(ATR_NONE, CLR_MSG_WARNING, "feel as if something evil is watching you.");
            }
            break;
        case 22: /* Water Nymph */
            if (zlevel > 4) {
                dowaternymph();
            }
            else
            {
                if (Blind) {
                        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel as if somebody is watching you.");
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "But then it passes disappears.");
                }
                else {
                    You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "an image of someone stalking you.");
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "But it disappears.");
                }
            }
            break;
        case 23: /* an Endless Stream of Snakes */
            if (zlevel > 8) {
                dowatersnakes();
            }
            else
            {
                You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "a snake hissing.");
            }
            break;
        case 24: /* Find a gem */
            if (!FOUNTAIN_IS_LOOTED(u.ux, u.uy)) {
                dofindgem();
                break;
            }
            /*FALLTHRU*/
        case 25: /* Water gushes forth */
            dogushforth(FALSE);
            break;
        case 26: /* Strange feeling */
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A strange tingling runs up your %s.", body_part(ARM));
            break;
        case 27: /* Strange feeling */
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "a sudden chill.");
            break;
        case 28: /* Strange feeling */
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "An urge to take a bath overwhelms you.");
            {
                long money = money_cnt(invent);
                struct obj *otmp;
                if (money > 10) {
                    /* Amount to lose.  Might get rounded up as fountains don't
                     * pay change... */
                    money = somegold(money) / 10;
                    for (otmp = invent; otmp && money > 0; otmp = otmp->nobj)
                        if (otmp->oclass == COIN_CLASS) {
                            long denomination = objects[otmp->otyp].oc_cost;
                            long coin_loss =
                                (money + denomination - 1) / denomination;
                            coin_loss = min(coin_loss, otmp->quan);
                            otmp->quan -= coin_loss;
                            money -= coin_loss * denomination;
                            if (!otmp->quan)
                                delobj(otmp);
                        }
                    You_ex(ATR_NONE, CLR_MSG_WARNING, "lost some of your money in the fountain!");
                    CLEAR_FOUNTAIN_LOOTED(u.ux, u.uy);
                    exercise(A_WIS, FALSE);
                }
            }
            break;
        case 29: /* You see coins */
            /* We make fountains have more coins the closer you are to the
             * surface.  After all, there will have been more people going
             * by.  Just like a shopping mall!  Chris Woodbury  */

            if (FOUNTAIN_IS_LOOTED(u.ux, u.uy))
                break;
            SET_FOUNTAIN_LOOTED(u.ux, u.uy);
            (void) mkgold((long) (rnd((dunlevs_in_dungeon(&u.uz) - dunlev(&u.uz)
                                       + 1) * 2) + 5),
                          u.ux, u.uy);
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Far below you, you see coins glistening in the %s.",
                      hliquid("water"));
            exercise(A_WIS, TRUE);
            newsym(u.ux, u.uy);
            break;
        }

        if (identified && !FOUNTAIN_IS_KNOWN(u.ux, u.uy))
        {
            pline_ex(ATR_NONE, CLR_MSG_HINT, "That was a magic fountain.");
            SET_FOUNTAIN_KNOWN(u.ux, u.uy);
        }

    }

    if (!nowaterdamage)
    {
        int er = water_damage(obj, NULL, TRUE);

        if (er != ER_NOTHING)
            effecthappened = TRUE;
#if 0
        if (obj->otyp == POT_ACID && er != ER_DESTROYED)
        { /* Acid and water don't mix */
            useup(obj);
            nodryup = TRUE;
        }
        else if (er != ER_NOTHING && ftyp == FOUNTAIN_MAGIC && !rn2(2))
        { /* no further effect */
            nodryup = TRUE;
        }
#endif
    }

    if(!effecthappened)
        pline("Nothing much happens.");

    update_inventory();

    if(!nodryup)
        dryup(u.ux, u.uy, TRUE);
}

const char* get_fountain_name(x, y)
int x, y;
{
    if (!isok(x, y))
        return "invalid coordinates";

    if (!IS_FOUNTAIN(levl[x][y].typ))
        return "a non-fountain";

    int ftyp = levl[x][y].subtyp; //  (levl[x][y].fountainmask& FOUNTAIN_TYPE_MASK);

    return FOUNTAIN_IS_KNOWN(x, y) ? (ftyp > FOUNTAIN_MAGIC ? fountain_type_text(ftyp) : levl[x][y].blessedftn ? "enchanted magic fountain" : "magic fountain") : defsyms[S_fountain].explanation;
}


const char* fountain_type_text(ftyp)
int ftyp;
{
    switch (ftyp)
    {
    case FOUNTAIN_NATURAL:
        return "natural fountain";
        break;
    case FOUNTAIN_MAGIC:
        return "magic fountain";
        break;
    case FOUNTAIN_HEALING:
        return "fountain of healing";
        break;
    case FOUNTAIN_MANA:
        return "fountain of mana";
        break;
    case FOUNTAIN_POWER:
        return "fountain of power";
        break;
    case FOUNTAIN_WATER:
        return "fountain of water";
        break;
    case FOUNTAIN_POISON:
        return "fountain of poison";
        break;
    default:
        return "fountain";
        break;
    }
}

void
breaksink(x, y)
int x, y;
{
    if (iflags.using_gui_sounds)
    {
        play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_BREAK);
        delay_output_milliseconds(1000);
    }
    if (cansee(x, y) || (x == u.ux && y == u.uy))
        pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "pipes break!  Water spurts out!");

    play_special_effect_at(SPECIAL_EFFECT_TRAP_RUST, 0, x, y, FALSE);
    play_sfx_sound(SFX_GUSH_OF_WATER_HITS);
    special_effect_wait_until_action(0);

    create_simple_location(x, y, FOUNTAIN, 0, 0, FOUNTAIN_NATURAL, 0 /*back_to_broken_glyph(x, y)*/, levl[x][y].floortyp, levl[x][y].floorsubtyp, levl[x][y].floorvartyp, FALSE);
    SET_FOUNTAIN_LOOTED(x, y);
    newsym(x, y);

    special_effect_wait_until_end(0);
}

void
drinksink()
{
    struct obj *otmp;
    struct monst *mtmp;

    if (Levitation && !Levitation_control) 
    {
        floating_above("sink");
        return;
    }
    
    if (iflags.using_gui_sounds)
    {
        play_sfx_sound(SFX_SINK_OPEN_TAP);
        delay_output_milliseconds(1500);
    }

    switch (rn2(20))
    {
    case 0:
        play_sfx_sound(SFX_QUAFF);
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "take a sip of very cold %s.", hliquid("water"));
        break;
    case 1:
        play_sfx_sound(SFX_QUAFF);
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "take a sip of very warm %s.", hliquid("water"));
        break;
    case 2:
        play_sfx_sound(SFX_QUAFF);
        You_ex(ATR_NONE, CLR_MSG_WARNING, "take a sip of scalding hot %s.", hliquid("water"));
        if (Fire_immunity || Improved_fire_resistance || Fire_resistance)
            pline("It seems quite tasty.");
        else
        {
            play_sfx_sound(SFX_SCOLDED);
            losehp(adjust_damage(rnd(6), (struct monst*)0, &youmonst, AD_FIRE, ADFLAGS_NONE), "sipping boiling water", KILLED_BY);
        }
        /* boiling water burns considered fire damage */
        break;
    case 3:
        if (mvitals[PM_SEWER_RAT].mvflags & MV_GONE)
            pline_The("sink seems quite dirty.");
        else {
            play_sfx_sound(SFX_WAS_HIDING);
            mtmp = makemon(&mons[PM_SEWER_RAT], u.ux, u.uy, NO_MM_FLAGS);
            if (mtmp)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "Eek!  There's %s in the sink!",
                      (Blind || !canspotmon(mtmp)) ? "something squirmy"
                                                   : a_monnam(mtmp));
        }
        break;
    case 4:
        do {
            otmp = mkobj(POTION_CLASS, FALSE, FALSE);
            if (otmp->otyp == POT_WATER) {
                obfree(otmp, (struct obj *) 0);
                otmp = (struct obj *) 0;
            }
        } while (!otmp);
        otmp->cursed = otmp->blessed = 0;
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Some %s liquid flows from the faucet.",
              Blind ? "odd" : hcolor(OBJ_DESCR(objects[otmp->otyp])));
        otmp->dknown = !(Blind || Hallucination);
        otmp->quan++;       /* Avoid panic upon useup() */
        otmp->speflags |= SPEFLAGS_FROM_SINK; /* kludge for docall() */
        (void) dopotion(otmp);
        obfree(otmp, (struct obj *) 0);
        break;
    case 5:
        if (!(levl[u.ux][u.uy].looted & S_LRING)) {
            play_sfx_sound(SFX_HIDDEN_TREASURE_FOUND);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "find a ring in the sink!");
            (void) mkobj_at(RING_CLASS, u.ux, u.uy, TRUE);
            levl[u.ux][u.uy].looted |= S_LRING;
            exercise(A_WIS, TRUE);
            newsym(u.ux, u.uy);
        } else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Some dirty %s backs up in the drain.", hliquid("water"));
        break;
    case 6:
        breaksink(u.ux, u.uy);
        break;
    case 7:
        pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "%s moves as though of its own will!", hliquid("water"));
        if ((mvitals[PM_WATER_ELEMENTAL].mvflags & MV_GONE)
            || !makemon(&mons[PM_WATER_ELEMENTAL], u.ux, u.uy, NO_MM_FLAGS))
            pline("But it quiets down.");
        else
            play_sfx_sound(SFX_WAS_HIDING);
        break;
    case 8:
        play_sfx_sound(SFX_QUAFF);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Yuk, this %s tastes awful.", hliquid("water"));
        more_experienced(1, 0);
        newexplevel();
        break;
    case 9:
        play_sfx_sound(SFX_QUAFF);
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "Gaggg... this tastes like sewage!  You vomit.");
        morehungry(rn1(30 - ACURR(A_CON), 11));
        vomit();
        break;
    case 10:
        play_sfx_sound(SFX_QUAFF);
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "This %s contains toxic wastes!", hliquid("water"));
        if (!Unchanging) {
            You("undergo a freakish metamorphosis!");
            polyself(0);
        }
        break;
    /* more odd messages --JJB */
    case 11:
        play_sfx_sound(SFX_SINK_CLANKING_FROM_PIPES);
        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "clanking from the pipes...");
        break;
    case 12:
        play_sfx_sound(SFX_SINK_SNATCHES_OF_SONG);
        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "snatches of song from among the sewers...");
        break;
    case 19:
        if (Hallucination) {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "From the murky drain, a hand reaches up... --oops--");
            break;
        }
        /*FALLTHRU*/
    default:
        play_sfx_sound(SFX_QUAFF);
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "take a sip of %s %s.",
            rn2(3) ? (rn2(2) ? "cold" : "warm") : "hot",
            hliquid("water"));
    }
}

void
init_fountains()
{
    /*Initialize fountain variations */
    int i;
    for (i = 0; i < MAX_FOUNTAIN_SUBTYPES; i++)
    {
        context.used_fountain_subtype[i] = i;
    }

    /* Water always looks like water, so it is not shuffled (the same for natural) */
    for (i = FOUNTAIN_MAGIC; i < LAST_SHUFFLED_FOUNTAIN - 1; i++)
    {
        int new_i = i + rn2(LAST_SHUFFLED_FOUNTAIN - i);
        int saved_value = context.used_fountain_subtype[i];
        context.used_fountain_subtype[i] = context.used_fountain_subtype[new_i];
        context.used_fountain_subtype[new_i] = saved_value;
    }

}

/*fountain.c*/
