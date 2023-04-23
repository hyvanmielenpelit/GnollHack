/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

/* GnollHack 4.0    mthrowu.c    $NHDT-Date: 1542765360 2018/11/21 01:56:00 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.78 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2016. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <math.h>

STATIC_DCL int FDECL(monmulti, (struct monst *, struct obj *, struct obj *));
STATIC_DCL void FDECL(monshoot, (struct monst *, struct obj *, struct obj *));
STATIC_DCL int FDECL(drop_throw, (struct obj *, BOOLEAN_P, int, int));
STATIC_DCL boolean FDECL(m_lined_up, (struct monst *, struct monst *, BOOLEAN_P, int, BOOLEAN_P, int));

extern const char* const flash_types[]; /* from zap.c */

#define URETREATING(x, y) \
    (distmin(u.ux, u.uy, x, y) > distmin(u.ux0, u.uy0, x, y))

#define PET_MISSILE_RANGE2 36 /* Square of distance within which pets shoot */

/*
 * Keep consistent with breath weapons in zap.c, and AD_* in monattk.h.
 */
STATIC_OVL NEARDATA const char *breathwep[] = {
    "fragments", "fire", "frost", "sleep gas", "a disintegration blast",
    "lightning", "poison gas", "acid", "a death ray",
    "a cloud of petrifying vapors"
};

STATIC_OVL NEARDATA const char* eyestalk[] = {
    "a ray of magical energy", "a fire ray", "a frost ray", "a sleep ray", "a disintegration ray",
    "a ray of lightning", "a ray of negative energy", "an acid ray", "a death ray",
    "a petrification ray"
};

extern boolean notonhead; /* for long worms */
STATIC_VAR int mesg_given; /* for m_throw()/thitu() 'miss' message */

/* hero is hit by something other than a monster */
int
thitu(tlev, dam, objp, name)
int tlev, dam;
struct obj **objp;
const char *name; /* if null, then format `*objp' */
{
    struct obj *obj = objp ? *objp : 0;
    const char *onm, *knm;
    boolean is_acid;
    int kprefix = KILLED_BY_AN, dieroll;
    char onmbuf[BUFSZ], knmbuf[BUFSZ];

    if (!name) {
        if (!obj)
        {
            panic("thitu: name & obj both null?");
            return 0;
        }
        name = strcpy(onmbuf,
                      (obj->quan > 1L) ? doname(obj) : mshot_xname(obj));
        knm = strcpy(knmbuf, killer_xname(obj));
        kprefix = KILLED_BY; /* killer_name supplies "an" if warranted */
    } else {
        knm = name;
        /* [perhaps ought to check for plural here to] */
        if (!strncmpi(name, "the ", 4) || !strncmpi(name, "an ", 3)
            || !strncmpi(name, "a ", 2))
            kprefix = KILLED_BY;
    }
    onm = (obj && obj_is_pname(obj)) ? the(name)
          : (obj && obj->quan > 1L) ? name
            : an(name);
    is_acid = (obj && obj->otyp == ACID_VENOM);

    //TO-HIT IS DONE HERE
    dieroll = rnd(20);
    if (u.uac + tlev <= dieroll) 
    {
        ++mesg_given;
        if (Blind || !flags.verbose) 
        {
            pline("It misses.");
        }
        else if (u.uac + tlev <= dieroll - 2) 
        {
            if (onm != onmbuf)
                Strcpy(onmbuf, onm); /* [modifiable buffer for upstart()] */
            pline("%s %s you.", upstart(onmbuf), vtense(onmbuf, "miss"));
        } else
            You("are almost hit by %s.", onm);
        return 0;
    } 
    else 
    {
        if ((is_acid && Acid_immunity))
        {
            if (Blind || !flags.verbose)
                You("are hit, but it does not seem to hurt you%s", exclam(dam));
            else
                You("are hit by %s, but it doesn't seem to hurt you%s", onm, exclam(dam));
            //pline("It doesn't seem to hurt you.");
        } 
        else if (obj && obj->oclass == POTION_CLASS)
        {
            /* an explosion which scatters objects might hit hero with one
               (potions deliberately thrown at hero are handled by m_throw) */
            if (Blind || !flags.verbose)
                You("are hit by something%s", exclam(dam));
            else
                You("are hit by %s%s", onm, exclam(dam));
            potionhit(&youmonst, &obj, POTHIT_OTHER_THROW);
            *objp = obj = 0; /* potionhit() uses up the potion */
        } 
        else 
        {
            double damage = adjust_damage(dam, (struct monst*)0, &youmonst, obj ? objects[obj->otyp].oc_damagetype : AD_PHYS, ADFLAGS_NONE);
            if(damage == 0)
            {
                if (Blind || !flags.verbose)
                    You("are hit, but it does not seem to hurt you%s", exclam(dam));
                else
                    You("are hit by %s, but it doesn't seem to hurt you%s", onm, exclam(dam));
            }
            else
            {
                int damagedealt = (int)damage + ((damage - (double)((int)damage) - ((double)(Upolyd ? u.mh_fraction : u.uhp_fraction) / 10000)) > 0 ? 1 : 0);

                if (Blind || !flags.verbose)
                    You("are hit for %d damage%s", damagedealt, exclam(damagedealt));
                else
                    You("are hit by %s for %d damage%s", onm, damagedealt, exclam(damagedealt));

                display_u_being_hit(HIT_GENERAL, damagedealt, 0UL);
            }
            if (obj && obj->material == MAT_SILVER && Hate_silver)
            {
                /* extra damage already applied by weapon_dmg_value() */
                pline_The("silver sears your flesh!");
                exercise(A_CON, FALSE);
            }
            if (is_acid)
                pline("It burns!"); /* acid damage */

            if (obj)
            {
                play_object_hit_sound(obj, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(&youmonst), damage, HMON_THROWN);
            }
            //DAMAGE IS DONE HERE
            losehp(damage, knm, kprefix);
            exercise(A_STR, FALSE);
        }
        return 1;
    }
}

/* Be sure this corresponds with what happens to player-thrown objects in
 * dothrow.c (for consistency). --KAA
 * Returns 0 if object still exists (not destroyed).
 */
STATIC_OVL int
drop_throw(obj, ohit, x, y)
register struct obj *obj;
boolean ohit;
int x, y;
{
    if (!obj)
        return 1;

    int retvalu = 1;
    int create;
    struct monst *mtmp;
    struct trap *t;
    boolean uses_spell_flags = obj ? object_uses_spellbook_wand_flags_and_properties(obj) : FALSE;

    if (obj->otyp == CREAM_PIE 
        || (!uses_spell_flags && (objects[obj->otyp].oc_aflags & A1_ITEM_VANISHES_ON_HIT) // The item vanishes here always, because we cannot check appropriateness properly
            )
        || (is_fragile(obj)
        && !is_obj_indestructible(obj)
        && !is_quest_artifact(obj)
        && !obj->oartifact)
        || obj->oclass == VENOM_CLASS
        || (ohit && obj->otyp == EGG))
        create = 0;
    else if (ohit && (is_multigen(obj) || is_rock(obj)))
        create = !rn2(3);
    else
        create = 1;

    if (create && !((mtmp = m_at(x, y)) != 0 && mtmp->mtrapped
                    && (t = t_at(x, y)) != 0
                    && is_pit(t->ttyp))) {
        int objgone = 0;

        if (down_gate(x, y) != -1)
            objgone = ship_object(obj, x, y, FALSE);
        if (!objgone) {
            if (!flooreffects(obj, x, y, "fall")) {
                place_object(obj, x, y);
                if (!mtmp && x == u.ux && y == u.uy)
                    mtmp = &youmonst;
                if (mtmp && ohit)
                    passive_obj(mtmp, obj, (struct attack *) 0);
                stackobj(obj);
                retvalu = 0;
            }
        }
    } else
        obfree(obj, (struct obj *) 0);
    return retvalu;
}

/* The monster that's being shot at when one monster shoots at another */
STATIC_OVL struct monst *target = 0;
/* The monster that's doing the shooting/throwing */
STATIC_OVL struct monst *archer = 0;

/* calculate multishot volley count for mtmp throwing otmp (if not ammo) or
   shooting otmp with mwep (if otmp is ammo and mwep appropriate launcher) */
STATIC_OVL int
monmulti(mtmp, otmp, mwep)
struct monst *mtmp;
struct obj *otmp, *mwep;
{
    //int skill = (int) objects[otmp->otyp].oc_skill;
    int multishot = 1;

    if (otmp->quan > 1L /* no point checking if there's only 1 */
        /* ammo requires corresponding launcher be wielded */
        && (is_ammo(otmp)
               ? matching_launcher(otmp, mwep)
               /* otherwise any stackable (non-ammo) weapon */
               : otmp->oclass == WEAPON_CLASS)
        && !is_confused(mtmp)) 
    {
        struct multishot_result msres = get_multishot_stats(mtmp, otmp, mwep, TRUE);
        multishot = msres.wielder_attacks * msres.weapon_attacks;
    }

    if (otmp->quan < multishot)
        multishot = (int) otmp->quan;
    if (multishot < 1)
        multishot = 1;
    return multishot;
}

/* mtmp throws otmp, or shoots otmp with mwep, at hero or at monster mtarg */
STATIC_OVL void
monshoot(mtmp, otmp, mwep)
struct monst *mtmp;
struct obj *otmp, *mwep;
{
    if (!mtmp || !otmp)
        return;

    boolean is_firing = (otmp && mwep && ammo_and_launcher(otmp, mwep));

    struct monst *mtarg = target;

    int weprange = m_weapon_range(mtmp, otmp, mwep);
    int multishot = monmulti(mtmp, otmp, mwep);
    /* int dm = distmin(mtmp->mx, mtmp->my,
        mtarg ? mtarg->mx : mtmp->mux,
        mtarg ? mtarg->my : mtmp->muy);*/
    /*
    * Caller must have called linedup() to set up tbx, tby.
    */

    if (canseemon(mtmp)) 
    {
        const char *onm;
        char onmbuf[BUFSZ], trgbuf[BUFSZ];

        if (multishot > 1) 
        {
            /* "N arrows"; multishot > 1 implies otmp->quan > 1, so
               xname()'s result will already be pluralized */
            Sprintf(onmbuf, "%d %s", multishot, xname(otmp));
            onm = onmbuf;
        } 
        else 
        {
            /* "an arrow" */
            onm = singular(otmp, xname);
            onm = obj_is_pname(otmp) ? the(onm) : an(onm);
        }
        m_shot.s = ammo_and_launcher(otmp, mwep) ? TRUE : FALSE;
        Strcpy(trgbuf, mtarg ? mon_nam(mtarg) : "");
        if (!strcmp(trgbuf, "it"))
            Strcpy(trgbuf, humanoid(mtmp->data) ? "someone" : something);
        pline("%s %s %s%s%s!", Monnam(mtmp),
              m_shot.s ? "shoots" : "throws", onm,
              mtarg ? " at " : "", trgbuf);
        m_shot.o = otmp->otyp;
    }
    else
    {
        m_shot.o = STRANGE_OBJECT; /* don't give multishot feedback */
    }

    m_shot.n = multishot;
    for (m_shot.i = 1; m_shot.i <= m_shot.n; m_shot.i++) 
    {
        update_m_action(mtmp, is_firing ? ACTION_TILE_FIRE : ACTION_TILE_THROW);
        if(is_firing)
            play_monster_simple_weapon_sound(mtmp, 0, mwep, OBJECT_SOUND_TYPE_FIRE);
        else
            play_monster_simple_weapon_sound(mtmp, 0, otmp, OBJECT_SOUND_TYPE_THROW);
        m_wait_until_action();

        m_throw(mtmp, mtmp->mx, mtmp->my, sgn(tbx), sgn(tby), weprange, otmp);
        /* conceptually all N missiles are in flight at once, but
           if mtmp gets killed (shot kills adjacent gas spore and
           triggers explosion, perhaps), inventory will be dropped
           and otmp might go away via merging into another stack */
        update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
        if (DEADMONSTER(mtmp) && m_shot.i < m_shot.n)
            /* cancel pending shots (perhaps ought to give a message here
               since we gave one above about throwing/shooting N missiles) */
            break; /* endmultishot(FALSE); */
    }
    /* reset 'm_shot' */
    m_shot.n = m_shot.i = 0;
    m_shot.o = STRANGE_OBJECT;
    m_shot.s = FALSE;


}

/* an object launched by someone/thing other than player attacks a monster;
   return 1 if the object has stopped moving (hit or its range used up) */
int
ohitmon(mtmp, otmp, range, verbose)
struct monst *mtmp; /* accidental target, located at <bhitpos.x,.y> */
struct obj *otmp;   /* missile; might be destroyed by drop_throw */
int range;          /* how much farther will object travel if it misses;
                       use -1 to signify to keep going even after hit,
                       unless it's gone (used for rolling_boulder_traps) */
boolean verbose;    /* give message(s) even when you can't see what happened */
{
    if (!mtmp || !otmp)
        return 1;

    int dmg, tmp;
    double damage = 0;
    boolean vis, ismimic;
    int objgone = 1;
    double poisondamage = 0;
    struct obj *mon_launcher = archer ? MON_WEP(archer) : NULL;

    notonhead = (bhitpos.x != mtmp->mx || bhitpos.y != mtmp->my);
    ismimic = M_AP_TYPE(mtmp) && M_AP_TYPE(mtmp) != M_AP_MONSTER;
    vis = cansee(bhitpos.x, bhitpos.y);

    tmp = 5 + find_mac(mtmp) + omon_adj(mtmp, otmp, FALSE);
    /* High level monsters will be more likely to hit */
    /* This check applies only if this monster is the target
     * the archer was aiming at. */
    if (archer && mtmp && target == mtmp) 
    {
        if (archer->m_lev > 5)
            tmp += archer->m_lev - 5;
//        if (mon_launcher && mon_launcher->oartifact)
//            tmp += spec_abon(mon_launcher, mtmp);
        if (otmp && mon_launcher && is_ammo(otmp)) 
        {
            if (!ammo_and_launcher(otmp, mon_launcher)) 
            {
                tmp -= 4;
            }
            else
            {
                tmp += weapon_to_hit_value(mon_launcher, mtmp, (struct monst*)0, 2);
                //tmp += weapon_skill_hit_bonus(uwep);  //Monsters do not get skill-based to-hit bonuses

                //Penalty for shooting short range
                if (archer && distmin(archer->mx, archer->my, mtmp->mx, mtmp->my) <= 1) 
                {
                    switch (objects[mon_launcher->otyp].oc_skill) 
                    {
                    case P_BOW:
                        tmp -= 10;
                        break;
                    case P_CROSSBOW:
                        tmp -= 8;
                        break;
                    default:
                        tmp -= 10;
                        break;
                    }
                }
            }
        }
    }

    if (tmp < rnd(20)) 
    {
        if (!ismimic) 
        {
            if (vis)
                miss(distant_name(otmp, mshot_xname), mtmp);
            else if (verbose && !target)
                pline("It is missed.");
        }
        if (!range) 
        { /* Last position; object drops */
            (void) drop_throw(otmp, 0, mtmp->mx, mtmp->my);
            return 1;
        }
    } 
    else if (otmp->oclass == POTION_CLASS)
    {
        if (ismimic)
            seemimic(mtmp);
        mtmp->msleeping = 0;
        refresh_m_tile_gui_info(mtmp, TRUE);
        if (vis)
            otmp->dknown = 1;
        /* probably thrown by a monster rather than 'other', but the
           distinction only matters when hitting the hero */
        potionhit(mtmp, &otmp, POTHIT_OTHER_THROW);
        return 1;
    }
    else
    {
        if (is_launcher(otmp))
            dmg = d(1, 2);
        else
            dmg = weapon_total_dmg_value(otmp, mtmp, (struct monst*)0, 1);

        if (otmp && mon_launcher && ammo_and_launcher(otmp, mon_launcher)) 
        {
            dmg += weapon_total_dmg_value(mon_launcher, mtmp, (struct monst*)0, 2);

            //Add strength dmg, no skill dmg
            if (objects[mon_launcher->otyp].oc_flags3 & O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH)
                dmg += objects[mon_launcher->otyp].oc_fixed_damage_bonus;
            else
                dmg += archer ? m_str_dmg_bonus(archer) : 0;

            /*
            if (mon_launcher->otyp == CROSSBOW) {
                dmg += 3;
            }
            else if (mon_launcher->otyp == HEAVY_CROSSBOW) {
                dmg += 6;
            }
            else if (mon_launcher->otyp == HAND_CROSSBOW) {
                dmg += 0;
            }
            else if (mon_launcher->otyp == REPEATING_CROSSBOW) {
                dmg += 0;
            }
            else {
                if(archer)
                    dmg += m_str_dmg_bonus(archer);
            }
            */
            //Bracers here, if need be
        }

        if (otmp->otyp == ACID_VENOM && is_mon_immune_to_acid(mtmp))
            dmg = 0;

        if (ismimic)
            seemimic(mtmp);

        if (mtmp->msleeping)
        {
            mtmp->msleeping = 0;
            refresh_m_tile_gui_info(mtmp, TRUE);
        }

        damage = adjust_damage(dmg, (struct monst*)0, mtmp, objects[otmp->otyp].oc_damagetype, ADFLAGS_NONE);

        enum hit_tile_types hit_tile = (otmp->opoisoned && is_poisonable(otmp) && !resists_poison(mtmp) ? HIT_POISONED : HIT_GENERAL);

        if (vis) 
        {
            if (otmp->otyp == EGG)
            {
                display_m_being_hit(mtmp, hit_tile, 0, 0UL, FALSE);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Splat!  %s is hit with %s egg!", Monnam(mtmp),
                    otmp->known ? an(corpse_monster_name(otmp)) : "an");
            }
            else
                hit_with_hit_tile(distant_name(otmp, mshot_xname), mtmp, exclam((int)ceil(damage)), (int)ceil(damage), "", hit_tile, FALSE);
        } 
        else if (verbose && !target)
        {

            display_m_being_hit(mtmp, hit_tile, 0, 0UL, FALSE);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%s is hit%s", (otmp->otyp == EGG) ? "Splat!  " : "",
                Monnam(mtmp), exclam((int)ceil(damage)));
        }

        if (otmp->opoisoned && is_poisonable(otmp)) 
        {
            if (resists_poison(mtmp)) 
            {
                if (vis)
                {
                    //play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "poison doesn't seem to affect %s.",
                        mon_nam(mtmp));
                }
            } 
            else 
            {
                play_sfx_sound_at_location(SFX_MONSTER_IS_POISONED, mtmp->mx, mtmp->my);
                poisondamage = adjust_damage(d(2, 6), (struct monst*)0, mtmp, AD_DRST, ADFLAGS_NONE);
                damage += poisondamage;
            }
        }

        if (otmp->elemental_enchantment)
        {
            switch(otmp->elemental_enchantment)
            {
            case COLD_ENCHANTMENT:
                if (is_mon_immune_to_cold(mtmp)) 
                {
                    if (vis)
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "cold doesn't seem to affect %s.",
                            mon_nam(mtmp));
                    }
                }
                else 
                {
                    if (vis)
                    {
                        play_sfx_sound_at_location(SFX_MONSTER_COVERED_IN_FROST, mtmp->mx, mtmp->my);
                        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "cold sears %s!", mon_nam(mtmp));
                    }
                    else if (verbose && !target)
                    {
                        play_sfx_sound_at_location(SFX_MONSTER_COVERED_IN_FROST, mtmp->mx, mtmp->my);
                        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "cold sears it!");
                    }
                    damage += adjust_damage(d(12, 6), (struct monst*)0, mtmp, AD_COLD, ADFLAGS_NONE);
                }
                if(is_ammo(otmp) || throwing_weapon(otmp) || objects[otmp->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                    otmp->elemental_enchantment = 0;
                break;
            case FIRE_ENCHANTMENT:
                if (is_mon_immune_to_fire(mtmp)) 
                {
                    if (vis)
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "fire doesn't seem to affect %s.",
                            mon_nam(mtmp));
                    }
                }
                else 
                {
                    if (vis)
                    {
                        play_sfx_sound_at_location(SFX_MONSTER_ON_FIRE, mtmp->mx, mtmp->my);
                        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "fire burns %s!", mon_nam(mtmp));
                    }
                    else if (verbose && !target)
                    {
                        play_sfx_sound_at_location(SFX_MONSTER_ON_FIRE, mtmp->mx, mtmp->my);
                        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "fire burns it!");
                    }
                    damage += adjust_damage(d(4, 6), (struct monst*)0, mtmp, AD_FIRE, ADFLAGS_NONE);
                }
                if (is_ammo(otmp) || throwing_weapon(otmp) || objects[otmp->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                    otmp->elemental_enchantment = 0;
                break;
            case LIGHTNING_ENCHANTMENT:
                if (is_mon_immune_to_elec(mtmp)) 
                {
                    if (vis)
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "lightning doesn't seem to affect %s.",
                            mon_nam(mtmp));
                    }
                }
                else
                {
                    if (vis)
                    {
                        play_sfx_sound_at_location(SFX_MONSTER_GETS_ZAPPED, mtmp->mx, mtmp->my);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is jolted by lightning!", Monnam(mtmp));
                    }
                    else if (verbose && !target)
                    {
                        play_sfx_sound_at_location(SFX_MONSTER_GETS_ZAPPED, mtmp->mx, mtmp->my);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "It is jolted by lightning!");
                    }

                    damage += adjust_damage(d(6, 6), (struct monst*)0, mtmp, AD_ELEC, ADFLAGS_NONE);
                }
                if (is_ammo(otmp) || throwing_weapon(otmp) || objects[otmp->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                    otmp->elemental_enchantment = 0;
                break;
            case DEATH_ENCHANTMENT:
                if (check_rider_death_absorption(mtmp, (const char*)0))
                {
                    /* no further action */
                }
                else if (resists_death(mtmp)) 
                {
                    if (vis)
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "death magic doesn't seem to affect %s.",
                            mon_nam(mtmp));
                    }
                }
                else 
                {
                    if (vis)
                    {
                        play_sfx_sound_at_location(SFX_MONSTER_IS_HIT_WITH_DEATH_MAGIC, mtmp->mx, mtmp->my);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is slain!", Monnam(mtmp));

                    }
                    else if (verbose && !target)
                    {
                        play_sfx_sound_at_location(SFX_MONSTER_IS_HIT_WITH_DEATH_MAGIC, mtmp->mx, mtmp->my);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "It is slain!");
                    }
                    damage = (double)mtmp->mhp + 1;
                }
                otmp->elemental_enchantment = 0;
                break;
            default:
                break;

            }
        }

        if (otmp->material == MAT_SILVER
            && mon_hates_silver(mtmp))
        {
            if (vis)
                pline_The_ex(ATR_NONE, CLR_MSG_MYSTICAL, "silver sears %s flesh!", s_suffix(mon_nam(mtmp)));
            else if (verbose && !target)
                pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "Its flesh is seared!");
        }

        if (otmp->otyp == ACID_VENOM && cansee(mtmp->mx, mtmp->my)) 
        {
            if (is_mon_immune_to_acid(mtmp)) 
            {
                if (vis || (verbose && !target))
                {
                    play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected.", Monnam(mtmp));
                }
            } 
            else 
            {
                if (vis)
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s burns %s!", hliquid("acid"), mon_nam(mtmp));
                else if (verbose && !target)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "It is burned!");
            }
        }

        if (otmp->otyp == EGG && touch_petrifies(&mons[otmp->corpsenm])) 
        {
            if (check_magic_cancellation_success(mtmp, 0) || resists_ston(mtmp))
            {
                play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mtmp->mx, mtmp->my);
                m_shieldeff(mtmp);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s resists!", Monnam(mtmp));
            }
            else
            {
                start_delayed_petrification(mtmp, FALSE);
            }
            if (resists_ston(mtmp))
                damage = 0;
        }

        if (!DEADMONSTER(mtmp))
        { /* might already be dead (if petrified) */
            deduct_monster_hp(mtmp, damage);
            //mtmp->mhp -= dmg;
            if (DEADMONSTER(mtmp))
            {
                if (poisondamage > 0 && (((double)mtmp->mhp + ((double)mtmp->mhp_fraction)/10000) - 1) > -poisondamage && vis)
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "poison was deadly...");
                if (vis || (verbose && !target))
                    pline("%s is %s!", Monnam(mtmp),
                          (is_not_living(mtmp->data) || is_vampshifter(mtmp)
                           || !canspotmon(mtmp)) ? "destroyed" : "killed");
                /* don't blame hero for unknown rolling boulder trap */
                if (!context.mon_moving && (otmp->otyp != BOULDER
                                            || range >= 0 || otmp->otrapped))
                    xkilled(mtmp, XKILL_NOMSG);
                else
                    mondied(mtmp);
            }
        }

        /* blinding venom and cream pie do 0 damage, but verify
           that the target is still alive anyway */
        if (!DEADMONSTER(mtmp)
            && can_blnd((struct monst *) 0, mtmp,
                        (uchar) ((otmp->otyp == BLINDING_VENOM) ? AT_SPIT
                                                                : AT_WEAP),
                        otmp)) 
        {
            if (vis && !is_blinded(mtmp))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is blinded by %s.", Monnam(mtmp), the(xname(otmp)));
            increase_mon_property(mtmp, BLINDED, rnd(25) + 20);
        }

        objgone = drop_throw(otmp, 1, bhitpos.x, bhitpos.y);

        if (!objgone && range == -1)
        { /* special case */
            obj_extract_self(otmp);    /* free it for motion again */
            return 0;
        }
        return 1;
    }
    return 0;
}

#define MT_FLIGHTCHECK(pre)                                             \
    (/* missile hits edge of screen */                                  \
     !isok(bhitpos.x + dx, bhitpos.y + dy)                              \
     /* missile hits the wall */                                        \
     || IS_ROCK(levl[bhitpos.x + dx][bhitpos.y + dy].typ)               \
     /* missile hit closed door */                                      \
     || closed_door(bhitpos.x + dx, bhitpos.y + dy)                     \
     /* missile might hit iron bars */                                  \
     /* the random chance for small objects hitting bars is */          \
     /* skipped when reaching them at point blank range */              \
     || (levl[bhitpos.x + dx][bhitpos.y + dy].typ == IRONBARS           \
         && hits_bars(&singleobj,                                       \
                      bhitpos.x, bhitpos.y,                             \
                      bhitpos.x + dx, bhitpos.y + dy,                   \
                      ((pre) ? 0 : !rn2(5)), 0))                        \
     /* Thrown objects "sink" */                                        \
     || (!(pre) && IS_SINK(levl[bhitpos.x][bhitpos.y].typ)))

void
m_throw(mon, x, y, dx, dy, range, obj)
struct monst *mon;       /* launching monster */
int x, y, dx, dy, range; /* launch point, direction, and range */
struct obj *obj;         /* missile (or stack providing it) */
{
    struct monst *mtmp;
    struct obj *singleobj;
    char sym = obj->oclass;
    int hitu = 0, oldumort, blindinc = 0;

    bhitpos.x = x;
    bhitpos.y = y;
    notonhead = FALSE; /* reset potentially stale value */

    if (obj->quan == 1L) {
        /*
         * Remove object from minvent.  This cannot be done later on;
         * what if the player dies before then, leaving the monster
         * with 0 daggers?  (This caused the infamous 2^32-1 orcish
         * dagger bug).
         *
         * VENOM is not in minvent - it should already be OBJ_FREE.
         * The extract below does nothing.
         */

        /* not possibly_unwield, which checks the object's */
        /* location, not its existence */
        if (MON_WEP(mon) == obj)
            setmnotwielded(mon, obj);
        obj_extract_self(obj);
        singleobj = obj;
        obj = (struct obj *) 0;
    } else {
        singleobj = splitobj(obj, 1L);
        obj_extract_self(singleobj);
    }

    singleobj->owornmask = 0; /* threw one of multiple weapons in hand? */

    if ((singleobj->cursed || singleobj->greased) && (dx || dy) && !rn2(7)) {
        if (canseemon(mon) && flags.verbose) {
            if (is_ammo(singleobj))
                pline("%s misfires!", Monnam(mon));
            else
                pline("%s as %s throws it!", Tobjnam(singleobj, "slip"),
                      mon_nam(mon));
        }
        dx = rn2(3) - 1;
        dy = rn2(3) - 1;
        /* check validity of new direction */
        if (!dx && !dy) {
            (void) drop_throw(singleobj, 0, bhitpos.x, bhitpos.y);
            return;
        }
    }

    if (MT_FLIGHTCHECK(TRUE)) 
    {
        (void) drop_throw(singleobj, 0, bhitpos.x, bhitpos.y);
        return;
    }
    mesg_given = 0; /* a 'missile misses' message has not yet been shown */

    /* Note: drop_throw may destroy singleobj.  Since obj must be destroyed
     * early to avoid the dagger bug, anyone who modifies this code should
     * be careful not to use either one after it's been freed.
     */
    if (sym)
        tmp_at(DISP_FLASH, obj_to_missile_glyph(singleobj, get_missile_index(dx, dy), rn2_on_display_rng));
    
    while (range-- > 0)
    { /* Actually the loop is always exited by break */
        xchar lastpos_x = bhitpos.x;
        xchar lastpos_y = bhitpos.y;

        bhitpos.x += dx;
        bhitpos.y += dy;
        if ((mtmp = m_at(bhitpos.x, bhitpos.y)) != 0) {
            if (ohitmon(mtmp, singleobj, range, TRUE))
                break;
        } else if (bhitpos.x == u.ux && bhitpos.y == u.uy) {
            if (multi)
                nomul(0);

            if (singleobj->oclass == GEM_CLASS
                && singleobj->otyp <= LAST_GEM + 9 /* 9 glass colors */
                && is_unicorn(youmonst.data)) {
                if (singleobj->otyp > LAST_GEM) {
                    You("catch the %s.", xname(singleobj));
                    You("are not interested in %s junk.",
                        s_suffix(mon_nam(mon)));
                    makeknown(singleobj->otyp);
                    dropy(singleobj);
                } else {
                    You(
                     "accept %s gift in the spirit in which it was intended.",
                        s_suffix(mon_nam(mon)));
                    (void) hold_another_object(singleobj,
                                               "You catch, but drop, %s.",
                                               xname(singleobj),
                                               "You catch:");
                }
                break;
            }
            if (singleobj->oclass == POTION_CLASS) {
                if (!Blind)
                    singleobj->dknown = 1;
                potionhit(&youmonst, &singleobj, POTHIT_MONST_THROW);
                break;
            }
            oldumort = u.umortality;

            int dam, hitv;
            int mindistance = 0;

            switch (singleobj->otyp) {
            case EGG:
                if (!touch_petrifies(&mons[singleobj->corpsenm])) {
                    impossible("monster throwing egg type %d",
                               singleobj->corpsenm);
                    hitu = 0;
                    break;
                }
            /* fall through */
            case CREAM_PIE:
            case BLINDING_VENOM:
                hitu = thitu(8, 0, &singleobj, (char *) 0);
                break;
            default:
                if (is_launcher(singleobj))
                    dam = d(1, 2);
                else
                    dam = weapon_total_dmg_value(singleobj, &youmonst, mon, 1);

                mindistance = distmin(u.ux, u.uy, mon->mx, mon->my);
                hitv = 3 - mindistance;
                if (hitv < -4)
                    hitv = -4;
                if (is_elf(mon->data)
                    && objects[singleobj->otyp].oc_skill == P_BOW) {
                    hitv++;
                    if (MON_WEP(mon) && MON_WEP(mon)->otyp == ELVEN_LONG_BOW)
                        hitv++;
                    if (singleobj->otyp == ELVEN_ARROW)
                        dam++;
                }

                //SEPARATE POINT BLANK ADJUSTMENT HERE, MORE ON AMMO_AND_LAUNCHER BELOW
                //Using wielded long-rangen weapons at black point is difficult
                if (mindistance <= 1) {
                    if (singleobj && MON_WEP(mon) && ammo_and_launcher(singleobj, MON_WEP(mon)))
                    {
                        switch (objects[MON_WEP(mon)->otyp].oc_skill) {
                        case P_BOW:
                            hitv -= BOW_TO_HIT_MELEE_PENALTY;
                            break;
                        case P_CROSSBOW:
                            hitv -= (BOW_TO_HIT_MELEE_PENALTY * 8) / 10;
                            break;
                        default:
                            hitv -= BOW_TO_HIT_MELEE_PENALTY;
                            break;
                        }
                        //Bracers here, if need be
                    }
                    else if(singleobj && throwing_weapon(singleobj))
                    {
                        hitv -= THROWN_WEAPON_TO_HIT_MELEE_PENALTY;
                    }
                }
                //All cases get dex ranged to-hit bonus
                hitv += m_ranged_strdex_to_hit_bonus(mon);

                //Give bow damage bonuses
                if(singleobj && is_ammo(singleobj))
                {
                    if(MON_WEP(mon) && ammo_and_launcher(singleobj, MON_WEP(mon)))
                    {
                        //Fitting ammo gets launcher's weapon_to_hit_value and weapon_dmg_value and str damage bonus if bow, fixed for crossbows
                        //LAUNCHER HITVAL
                        hitv += weapon_to_hit_value(MON_WEP(mon), &youmonst, mon, 2); //MON_WEP(mon)->enchantment - greatest_erosion(MON_WEP(mon));
                        //hitv += weapon_skill_hit_bonus(MON_WEP(mon)); //Monsters do not get skill bonuses
                        //LAUNCHER DMGVAL
                        dam += weapon_total_dmg_value(MON_WEP(mon), &youmonst, mon, 2);

                        //Give strength damage bonus
                        if (objects[MON_WEP(mon)->otyp].oc_flags3 & O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH)
                            dam += objects[MON_WEP(mon)->otyp].oc_fixed_damage_bonus;
                        else 
                            dam += m_str_dmg_bonus(mon);
                    }
                    else
                    {
                        //Non-fitting ammo gets to-hit penalty and no str damage bonus
                        hitv -= 4;
                    }
                }
                else
                {
                    //Non-ammo (normal thrown weapon) gets damage bonus
                    dam += m_thrown_str_dmg_bonus(mon);
                }
                if (bigmonst(youmonst.data))
                    hitv++;
                hitv += 10 + weapon_to_hit_value(singleobj, &youmonst, mon, 1);
                hitv += mon->m_lev + mon->mhitinc;
                dam += mon->mdaminc;
                if (dam < 1)
                    dam = 1;
                hitu = thitu(hitv, dam, &singleobj, (char *) 0);
            }

            if (hitu && singleobj->opoisoned && is_poisonable(singleobj))
            {
                char onmbuf[BUFSZ], knmbuf[BUFSZ];

                Strcpy(onmbuf, xname(singleobj));
                Strcpy(knmbuf, killer_xname(singleobj));
                poisoned(onmbuf, A_STR, knmbuf,
                         /* if damage triggered life-saving,
                            poison is limited to attrib loss */
                           0, TRUE, 2);
            }

            if (hitu && singleobj->elemental_enchantment)
            {
                char onmbuf[BUFSZ], knmbuf[BUFSZ];

                Strcpy(onmbuf, xname(singleobj));
                Strcpy(knmbuf, killer_xname(singleobj));
                
                /* if damage triggered life-saving,
                   major death magic is limited to minor death magic */
                extra_enchantment_damage(onmbuf, singleobj->elemental_enchantment, knmbuf, (u.umortality > oldumort));
            
                //Remove special enchantment
                switch (singleobj->elemental_enchantment)
                {
                case COLD_ENCHANTMENT:
                    if (is_ammo(singleobj) || throwing_weapon(singleobj) || objects[singleobj->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                        singleobj->elemental_enchantment = 0;
                    break;
                case FIRE_ENCHANTMENT:
                    if (is_ammo(singleobj) || throwing_weapon(singleobj) || objects[singleobj->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                        singleobj->elemental_enchantment = 0;
                    break;
                case LIGHTNING_ENCHANTMENT:
                    if (is_ammo(singleobj) || throwing_weapon(singleobj) || objects[singleobj->otyp].oc_merge ? 1 : !rn2(ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL))
                        singleobj->elemental_enchantment = 0;
                    break;
                case DEATH_ENCHANTMENT:
                    singleobj->elemental_enchantment = 0;
                    break;
                default:
                    break;
                }
            }


            if (hitu && can_blnd((struct monst *) 0, &youmonst,
                                 (uchar) ((singleobj->otyp == BLINDING_VENOM)
                                             ? AT_SPIT
                                             : AT_WEAP),
                                 singleobj))
            {
                blindinc = rnd(25);
                if (singleobj->otyp == CREAM_PIE) {
                    if (!Blind)
                        pline("Yecch!  You've been creamed.");
                    else
                        pline("There's %s sticky all over your %s.",
                              something, body_part(FACE));
                } else if (singleobj->otyp == BLINDING_VENOM) {
                    const char *eyes = body_part(EYE);

                    if (eyecount(youmonst.data) != 1)
                        eyes = makeplural(eyes);
                    /* venom in the eyes */
                    if (!Blind)
                        pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "venom blinds you.");
                    else
                        Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s.", eyes, vtense(eyes, "sting"));
                }
            }
            if (hitu && singleobj->otyp == EGG) {
                if (!Stoned && !Stone_resistance
                    && !(poly_when_stoned(youmonst.data)
                         && polymon(PM_STONE_GOLEM))) {
                    make_stoned(5L, (char *) 0, KILLED_BY, "", HINT_KILLED_HIT_BY_COCKATRICE_CORPSE);
                }
            }
            stop_occupation();
            if (hitu)
            {
                (void) drop_throw(singleobj, hitu, u.ux, u.uy);
                break;
            }
        }

        if (!range /* reached end of path */
            || MT_FLIGHTCHECK(FALSE)) 
        {
            if (singleobj) { /* hits_bars might have destroyed it */
                if (m_shot.n > 1
                    && (!mesg_given || bhitpos.x != u.ux || bhitpos.y != u.uy)
                    && (cansee(bhitpos.x, bhitpos.y)
                        || (archer && canseemon(archer))))
                    pline("%s misses.", The(mshot_xname(singleobj)));
                (void) drop_throw(singleobj, 0, bhitpos.x, bhitpos.y);
            }
            break;
        }

        tmp_at(bhitpos.x, bhitpos.y);
        if (singleobj && ((is_poisonable(singleobj) && singleobj->opoisoned) || singleobj->special_quality != 0 || singleobj->elemental_enchantment || singleobj->exceptionality || singleobj->mythic_prefix || singleobj->mythic_suffix || singleobj->oeroded || singleobj->oeroded2 || get_obj_height(singleobj) > 0))
        {
            show_missile_info(bhitpos.x, bhitpos.y, singleobj->opoisoned, singleobj->material, singleobj->special_quality, singleobj->elemental_enchantment, singleobj->exceptionality, singleobj->mythic_prefix, singleobj->mythic_suffix, singleobj->oeroded, singleobj->oeroded2, get_missile_flags(singleobj, FALSE), get_obj_height(singleobj), 0, 0);
            flush_screen(1);
        }
        if (isok(lastpos_x, lastpos_y))
            show_missile_info(lastpos_x, lastpos_y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0UL, 0, 0, 0); /* Clear missile info out in the previous location */
        adjusted_delay_output();
    }

    /* Note: singleobj may be a dangling pointer now */
    //tmp_at(bhitpos.x, bhitpos.y);
    //adjusted_delay_output();
    
    tmp_at(DISP_END, 0);
    mesg_given = 0; /* reset */

    if (blindinc) 
    {
        u.ucreamed += blindinc;
        make_blinded(Blinded + (long) blindinc, FALSE);
        if (!Blind)
            Your1(vision_clears);
    }
}

#undef MT_FLIGHTCHECK

/* Monster throws item at another monster */
int
thrwmm(mtmp, mtarg)
struct monst *mtmp, *mtarg;
{
    struct obj *otmp, *mwep;
    register xchar x, y;
    boolean ispole;

    /* Polearms won't be applied by monsters against other monsters */
    if (mtmp->weapon_strategy == NEED_WEAPON || !MON_WEP(mtmp)) {
        mtmp->weapon_strategy = NEED_RANGED_WEAPON;
        /* mon_wield_item resets weapon_strategy as appropriate */
        if (mon_wield_item(mtmp, FALSE, mtarg->mx, mtarg->my) != 0)
            return 0;
    }

    /* Pick a weapon */
    otmp = select_rwep(mtmp);
    if (!otmp)
        return 0;
    ispole = is_appliable_pole_type_weapon(otmp);

    x = mtmp->mx;
    y = mtmp->my;

    mwep = MON_WEP(mtmp); /* wielded weapon */
    int weprange = m_weapon_range(mtmp, otmp, mwep);

    if (!ispole && m_lined_up(mtarg, mtmp, TRUE, AD_PHYS, FALSE, weprange))
    {
        int chance = max(M_SHOOT_CHANCE_RANGE - distmin(x, y, mtarg->mx, mtarg->my), 1);

        if (!is_fleeing(mtarg) || !rn2(chance))
        {
            if (ammo_and_launcher(otmp, mwep)
                && dist2(mtmp->mx, mtmp->my, mtarg->mx, mtarg->my)
                   > PET_MISSILE_RANGE2) //Monsters shoot a range of 8
                return 0; /* Out of range */
            /* Set target monster */
            target = mtarg;
            archer = mtmp;
            update_m_facing(mtmp, mtarg->mx - mtmp->mx, TRUE);
            monshoot(mtmp, otmp, mwep); /* multishot shooting or throwing */
            archer = target = (struct monst *) 0;
            nomul(0);
            return 1;
        }
    }
    return 0;
}

/* monster spits substance at monster */
int
spitmm(mtmp, mattk, mtarg)
struct monst *mtmp, *mtarg;
struct attack *mattk;
{
    if (!mtmp || !mtarg || !mattk)
        return 0;

    struct obj *otmp;
    int range = mattk->range ? mattk->range : M_GENERIC_RANGED_ATTACK_RANGE;

    if (is_cancelled(mtmp)) {
        if (!Deaf)
            pline("A dry rattle comes from %s throat.",
                  s_suffix(mon_nam(mtmp)));
        return 0;
    }
    if (m_lined_up(mtarg, mtmp, TRUE, mattk->adtyp, FALSE, range)
        && !is_immune(mtarg, mattk->adtyp))
    {
        update_m_facing(mtmp, mtarg->mx - mtmp->mx, TRUE);

        switch (mattk->adtyp) {
        case AD_BLND:
        case AD_DRST:
            otmp = mksobj(BLINDING_VENOM, TRUE, FALSE, FALSE);
            break;
        default:
            impossible("bad attack type in spitmu");
            /* fall through */
        case AD_ACID:
            otmp = mksobj(ACID_VENOM, TRUE, FALSE, FALSE);
            break;
        }
        if (!rn2(max(1, M_SHOOT_CHANCE_RANGE - distmin(mtmp->mx,mtmp->my,mtarg->mx,mtarg->my))))
        {
            if (canseemon(mtmp))
                pline("%s spits venom!", Monnam(mtmp));
            target = mtarg;
            m_throw(mtmp, mtmp->mx, mtmp->my, sgn(tbx), sgn(tby),
                    distmin(mtmp->mx,mtmp->my,mtarg->mx,mtarg->my), otmp);
            target = (struct monst *)0;
            nomul(0);

            /* If this is a pet, it'll get hungry. Minions and
             * spell beings won't hunger */
            if (mtmp->mtame && !mtmp->isminion) {
                struct edog *dog = EDOG(mtmp);

                /* Hunger effects will catch up next move */
                if (dog->hungrytime > 1)
                    dog->hungrytime -= 5;
            }

            return 1;
        }
    }
    return 0;
}

/* monster breathes at monster (ranged) */
int
breamm(mtmp, mattk, mtarg)
struct monst *mtmp, *mtarg;
struct attack  *mattk;
{
    if (!mtmp || !mtarg || !mattk)
        return 0;


    /* if new breath types are added, change AD_ACID to max type */
    int typ = get_ray_adtyp(mattk->adtyp); // Does not include death ray
    int range = mattk->range ? mattk->range : M_BREATH_WEAPON_RANGE;

    if (m_lined_up(mtarg, mtmp, TRUE, typ, TRUE, range)
        && (typ != mattk->adtyp || !is_immune(mtarg, typ)))
    {
        update_m_facing(mtmp, mtarg->mx - mtmp->mx, TRUE);
        if (is_cancelled(mtmp))
        {
            if (!Deaf) {
                if (canseemon(mtmp))
                    pline("%s coughs.", Monnam(mtmp));
                else
                    You_hear("a cough.");
            }
            return 0;
        }
        if (!mtmp->mspec_used && rn2(3)) 
        {
            if ((typ >= AD_MAGM) && (typ <= AD_STON))
            {
                if (canseemon(mtmp))
                    pline("%s breathes %s!", Monnam(mtmp), breathwep[typ - 1]);
                dobuzz((int) (-20 - (typ - 1)), (struct obj*)0, mtmp, (int) mattk->damn, (int)mattk->damd, (int)mattk->damp,
                       mtmp->mx, mtmp->my, sgn(tbx), sgn(tby), FALSE);
                nomul(0);
                /* breath runs out sometimes. Also, give monster some
                 * cunning; don't breath if the target fell asleep.
                 */
                mtmp->mspec_used = (5 + rn2(10));

                /* If this is a pet, it'll get hungry. Minions and
                 * spell beings won't hunger */
                if (mtmp->mtame && !mtmp->isminion) 
                {
                    struct edog *dog = EDOG(mtmp);

                    /* Hunger effects will catch up next move */
                    if (dog->hungrytime >= 10)
                        dog->hungrytime -= 10;
                }
            } else impossible("Breath weapon %d used", typ-1);
        } else
            return 0;
    }
    return 1;
}


/* gazer gazes at monster (ranged) */
int
eyesmm(mtmp, mattk, mtarg)
struct monst* mtmp, * mtarg;
struct attack* mattk;
{
    if (!mtmp || !mtarg || !mattk)
        return 0;

    int typ = get_ray_adtyp(mattk->adtyp);
    int range = mattk->range ? mattk->range : M_RAY_RANGE;

    if (m_lined_up(mtarg, mtmp, TRUE, typ, TRUE, range)
        && (typ != mattk->adtyp || !is_immune(mtarg, typ)))
    {
        update_m_facing(mtmp, mtarg->mx - mtmp->mx, TRUE);
        if (is_cancelled(mtmp) || is_blinded(mtmp))
        {
            if (canseemon(mtmp))
                pline("%s blinks.", Monnam(mtmp));
            return 0;
        }

        if ((typ >= AD_MAGM) && (typ <= AD_STON))
        {
            if (canseemon(mtmp))
                pline("One of %s eyestalks fires %s!", s_suffix(mon_nam(mtmp)), eyestalk[typ - 1]);
            dobuzz((int)(-30 - (typ - 1)), (struct obj*)0, mtmp, (int)mattk->damn, (int)mattk->damd, (int)mattk->damp,
                mtmp->mx, mtmp->my, sgn(tbx), sgn(tby), FALSE);
            nomul(0);
        }
        else 
            impossible("Eyestalk %d used", typ - 1);
    }
    return 1;
}

const char* get_eyestalk_ray_name(typ)
int typ;
{
    return eyestalk[typ - 1];
}

const char* get_breath_weapon_name(typ)
int typ;
{
    return breathwep[typ - 1];
}

/* monster casts a ray spell at monster (ranged) */
int
buzzmm(mtmp, mattk, mtarg)
struct monst* mtmp, * mtarg;
struct attack* mattk;
{
    if (!mtmp || !mattk)
        return 0;
        
    /* Do not waste magic on reflecting targets */
    if (is_reflecting(mtarg) && rn2(5))
        return 0;

    int adtyp = 0;
    int damn = 0, damd = 0, damp = 0;
    int range = mattk->range ? mattk->range : M_RAY_RANGE;

    set_m_ray_spell_stats(mtmp, mattk, mtarg, &adtyp, &damn, &damd, &damp);

    if (adtyp < AD_MAGM || adtyp > AD_STON)
        return 0;

    if (m_lined_up(mtarg, mtmp, TRUE, adtyp, TRUE, range))
    {
        update_m_facing(mtmp, mtarg->mx - mtmp->mx, TRUE);
        if (is_cancelled(mtmp))
        {
            if (!Deaf) 
            {
                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_CURSE);
                if (canseemon(mtmp))
                {
                    pline("%s curses at %s.", Monnam(mtmp), mon_nam(mtarg));
                }
                else
                    You_hear("a curse.");
            }
            return 0;
        }
        if (!mtmp->mspec_used && rn2(3))
        {
            if ((adtyp >= AD_MAGM) && (adtyp <= AD_STON))
            {
                if (canseemon(mtmp))
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s casts \'%s\' at %s!", Monnam(mtmp),
                        flash_types[ad_to_typ(adtyp)], mon_nam(mtarg));

                dobuzz((int)(-ad_to_typ(adtyp)), (struct obj*)0, mtmp, damn, damd, damp,
                    mtmp->mx, mtmp->my, sgn(tbx), sgn(tby), FALSE);

                nomul(0);

                mtmp->mspec_used = rnd(3);

                /* If this is a pet, it'll get hungry. Minions and
                 * spell beings won't hunger */
                if (mtmp->mtame && !mtmp->isminion)
                {
                    struct edog* dog = EDOG(mtmp);

                    /* Hunger effects will catch up next move */
                    if (dog->hungrytime >= 10)
                        dog->hungrytime -= 10;
                }
            }
            else impossible("Monster ray spell %d used", adtyp - 1);
        }
        else
            return 0;
    }
    return 1;
}

void
set_m_ray_spell_stats(mtmp, mattk, mtarg, typ, damn, damd, damp)
struct monst* mtmp;
struct attack* mattk;
struct monst* mtarg;
int *typ, *damn, *damd, *damp;
{
    /* Currently only arcane spell-casters */
    if (mattk->adtyp != AD_SPEL)
    {
        *typ = 0;
        return;
    }

    //boolean is_target_you = (mtarg == &youmonst);
    boolean spell_ok[10] = { 0 };
    int i;
    for (i = AD_MAGM; i <= AD_STON; i++)
    {
        spell_ok[i - 1] = !is_immune(mtarg, i);

    }

    int ml = 0;
    if (mattk->mcadj > 0)
        ml = mattk->mcadj;
    else
        ml = mtmp->m_lev;

    if (ml <= 2)
    {
        *typ = 0;
        return;
    }
    else if (ml <= 6)
    {
        /* low level monsters can cast magic missile */
        *typ = AD_MAGM;
        *damn = 2;
        *damd = 6;
        *damp = 0;
        return;
    }

    /* Do not use spells that potentially kills the caster */
    spell_ok[AD_STON - 1] = FALSE;
    spell_ok[AD_DISN - 1] = FALSE;

    /* No spells for these */
    spell_ok[AD_ACID - 1] = FALSE;
    spell_ok[AD_DRST - 1] = FALSE;

    if (ml <= 25 && mtmp->mnum != PM_ARCH_LICH)
    {
        spell_ok[AD_DRAY - 1] = FALSE;
    }

    if (ml <= 15 && mtmp->mnum != PM_ARCH_LICH && mtmp->mnum != PM_MASTER_LICH)
    {
        spell_ok[AD_COLD - 1] = FALSE;
    }

    if (ml <= 10)
    {
        spell_ok[AD_ELEC - 1] = FALSE;
    }


    if (mattk->adtyp != AD_SPEL)
    {
        spell_ok[AD_DISN - 1] = FALSE;
        spell_ok[AD_STON - 1] = FALSE;
    }

    int sum = 0;
    for (i = 0; i < 10; i++)
        sum += spell_ok[i];

    /* Too weak if others are available */
    if (ml >= 11 && ml < 20 && (sum - spell_ok[AD_SLEE - 1]) > 1 && spell_ok[AD_MAGM - 1])
    {
        spell_ok[AD_MAGM - 1] = FALSE;
        sum--;
    }

    if (ml >= 16 && (sum - spell_ok[AD_SLEE - 1]) > 1 && spell_ok[AD_FIRE - 1])
    {
        spell_ok[AD_FIRE - 1] = FALSE;
        sum--;
    }

    if (ml >= 20 && (sum - spell_ok[AD_SLEE - 1]) > 1 && spell_ok[AD_ELEC - 1])
    {
        spell_ok[AD_ELEC - 1] = FALSE;
        sum--;
    }

    if (sum <= 0)
    {
        *typ = 0;
        return;
    }

    int sel_spell = rnd(sum);
    int sel_adtyp = 0;
    do
    {
        sel_adtyp++;
        
        if (sel_adtyp > 10)
            break;

        if(spell_ok[sel_adtyp - 1] == TRUE)
            sel_spell--;

    } while (sel_spell > 0);

    if (sel_adtyp <= 0 || sel_adtyp > 10)
    {
        *typ = 0;
        return;
    }

    *typ = sel_adtyp;

    switch (sel_adtyp)
    {
    case AD_MAGM:
        *damn = ml >= 15 ? 16 : 2; /* larger damage is greater magic missile */
        *damd = 6;
        *damp = 0;
        break;
    case AD_FIRE:
        *damn = 4;
        *damd = 6;
        *damp = 0;
        break;
    case AD_ELEC:
        *damn = 6;
        *damd = 6;
        *damp = 0;
        break;
    case AD_COLD:
        *damn = 12;
        *damd = 6;
        *damp = 0;
        break;
    case AD_DISN:
    case AD_DRAY:
    case AD_STON:
        *damn = 1;
        *damd = 255;
        *damp = 0;
        break;
    case AD_SLEE:
        *damn = 0;
        *damd = 0;
        *damp = 0;
        break;
    default:
        break;
    }

}

/* remove an entire item from a monster's inventory; destroy that item */
void
m_useupall(mon, obj)
struct monst *mon;
struct obj *obj;
{
    obj_extract_self(obj);
    if (obj->owornmask) {
        if (obj == MON_WEP(mon))
            mwepgone(mon);
        mon->worn_item_flags &= ~obj->owornmask;
        obj->owornmask = 0L;
        update_all_mon_statistics(mon, FALSE);
        if (mon == u.usteed && obj->otyp == SADDLE)
            dismount_steed(DISMOUNT_FELL);
    }
    obfree(obj, (struct obj *) 0);
}

/* remove one instance of an item from a monster's inventory */
void
m_useup(mon, obj)
struct monst *mon;
struct obj *obj;
{
    if (obj->quan > 1L) {
        obj->quan--;
        obj->owt = weight(obj);
    } else {
        m_useupall(mon, obj);
    }
}

/* monster attempts ranged weapon attack against player */
void
thrwmu(mtmp)
struct monst *mtmp;
{
    struct obj *otmp, *mwep;
    xchar x, y;
    const char *onm;

    if (!mtmp)
        return;

    /* Rearranged beginning so monsters can use polearms not in a line */
    if (mtmp->weapon_strategy == NEED_WEAPON || !MON_WEP(mtmp)) {
        mtmp->weapon_strategy = NEED_RANGED_WEAPON;
        /* mon_wield_item resets weapon_strategy as appropriate */
        if (mon_wield_item(mtmp, FALSE, u.ux, u.uy) != 0)
            return;
    }

    /* Pick a weapon */
    otmp = select_rwep(mtmp);
    if (!otmp)
        return;

    update_m_facing(mtmp, u.ux - mtmp->mx, TRUE);
    if (is_appliable_pole_type_weapon(otmp))
    {
        int dam, hitv;
        int min_range = 1, max_range = 2;
        get_pole_type_weapon_min_max_distances(otmp, mtmp, &min_range, &max_range);
        int distance2 = dist2(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy);

        if (otmp != MON_WEP(mtmp))
            return; /* polearm must be wielded */

        if (distance2 > max_range || distance2 < min_range
            || !couldsee(mtmp->mx, mtmp->my))
            return; /* Out of range, or intervening wall */

        update_m_action(mtmp, ACTION_TILE_ATTACK);
        play_monster_simple_weapon_sound(mtmp, 0, otmp, OBJECT_SOUND_TYPE_SWING_MELEE);
        m_wait_until_action();

        if (canseemon(mtmp)) {
            onm = xname(otmp);
            pline("%s thrusts %s.", Monnam(mtmp),
                  obj_is_pname(otmp) ? the(onm) : an(onm));
        }

        if (is_launcher(otmp))
            dam = d(1, 2);
        else
            dam = weapon_total_dmg_value(otmp, &youmonst, mtmp, 1);

        hitv = 3 - distmin(u.ux, u.uy, mtmp->mx, mtmp->my);
        if (hitv < -4)
            hitv = -4;
        if (bigmonst(youmonst.data))
            hitv++;
        hitv += 10 + weapon_to_hit_value(otmp, &youmonst, mtmp, 1);
        hitv += mtmp->m_lev + mtmp->mhitinc;
        hitv += m_strdex_to_hit_bonus(mtmp); // since a pole, str & dex to hit bonus
        dam += m_str_dmg_bonus(mtmp); // strength damage bonus
        dam += mtmp->mdaminc;

        if (dam < 1)
            dam = 1;

        (void) thitu(hitv, dam, &otmp, (char *) 0);

        update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
        stop_occupation();
        return;
    }

    x = mtmp->mx;
    y = mtmp->my;
    /* If you are coming toward the monster, the monster
     * should try to soften you up with missiles.  If you are
     * going away, you are probably hurt or running.  Give
     * chase, but if you are getting too far away, throw.
     */
    mwep = MON_WEP(mtmp); /* wielded weapon */
    int weprange = m_weapon_range(mtmp, otmp, mwep);
    if (!lined_up(mtmp, TRUE, AD_PHYS, FALSE, weprange)
        || (URETREATING(x, y)
            && rn2(max(1, M_SHOOT_CHANCE_RANGE - distmin(x, y, mtmp->mux, mtmp->muy)))))
        return;

    monshoot(mtmp, otmp, mwep); /* multishot shooting or throwing */
    nomul(0);
}

/* monster spits substance at you */
int
spitmu(mtmp, mattk)
struct monst *mtmp;
struct attack *mattk;
{
    if (!mtmp || !mattk)
        return 0;

    struct obj *otmp;
    int range = mattk->range ? mattk->range : M_GENERIC_RANGED_ATTACK_RANGE;

    if (is_cancelled(mtmp)) 
    {
        if (!Deaf)
            pline("A dry rattle comes from %s throat.",
                  s_suffix(mon_nam(mtmp)));
        return 0;
    }
    if (lined_up(mtmp, TRUE, mattk->adtyp, FALSE, range)
        && !is_immune(&youmonst, mattk->adtyp))
    {
        update_m_facing(mtmp, u.ux - mtmp->mx, TRUE);
        switch (mattk->adtyp)
        {
        case AD_BLND:
        case AD_DRST:
            otmp = mksobj(BLINDING_VENOM, TRUE, FALSE, FALSE);
            break;
        default:
            impossible("bad attack type in spitmu");
        /* fall through */
        case AD_ACID:
            otmp = mksobj(ACID_VENOM, TRUE, FALSE, FALSE);
            break;
        }
        if (otmp && !rn2(max(1, M_SHOOT_CHANCE_RANGE
                 - distmin(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy))))
        {
            boolean action_taken = FALSE;
            if (canseemon(mtmp))
            {
                action_taken = TRUE;
                update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : ACTION_TILE_FIRE);
            }
            play_monster_simple_weapon_sound(mtmp, get_pm_attack_index(mtmp->data, mattk), (struct obj*)0, OBJECT_SOUND_TYPE_FIRE);
            if (action_taken)
                m_wait_until_action();
            if (canseemon(mtmp))
                pline("%s spits venom!", Monnam(mtmp));
            m_throw(mtmp, mtmp->mx, mtmp->my, sgn(tbx), sgn(tby),
                    distmin(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy), otmp);
            nomul(0);
            if (action_taken)
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            return 0;
        }
        else 
        {
            obj_extract_self(otmp);
            obfree(otmp, (struct obj *) 0);
        }
    }
    return 0;
}

/* gazer fires an eyestalk at you (ranged) */
int
eyesmu(mtmp, mattk)
struct monst *mtmp;
struct attack *mattk;
{
    //Sanity check
    if (!mtmp || !mattk)
        return 0;

    int typ = get_ray_adtyp(mattk->adtyp);
    int range = mattk->range ? mattk->range : M_RAY_RANGE;

    if (lined_up(mtmp, TRUE, typ, TRUE, range)
        && (typ != mattk->adtyp || !is_immune(&youmonst, typ)))
    {
        update_m_facing(mtmp, u.ux - mtmp->mx, TRUE);
        if (is_cancelled(mtmp) || is_blinded(mtmp))
        {
            if (canseemon(mtmp))
                pline("%s blinks.", Monnam(mtmp));
            return 0;
        }
        if ((typ >= AD_MAGM) && (typ <= AD_STON))
        {
            boolean action_taken = FALSE;
            if (canseemon(mtmp))
            {
                action_taken = TRUE;
                update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
            }
            play_monster_simple_weapon_sound(mtmp, get_pm_attack_index(mtmp->data, mattk), (struct obj*)0, OBJECT_SOUND_TYPE_FIRE);
            if (action_taken)
                m_wait_until_action();

            if (canseemon(mtmp))
                pline("One of %s eyestalks fires %s!", s_suffix(mon_nam(mtmp)),
                        eyestalk[typ - 1]);
            buzz((int)(-30 - (typ - 1)), (struct obj*)0, mtmp, (int)mattk->damn, (int)mattk->damd, (int)mattk->damp, mtmp->mx,
                    mtmp->my, sgn(tbx), sgn(tby));
            nomul(0);
            if (action_taken)
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
        }
        else
            impossible("Eyestalk %d used", typ - 1);
    }
    return DEADMONSTER(mtmp) ? 2 : 1;
}


/* monster breathes at you (ranged) */
int
breamu(mtmp, mattk)
struct monst* mtmp;
struct attack* mattk;
{
    //Sanity check
    if (!mtmp || !mattk)
        return 0;

    /* if new breath types are added, change AD_ACID to max type */
    int typ = get_ray_adtyp(mattk->adtyp); //NOTE: Does not include death ray
    int range = mattk->range ? mattk->range : M_BREATH_WEAPON_RANGE;

    if (lined_up(mtmp, TRUE, typ, TRUE, range) 
        && (typ != mattk->adtyp || !is_immune(&youmonst, typ)))
    {
        update_m_facing(mtmp, u.ux - mtmp->mx, TRUE);
        if (is_cancelled(mtmp))
        {
            if (!Deaf) 
            {
                if (canseemon(mtmp))
                pline("%s coughs.", Monnam(mtmp));
                else
                You_hear("a cough.");
            }
            return 0;
        }
        if (!mtmp->mspec_used && rn2(3))
        {
            if ((typ >= AD_MAGM) && (typ <= AD_STON))
            {
                boolean action_taken = FALSE;
                if (canspotmon(mtmp))
                {
                    action_taken = TRUE;
                    update_m_action(mtmp, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
                }

                play_monster_simple_weapon_sound(mtmp, get_pm_attack_index(mtmp->data, mattk), (struct obj*)0, OBJECT_SOUND_TYPE_FIRE);

                if (action_taken)
                    m_wait_until_action();

                if (canseemon(mtmp))
                    pline("%s breathes %s!", Monnam(mtmp),
                        breathwep[typ - 1]);
                buzz((int)(-20 - (typ - 1)), (struct obj*)0, mtmp, (int)mattk->damn, (int)mattk->damd, (int)mattk->damp, mtmp->mx,
                    mtmp->my, sgn(tbx), sgn(tby));
                nomul(0);
                /* breath runs out sometimes. Also, give monster some
                 * cunning; don't breath if the player fell asleep.
                 */
                //if (!rn2(3))
                mtmp->mspec_used = (5 + rn2(10));
                
                if (typ == AD_SLEE && !Sleep_resistance)
                    mtmp->mspec_used += rnd(20);
                
                if (action_taken)
                    update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            }
            else
                impossible("Breath weapon %d used", typ - 1);
        }
    }
    return DEADMONSTER(mtmp) ? 2 : 1;
}


boolean
linedup(ax, ay, bx, by, boulderhandling, block_if_hostile_monster, block_if_peaceful_monster, block_if_tame_monster, dmgtype, attack_can_reflect, range)
register xchar ax, ay, bx, by;
int boulderhandling; /* 0=block, 1=ignore, 2=conditionally block */
boolean block_if_hostile_monster;
boolean block_if_peaceful_monster;
boolean block_if_tame_monster;
int dmgtype;
boolean attack_can_reflect;
int range;
{
    int dx, dy, boulderspots;

    /* These two values are set for use after successful return. */
    tbx = ax - bx;
    tby = ay - by;

    /* sometimes displacement makes a monster think that you're at its
       own location; prevent it from throwing and zapping in that case */
    if (!tbx && !tby)
        return FALSE;

    if ((!tbx || !tby || abs(tbx) == abs(tby)) /* straight line or diagonal */
        && dist2(tbx, tby, 0, 0) <= range * range
        )
    {
        boolean path_clear_base = (ax == u.ux && ay == u.uy) ? (boolean)couldsee(bx, by)
            : clear_path(ax, ay, bx, by);

        if (path_clear_base /* no boulders */
            && !block_if_hostile_monster && !block_if_peaceful_monster && !block_if_tame_monster
            && !attack_can_reflect)/* no need to consider monsters */
            return TRUE;

        if (!path_clear_base /* boulders or otherwise blocked */
            && boulderhandling == 0) /* no need to check boulders or monsters separately */
            return FALSE;

        /* here we need to check either boulders or monsters, or both */

        /* don't have line of sight or need to check monsters, but might still be lined up
           if that lack of sight is due solely to boulders or monsters */
        dx = sgn(ax - bx), dy = sgn(ay - by);
        boulderspots = 0;
        do {
            /* <bx,by> is guaranteed to eventually converge with <ax,ay> */
            bx += dx, by += dy;
            if (IS_ROCK(levl[bx][by].typ) || closed_door(bx, by))
                return FALSE;
            if (bx != ax || by != ay)
            {
                if (MON_AT(bx, by))
                {
                    struct monst* mtmp = m_at(bx, by);
                    if (mtmp)
                    {
                        if (is_reflecting(mtmp) && attack_can_reflect)
                            return FALSE;
                        if (!is_immune(mtmp, dmgtype))
                        {
                            if (block_if_hostile_monster && !is_peaceful(mtmp))
                                return FALSE;
                            if (block_if_peaceful_monster && is_peaceful(mtmp) && !is_tame(mtmp))
                                return FALSE;
                            if (block_if_tame_monster && is_peaceful(mtmp) && is_tame(mtmp))
                                return FALSE;
                        }
                    }
                }
                else if (bx == u.ux && by == u.uy)
                {
                    if ((Reflecting || is_reflecting(&youmonst)) && attack_can_reflect)
                        return FALSE;
                    if (!is_immune(&youmonst, dmgtype))
                    {
                        if (block_if_tame_monster)
                            return FALSE;
                    }
                }
            }
            if (sobj_at(BOULDER, bx, by))
                ++boulderspots;

            if (boulderhandling == 0 && boulderspots > 0)
                return FALSE;

        } while (bx != ax || by != ay);
        
        /* reached target position without encountering obstacle */
        if (boulderspots > 0)
        {
            if (boulderhandling == 0)
                return FALSE; /* You should never get here */
            else if (boulderhandling == 1)
                return TRUE; /* All boulders were ignored */
            else if (boulderhandling == 2 && rn2(2 + boulderspots) < 2)
                return TRUE;
        }
        else
            return path_clear_base;
    }
    return FALSE;
}

STATIC_OVL boolean
m_lined_up(mtarg, mtmp, can_hit_others, dmgtype, attack_can_reflect, range)
struct monst *mtarg, *mtmp;
boolean can_hit_others;
int dmgtype;
boolean attack_can_reflect;
int range;
{

    boolean block_if_hostile_monster = 0;
    boolean block_if_peaceful_monster = 0;
    boolean block_if_tame_monster = 0;

    if (can_hit_others && !(Conflict || is_crazed(mtmp)))
    {
        block_if_hostile_monster = !is_peaceful(mtmp) && !mon_has_bloodlust(mtmp);
        block_if_peaceful_monster = !mon_has_bloodlust(mtmp) && mtmp->data->maligntyp >= 0;
        block_if_tame_monster = is_tame(mtmp);
    }


    return (linedup(mtarg->mx, mtarg->my, mtmp->mx, mtmp->my, 0, block_if_hostile_monster, block_if_peaceful_monster, block_if_tame_monster, dmgtype, attack_can_reflect, range));
}


/* is mtmp in position to use ranged attack? */
boolean
lined_up(mtmp, can_hit_others, dmgtype, attack_can_reflect, range)
register struct monst *mtmp;
boolean can_hit_others;
int dmgtype;
boolean attack_can_reflect;
int range;
{
    boolean ignore_boulders;

    /* hero concealment usually trumps monst awareness of being lined up */
    if (Upolyd && rn2(25)
        && (u.uundetected || (U_AP_TYPE != M_AP_NOTHING
                              && U_AP_TYPE != M_AP_MONSTER)))
        return FALSE;

    boolean block_if_hostile_monster = 0;
    boolean block_if_peaceful_monster = 0;
    boolean block_if_tame_monster = 0;

    if(can_hit_others && !(Conflict || is_crazed(mtmp)))
    {
        block_if_hostile_monster = !is_peaceful(mtmp) && !mon_has_bloodlust(mtmp);
        block_if_peaceful_monster = !mon_has_bloodlust(mtmp) && mtmp->data->maligntyp >= 0;
        block_if_tame_monster = is_tame(mtmp);
    }

    ignore_boulders = (throws_rocks(mtmp->data)
                       || m_carrying(mtmp, WAN_STRIKING));
    return linedup(mtmp->mux, mtmp->muy, mtmp->mx, mtmp->my,
                   ignore_boulders ? 1 : 2, block_if_hostile_monster, block_if_peaceful_monster, block_if_tame_monster, dmgtype, attack_can_reflect, range);
}

/* check if a monster is carrying a particular item */
struct obj *
m_carrying(mtmp, type)
struct monst *mtmp;
int type;
{
    register struct obj *otmp;

    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == type)
            return otmp;
    return (struct obj *) 0;
}

void
hit_bars(objp, objx, objy, barsx, barsy, your_fault, from_invent)
struct obj **objp;      /* *objp will be set to NULL if object breaks */
int objx, objy, barsx, barsy;
boolean your_fault, from_invent;
{
    struct obj *otmp = *objp;
    int obj_type = otmp->otyp;
    boolean unbreakable = (levl[barsx][barsy].wall_info & W_NONDIGGABLE) != 0;

    if (your_fault
        ? hero_breaks(otmp, objx, objy, from_invent)
        : breaks(otmp, objx, objy)) {
        *objp = 0; /* object is now gone */
        /* breakage makes its own noises */
        if (obj_type == POT_ACID) {
            if (cansee(barsx, barsy) && !unbreakable)
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "iron bars are dissolved!");
            else
                You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, Hallucination ? "angry snakes!" : "a hissing noise.");
            if (!unbreakable)
                dissolve_bars(barsx, barsy);
        }
    }
    else if (obj_type == BOULDER || obj_type == HEAVY_IRON_BALL)
        pline("Whang!");
    else if (otmp->oclass == COIN_CLASS
             || otmp->material == MAT_GOLD
             || otmp->material == MAT_SILVER
             || otmp->material == MAT_PLATINUM
        )
        pline("Clink!");
    else
        pline("Clonk!");
}

/* TRUE iff thrown/kicked/rolled object doesn't pass through iron bars */
boolean
hits_bars(obj_p, x, y, barsx, barsy, always_hit, whodidit)
struct obj **obj_p; /* *obj_p will be set to NULL if object breaks */
int x, y, barsx, barsy;
int always_hit; /* caller can force a hit for items which would fit through */
int whodidit;   /* 1==hero, 0=other, -1==just check whether it'll pass thru */
{
    struct obj *otmp = *obj_p;
    int obj_type = otmp->otyp;
    boolean hits = always_hit;

    if (!hits)
        switch (otmp->oclass) {
        case WEAPON_CLASS: {
            int oskill = objects[obj_type].oc_skill;

            hits = (oskill != -P_BOW && oskill != -P_CROSSBOW
                    && oskill != -P_THROWN_WEAPON
                    && oskill != P_THRUSTING_WEAPON); /* but not dagger */
            break;
        }
        case ARMOR_CLASS:
            hits = (objects[obj_type].oc_armor_category != ARM_GLOVES);
            break;
        case TOOL_CLASS:
            hits = (obj_type != SKELETON_KEY && obj_type != LOCK_PICK
                    && obj_type != CREDIT_CARD && obj_type != TALLOW_CANDLE
                    && obj_type != WAX_CANDLE && obj_type != MAGIC_CANDLE
                    && obj_type != TIN_WHISTLE && obj_type != MAGIC_WHISTLE);
            break;
        case ROCK_CLASS: /* includes boulder */
            if (obj_type != STATUE || mons[otmp->corpsenm].msize > MZ_TINY)
                hits = TRUE;
            break;
        case FOOD_CLASS:
            if (obj_type == CORPSE && mons[otmp->corpsenm].msize > MZ_TINY)
                hits = TRUE;
            else
                hits = (obj_type == MEAT_STICK
                        || obj_type == HUGE_CHUNK_OF_MEAT);
            break;
        case SPBOOK_CLASS:
        case REAGENT_CLASS:
        case WAND_CLASS:
        case BALL_CLASS:
        case CHAIN_CLASS:
        case ART_CLASS:
            hits = TRUE;
            break;
        default:
            break;
        }

    if (hits && whodidit != -1) {
        hit_bars(obj_p, x,y, barsx,barsy, whodidit, FALSE);
    }

    return hits;
}

void
reset_mthrowu(VOID_ARGS)
{
    mesg_given = 0;
    notonhead = FALSE;
}

/*mthrowu.c*/
