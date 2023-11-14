/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    zap.c    $NHDT-Date: 1551395521 2019/02/28 23:12:01 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.307 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2013. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <math.h>

/* Disintegration rays have special treatment; corpses are never left.
 * But the routine which calculates the damage is separate from the routine
 * which kills the monster.  The damage routine returns this cookie to
 * indicate that the monster should be disintegrated.
 */
#define DISINTEGRATION_DUMMY_DAMAGE 5000

STATIC_VAR NEARDATA boolean obj_zapped;
STATIC_VAR NEARDATA int poly_zapped;

extern boolean notonhead; /* for long worms */

/* kludge to use mondied instead of killed */
extern boolean m_using;

STATIC_DCL void FDECL(polyuse, (struct obj *, int, int));
STATIC_DCL void FDECL(create_polymon, (struct obj *, int));
STATIC_DCL boolean FDECL(zap_updown, (struct obj *));
STATIC_DCL void FDECL(zhitu, (int, struct obj*, struct monst*, int, int, int, const char *));
STATIC_DCL void FDECL(revive_egg, (struct obj *));
STATIC_DCL boolean FDECL(zap_steed, (struct obj *));
STATIC_DCL void FDECL(skiprange, (int, int *, int *));
STATIC_DCL int FDECL(zap_hit, (int, int, struct obj*, struct monst*));
STATIC_DCL void FDECL(backfire, (struct obj *));
STATIC_DCL int FDECL(m_spell_hit_skill_bonus, (struct monst*, int));
STATIC_DCL int FDECL(m_spell_hit_dex_bonus, (struct monst*, int));
STATIC_DCL int FDECL(m_wand_hit_skill_bonus, (struct monst*, int));
STATIC_DCL void FDECL(wishcmdassist, (int));
STATIC_DCL int FDECL(get_summon_monster_type, (int));

#define ZT_MAGIC_MISSILE (AD_MAGM - 1)
#define ZT_FIRE (AD_FIRE - 1)
#define ZT_COLD (AD_COLD - 1)
#define ZT_SLEEP (AD_SLEE - 1)
#define ZT_DISINTEGRATION (AD_DISN - 1) /* or disintegration */
#define ZT_LIGHTNING (AD_ELEC - 1)
#define ZT_POISON_GAS (AD_DRST - 1)
#define ZT_ACID (AD_ACID - 1)
#define ZT_DEATH (AD_DRAY - 1)
#define ZT_PETRIFICATION (AD_STON - 1)
/* 9 is currently unassigned */

#define ZT_WAND(x) (x)
#define ZT_SPELL(x) (10 + (x))
#define ZT_BREATH(x) (20 + (x))
#define ZT_EYESTALK(x) (30 + (x))

#define is_hero_spell(type) ((type) >= 10 && (type) < 20)

#define M_IN_WATER(ptr) \
    ((ptr)->mlet == S_EEL || amphibious(ptr) || is_swimmer(ptr))

STATIC_VAR const char are_blinded_by_the_flash[] =
    "are blinded by the flash!";

const char *const flash_types[] =       /* also used in buzzmu(mcastu.c) */
    {
        "magic missile", /* Wands must be 0-9 */
        "bolt of fire", "bolt of cold", "sleep ray", "disintegration ray",
        "bolt of lightning", "", "", "death ray", "petrification ray",

        "magic missile", /* Spell equivalents must be 10-19 */
        "fire bolt", "cone of cold", "sleep", "disintegrate",
        "lightning bolt", /* there is no spell, used for retribution */
        "", "", "finger of death", "stone to flesh",

        "blast of missiles", /* Dragon breath equivalents 20-29*/
        "blast of fire", "blast of frost", "blast of sleep gas",
        "blast of disintegration", "blast of lightning",
        "blast of poison gas", "blast of acid", "blast of death magic", "blast of petrififying vapors",

        "magic missile", /* Eyestalk equivalents must be 30-39 */
        "bolt of fire", "bolt of cold", "sleep ray", "disintegration ray",
        "bolt of lightning", "", "", "death ray", "petrification ray"

};

int
get_maximum_applicable_spell_damage_level(otyp, origmonst)
int otyp;
struct monst* origmonst;
{
    if (otyp <= STRANGE_OBJECT || otyp >= NUM_OBJECTS)
        return 0;

    int skill_type = objects[otyp].oc_skill;
    int skill_level = (int)(origmonst == &youmonst ? P_SKILL_LEVEL(skill_type) : is_prince(origmonst->data) ? P_GRAND_MASTER : is_lord(origmonst->data) ? P_EXPERT : P_BASIC);
    int max_level =  10 * max(0, skill_level - 1);
    return max_level;
}

int
get_spell_skill_level(otyp, origmonst, targetmonst)
int otyp;
struct monst* origmonst;
struct monst* targetmonst;
{
    if (otyp <= STRANGE_OBJECT || otyp >= NUM_OBJECTS)
        return P_ISRESTRICTED;

    int skill_level = P_UNSKILLED;

    boolean same_side = (origmonst && targetmonst && ((origmonst == targetmonst) || ((origmonst == &youmonst) && is_tame(targetmonst)) || ((origmonst != &youmonst && !is_peaceful(origmonst) && !is_peaceful(targetmonst)))));
    if (origmonst)
    {
        int skill_type = P_NONE;

        if (objects[otyp].oc_class == WAND_CLASS || (objects[otyp].oc_class == TOOL_CLASS && objects[otyp].oc_skill == P_WAND))
        {
            skill_type = P_WAND;
        }
        else if (objects[otyp].oc_class == SPBOOK_CLASS)
        {
            skill_type = objects[otyp].oc_skill;
        }

        if (same_side)
        {
            /* Use unskilled */
        }
        else if (origmonst == &youmonst)
            skill_level = P_SKILL_LEVEL(skill_type);
        else
            skill_level = is_prince(origmonst->data) ? P_SKILLED : is_lord(origmonst->data) || is_wizard(origmonst->data) ? P_BASIC : P_UNSKILLED;

    }

    return skill_level;
}

int 
get_spell_damage(otyp, origmonst, targetmonst)
int otyp;
struct monst* origmonst;
struct monst* targetmonst;
{
    if (otyp <= STRANGE_OBJECT || otyp >= NUM_OBJECTS)
        return 0;

    /* Skill bonus from using wand -- affects only non-tame monsters for the player */
    double dicemult = 1.0;
    boolean same_side = (origmonst && targetmonst && ((origmonst == targetmonst) || ((origmonst == &youmonst) && is_tame(targetmonst)) || ((origmonst != &youmonst && !is_peaceful(origmonst) && !is_peaceful(targetmonst)))));
    if (origmonst && (objects[otyp].oc_class == WAND_CLASS || (objects[otyp].oc_class == TOOL_CLASS && objects[otyp].oc_skill == P_WAND)))
    {
        int skill_level = P_UNSKILLED;
        if (same_side)
        {
            /* Use unskilled */
        }
        else if (origmonst == &youmonst)
            skill_level = P_SKILL_LEVEL(P_WAND);
        else
            skill_level = is_prince(origmonst->data) ? P_SKILLED : is_lord(origmonst->data) || is_wizard(origmonst->data) ? P_BASIC : P_UNSKILLED; /* No increase in wand damage for monsters to avoid unnecessary instadeaths */

        dicemult = get_wand_damage_multiplier(skill_level);
    }

    int dmg = d(max(1, (int)((double)objects[otyp].oc_spell_dmg_dice * dicemult)), objects[otyp].oc_spell_dmg_diesize) + objects[otyp].oc_spell_dmg_plus;
    if (has_spell_otyp_per_level_bonus(otyp) && origmonst)
    {
        int max_level = get_maximum_applicable_spell_damage_level(otyp, origmonst);

        int applied_level = min(max_level, origmonst == &youmonst ? u.ulevel : origmonst->m_lev);
        int applied_bonuses = applied_level / (int)objects[otyp].oc_spell_per_level_step;
        for(int i = 0; i < applied_bonuses; i++)
            dmg += (d(max(1, (int)((double)objects[otyp].oc_spell_per_level_dice * dicemult)), objects[otyp].oc_spell_per_level_diesize) + objects[otyp].oc_spell_per_level_plus);
    }


    if (dmg < 0)
        dmg = 0;

    return dmg;
}

int
get_obj_spell_duration(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    int otyp = obj->otyp;
    int durdice = objects[otyp].oc_spell_dur_dice + (obj->speflags & SPEFLAGS_BEING_BROKEN ? obj->charges : 0);
    int duration = d(durdice, objects[otyp].oc_spell_dur_diesize) + objects[otyp].oc_spell_dur_plus + bcsign(obj) * objects[otyp].oc_spell_dur_buc_plus;
    return max(0, duration);
}

int
get_otyp_spell_duration(otyp)
int otyp;
{
    struct obj tempobj = { 0 };
    tempobj.otyp = otyp;
    return get_obj_spell_duration(&tempobj);
}

/*
 * Recognizing unseen wands by zapping:  in 3.4.3 and earlier, zapping
 * most wand types while blind would add that type to the discoveries
 * list even if it had never been seen (ie, picked up while blinded
 * and shown in inventory as simply "a wand").  This behavior has been
 * changed; now such wands won't be discovered.  But if the type is
 * already discovered, then the individual wand whose effect was just
 * observed will be flagged as if seen.  [You already know wands of
 * striking; you zap "a wand" and observe striking effect (presumably
 * by sound or touch); it'll become shown in inventory as "a wand of
 * striking".]
 *
 * Unfortunately, the new behavior isn't really correct either.  There
 * should be an `eknown' bit for "effect known" added for wands (and
 * for potions since quaffing one of a stack is similar) so that the
 * particular wand which has been zapped would have its type become
 * known (it would change from "a wand" to "a wand of striking", for
 * example) without the type becoming discovered or other unknown wands
 * of that type showing additional information.  When blindness ends,
 * all objects in inventory with the eknown bit set would be discovered
 * and other items of the same type would become known as such.
 */

/* wand discovery gets special handling when hero is blinded */
void
learnwand(obj)
struct obj *obj;
{
    /* For a wand (or wand-like tool) zapped by the player, if the
       effect was observable (determined by caller; usually seen, but
       possibly heard or felt if the hero is blinded) then discover the
       object type provided that the object itself is known (as more
       than just "a wand").  If object type is already discovered and
       we observed the effect, mark the individual wand as having been
       seen.  Suppress spells (which use fake spellbook object for `obj')
       so that casting a spell won't re-discover its forgotten book. */
    if (obj->oclass != SPBOOK_CLASS) {
        /* if type already discovered, treat this item has having been seen
           even if hero is currently blinded (skips redundant makeknown) */
        if (objects[obj->otyp].oc_name_known) {
            obj->dknown = 1; /* will usually be set already */

        /* otherwise discover it if item itself has been or can be seen */
        } else {
            /* in case it was picked up while blind and then zapped without
               examining inventory after regaining sight (bypassing xname) */
            if (!Blind)
                obj->dknown = 1;
            /* make the discovery iff we know what we're manipulating */
            if (obj->dknown)
                makeknown(obj->otyp);
        }
    }
}

int
get_saving_throw_adjustment(otmp, targetmonst, origmonst)
struct obj* otmp;
struct monst* targetmonst, *origmonst;
{
    if (!otmp)
        return 0;
    
    int otyp = otmp->otyp;

    int res = 0;
    res += objects[otyp].oc_spell_saving_throw_adjustment;

    /* Adjustment for blessed and cursed objects */
    if (otmp->blessed)
        res -= 5;
    else if (otmp->cursed)
        res += 5;

    /* Extra penalties from negate/etc. magic resistance */
    if (targetmonst)
    {
        if (targetmonst == &youmonst)
        {
            if (No_magic_resistance)
                res -= 20;
            else if (One_fourth_magic_resistance)
                res -= 12;
            else if (Half_magic_resistance)
                res -= 8;
            else if (Three_fourths_magic_resistance)
                res -= 4;
        }
        else
        {
            if (targetmonst->mprops[NO_MAGIC_RESISTANCE])
                res -= 20;
            else if (targetmonst->mprops[ONE_FOURTH_MAGIC_RESISTANCE])
                res -= 12;
            else if (targetmonst->mprops[HALVED_MAGIC_RESISTANCE])
                res -= 8;
            else if (targetmonst->mprops[THREE_FOURTHS_MAGIC_RESISTANCE])
                res -= 4;
        }
    }

    if (!origmonst)
        return res;

    int skill = P_NONE;
    if (otmp->oclass == SPBOOK_CLASS || otmp->oclass == WAND_CLASS || otmp->oclass == TOOL_CLASS)
    {
        skill = objects[otyp].oc_skill;

        int skill_level = P_UNSKILLED;
        if (origmonst == &youmonst)
        {
            skill_level = P_SKILL_LEVEL(skill);
        }
        else
        {
            if (is_prince(origmonst->data))
                skill_level = P_GRAND_MASTER;
            else if (is_lord(origmonst->data))
                skill_level = P_EXPERT;
            else
                skill_level = P_BASIC;
        }
        if(skill == P_WAND)
            res += get_wand_skill_level_saving_throw_adjustment(skill_level);
        else
            res += get_spell_skill_level_saving_throw_adjustment(skill_level);
    }

    return res;
}

int
get_spell_skill_level_saving_throw_adjustment(skill_level)
int skill_level;
{
    return -3 * (max(0, skill_level - 1) - 1);
}

int
get_wand_skill_level_saving_throw_adjustment(skill_level)
int skill_level;
{
    return -2 * (max(0, skill_level - 1) - 0);
}

/* Routines for IMMEDIATE wands and spells. */
/* bhitm: monster mtmp was hit by the effect of wand or spell otmp */
int
bhitm(mtmp, otmp, origmonst)
struct monst *mtmp;
struct obj *otmp;
struct monst* origmonst;
{
    int res = 0;
    boolean wake = TRUE; /* Most 'zaps' should wake monster */
    boolean reveal_invis = FALSE, learn_it = FALSE;
    boolean helpful_gesture = FALSE;
    int otyp = otmp->otyp;
    const char *zap_type_text = otmp && OBJ_CONTENT_NAME(otmp->otyp) ? OBJ_CONTENT_NAME(otmp->otyp) : otmp && otmp->oclass == SPBOOK_CLASS ? OBJ_NAME(objects[otmp->otyp]) : "spell";
    struct obj *obj;
    boolean disguised_mimic = (is_mimic(mtmp->data)
                               && M_AP_TYPE(mtmp) != M_AP_NOTHING);
    int duration = get_obj_spell_duration(otmp);
    int dmg = get_spell_damage(otyp, origmonst, mtmp);
    int save_adj = get_saving_throw_adjustment(otmp, mtmp, origmonst);
    boolean surpress_noeffect_message = FALSE;
    //boolean magic_resistance_success = check_magic_resistance_and_inflict_damage(mtmp, otmp, 0, 0, 0, NOTELL);
    boolean magic_cancellation_success = check_magic_cancellation_success(mtmp, save_adj);
    if (u.uswallow && mtmp == u.ustuck)
        reveal_invis = FALSE;

    notonhead = (mtmp->mx != bhitpos.x || mtmp->my != bhitpos.y);

    switch (otyp) {
    case WAN_STRIKING:
        zap_type_text = "wand";
    /*FALLTHRU*/
    case SPE_FORCE_BOLT:
    case SPE_FORCE_STRIKE:
    case SPE_MAGIC_ARROW:
    case SPE_ARROW_OF_DIANA:
        res = 1;
        reveal_invis = TRUE;
        if (disguised_mimic)
            seemimic(mtmp);
        if (is_mon_immune_to_magic_missile(mtmp) || Invulnerable)
        { /* match effect on player */
            play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "Boing!");
            break; /* skip makeknown */
        }
        else // if (u.uswallow || rnd(20) < 10 + find_mac(mtmp))
        {
            /* resist deals the damage and displays the damage dealt */
            play_sfx_sound_at_location(SFX_MAGIC_ARROW_HIT, mtmp->mx, mtmp->my);
            hit_with_hit_tile(zap_type_text, mtmp, exclam(dmg), -1, "", HIT_GENERAL, FALSE);
            (void) inflict_spell_damage(mtmp, otmp, origmonst, dmg, AD_MAGM, TELL);
        } 
        //else
        //    miss(zap_type_text, mtmp);
        learn_it = TRUE;
        break;
    case SPE_SHOCKING_TOUCH:
    case SPE_ELECTROCUTE:
        res = 1;
        reveal_invis = TRUE;
        if (disguised_mimic)
            seemimic(mtmp);

        if (is_mon_immune_to_elec(mtmp))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by your electric touch!", Monnam(mtmp));
            break;
        }
        /* resist deals the damage and displays the damage dealt */
        play_sfx_sound(SFX_MONSTER_GETS_ZAPPED);
        Your_ex(ATR_NONE, CLR_MSG_SPELL, "touch jolts %s with electricity!", mon_nam(mtmp));
        display_m_being_hit(mtmp, HIT_ELECTROCUTED, dmg, 0UL, FALSE);
        (void)check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, TRUE, dmg, AD_ELEC, TELL);
        learn_it = TRUE;
        break;
    case SPE_HEAVENLY_TOUCH:
    case SPE_TOUCH_OF_DIVINITY:
        res = 1;
        reveal_invis = TRUE;
        if (disguised_mimic)
            seemimic(mtmp);

        if (!(is_demon(mtmp->data) || is_undead(mtmp->data) || is_vampshifter(mtmp)))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by your %s!", Monnam(mtmp), OBJ_NAME(objects[otyp]));
            break;
        }
        /* resist deals the damage and displays the damage dealt */
        play_sfx_sound(SFX_MONSTER_IS_HIT_WITH_CELESTIAL_MAGIC);
        Your_ex(ATR_NONE, CLR_MSG_SPELL, "%s sears %s!", OBJ_NAME(objects[otyp]), mon_nam(mtmp));
        display_m_being_hit(mtmp, HIT_GENERAL, dmg, 0UL, FALSE);
        (void)check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, TRUE, dmg, AD_CLRC, TELL);
        learn_it = TRUE;
        break;
    case SPE_BURNING_HANDS:
    case SPE_INCINERATE:
        res = 1;
        reveal_invis = TRUE;
        if (disguised_mimic)
            seemimic(mtmp);

        if (is_mon_immune_to_fire(mtmp))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by your fiery touch!", Monnam(mtmp));
            break;
        }
        /* resist deals the damage and displays the damage dealt */
        play_sfx_sound(SFX_MONSTER_ON_FIRE);
        Your_ex(ATR_NONE, CLR_MSG_SPELL, "fiery touch burns %s!", mon_nam(mtmp));
        display_m_being_hit(mtmp, HIT_ON_FIRE, dmg, 0UL, FALSE);
        (void)check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, TRUE, dmg, AD_FIRE, TELL);
        learn_it = TRUE;
        break;
    case SPE_FREEZING_TOUCH:
    case SPE_GLACIAL_GRASP:
        res = 1;
        reveal_invis = TRUE;
        if (disguised_mimic)
            seemimic(mtmp);

        if (is_mon_immune_to_cold(mtmp))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by your freezing touch!", Monnam(mtmp));
            break;
        }
        /* resist deals the damage and displays the damage dealt */
        play_sfx_sound(SFX_MONSTER_COVERED_IN_FROST);
        Your_ex(ATR_NONE, CLR_MSG_SPELL, "freezing touch sears %s!", mon_nam(mtmp));
        display_m_being_hit(mtmp, HIT_FROZEN, dmg, 0UL, FALSE);
        (void)check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, TRUE, dmg, AD_COLD, TELL);
        learn_it = TRUE;
        break;
    case SPE_RAY_OF_RADIANCE:
    case SPE_SUNLIGHT_BEAM:
        res = 1;
        reveal_invis = TRUE;
        if (disguised_mimic)
            seemimic(mtmp);

        if (is_undead(mtmp->data) || is_demon(mtmp->data) || is_vampshifter(mtmp) || hates_light(mtmp->data))
        {
            /* resist deals the damage and displays the damage dealt */
            play_sfx_sound(SFX_MONSTER_IS_HIT_WITH_CELESTIAL_MAGIC);
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "The %s sears %s!", OBJ_NAME(objects[otyp]),  mon_nam(mtmp));
            display_m_being_hit(mtmp, HIT_GENERAL, dmg, 0UL, FALSE);
            (void)inflict_spell_damage(mtmp, otmp, origmonst, dmg, AD_CLRC, TELL);
            learn_it = TRUE;
        }
        else
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by the %s!", Monnam(mtmp), OBJ_NAME(objects[otyp]));
        }
        break;
    case SPE_TOUCH_OF_DEATH:
        res = 1;
        reveal_invis = TRUE;
        You_ex(ATR_NONE, CLR_MSG_SPELL, "reach out with your deadly touch...");
        if (check_rider_death_absorption(mtmp, (const char*)0))
        {
            /* no further action */
            break;
        }
        else if (resists_death(mtmp))
        { /* match effect on player */
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by your touch!", Monnam(mtmp));
            break; /* skip makeknown */
        } 
        else if (magic_cancellation_success)
        {
            play_sfx_sound_at_location(SFX_UNLUCKILY_YOUR_TOUCH_DID_NOT_WORK, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Luckily for %s, it didn't work!", mon_nam(mtmp));
            break; /* skip makeknown */
        }
        else
        { //Otherwise dead
            play_sfx_sound_at_location(SFX_MONSTER_IS_HIT_WITH_DEATH_MAGIC, mtmp->mx, mtmp->my);
            display_m_being_hit(mtmp, HIT_DEATH, 1000, 0UL, FALSE);
            mtmp->mhp = 0;
            if (DEADMONSTER(mtmp)) {
                killed(mtmp);
            }
        }
        break;
    case SPE_OBLITERATE:
        res = 1;
        reveal_invis = TRUE;
        You_ex(ATR_NONE, CLR_MSG_SPELL, "reach out with your obliterating touch...");
        if (check_rider_disintegration(mtmp, (const char*)0))
        {
            /* no further action */
            break;
        }
        else if (resists_disint(mtmp) || is_peaceful(mtmp))
        { /* match effect on player */
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by your touch!", Monnam(mtmp));
            break; /* skip makeknown */
        }
        else if (magic_cancellation_success)
        {
            play_sfx_sound_at_location(SFX_UNLUCKILY_YOUR_TOUCH_DID_NOT_WORK, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Luckily for %s, it didn't work!", mon_nam(mtmp));
            break; /* skip makeknown */
        }
        else
        { //Otherwise dead
            play_sfx_sound(SFX_DISINTEGRATE);
            display_m_being_hit(mtmp, HIT_DISINTEGRATED, 1000, 0UL, FALSE);
            disintegrate_mon(mtmp, 1, "obliteration spell");
        }
        break;
    case SPE_FLESH_TO_STONE:
        res = 1;
        if (resists_ston(mtmp))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected.", Monnam(mtmp));
        }
        else if (check_ability_resistance_success(mtmp, A_DEX, save_adj))
        {
            play_sfx_sound_at_location(SFX_ENEMY_RESISTANCE_SUCCESS_DEX, mtmp->mx, mtmp->my);
            pline("%s dodges your spell.", Monnam(mtmp));
        }
        else
            start_delayed_petrification(mtmp, TRUE);
        break;
    case SPE_TOUCH_OF_PETRIFICATION:
        res = 1;
        You_ex(ATR_NONE, CLR_MSG_SPELL, "reach out with your petrifying touch...");
        if (resists_ston(mtmp))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected.", Monnam(mtmp));
        }
        else if (magic_cancellation_success)
        {
            play_sfx_sound_at_location(SFX_UNLUCKILY_YOUR_TOUCH_DID_NOT_WORK, mtmp->mx, mtmp->my);
            pline("Luckily for %s, it didn't work!", mon_nam(mtmp));
        }
        else
            start_delayed_petrification(mtmp, TRUE);
        break;

    case SPE_GAZE_OF_PETRIFICATION:
        res = 1;
        if (!m_canseeu(mtmp))
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s couldn't see your gaze.", Monnam(mtmp));
        else if (mon_reflects(mtmp, (char*)0))
        {
            if (canseemon(mtmp))
            {
                play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mtmp->mx, mtmp->my);
                (void)mon_reflects(mtmp, "Your gaze is reflected by %s %s.");
            }

            if (!Blind) 
            {
                if (Reflecting) 
                {
                    play_sfx_sound(SFX_GENERAL_REFLECTS);
                    (void)ureflects("Your reflected gaze is reflected away by your %s.",
                        s_suffix(Monnam(mtmp)));
                }
                if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my)
                    && !Stone_resistance)
                {
                    You_ex(ATR_NONE, CLR_MSG_WARNING, "meet your own reflected gaze.");
                    stop_occupation();
                    if (poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM))
                        break;
                    play_sfx_sound(SFX_PETRIFY);
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "turn to stone...");
                    killer.format = KILLED_BY;
                    Strcpy(killer.name, mon_monster_name(mtmp));
                    done(STONING);
                }
            }
        }
        else if (resists_ston(mtmp) || !mon_can_see(mtmp) || !haseyes(mtmp->data))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected.", Monnam(mtmp));
        }
#if 0
        else if (check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, FALSE, 0, 0, TELL))
        {
            /* nothing else */
        }
#endif
        else /* You cannot dodge the gaze of Medusa! */
            minstapetrify(mtmp, TRUE);

        break;
    case SPE_POWER_WORD_KILL:
        res = 1;
        reveal_invis = TRUE;
        if (check_rider_death_absorption(mtmp, (const char*)0))
        {
            /* no further action */
            break;
        }
        else if (resists_death(mtmp))
        { /* match effect on player */
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by the power word!", Monnam(mtmp));
            break; /* skip makeknown */
        }
        else if(!check_ability_resistance_success(mtmp, A_CON, save_adj)) // if (!check_magic_resistance_and_inflict_damage(mtmp, otmp, FALSE, 0, 0, TELL))
        { //Otherwise dead
            play_sfx_sound_at_location(SFX_MONSTER_IS_HIT_WITH_DEATH_MAGIC, mtmp->mx, mtmp->my);
            display_m_being_hit(mtmp, HIT_DEATH, 1000, 0UL, FALSE);
            mtmp->mhp = 0;
            if (DEADMONSTER(mtmp)) {
                killed(mtmp);
            }
        }
        break;
    case SPE_POWER_WORD_STUN:
        res = 1;
        reveal_invis = TRUE;
        if (mindless(mtmp->data)) { /* match effect on player */
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by the power word!", Monnam(mtmp));
            break; /* skip makeknown */
        }
        else if (is_stunned(mtmp)) { /* match effect on player */
            pline("%s does not seem more stunned than before.", Monnam(mtmp));
            break; /* skip makeknown */
        }
        else if (!check_ability_resistance_success(mtmp, A_CON, save_adj))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(0);
            play_sfx_sound_at_location(SFX_ACQUIRE_STUN, mtmp->mx, mtmp->my);
            increase_mon_property_verbosely(mtmp, STUNNED, 10 + rnd(10));
            special_effect_wait_until_end(0);
        }
        break;
    case SPE_POWER_WORD_BLIND:
        res = 1;
        reveal_invis = TRUE;
        if (!haseyes(mtmp->data) || mindless(mtmp->data))
        { /* match effect on player */
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected by the power word!", Monnam(mtmp));
            break; /* skip makeknown */
        }
        else if (is_blinded(mtmp) && ((duration > 0 && get_mon_property(mtmp, BLINDED) > duration) || (mtmp->mprops[BLINDED] & ~M_TIMEOUT)))
        { /* match effect on player */
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s does not seem more blind than before.", Monnam(mtmp));
            break; /* skip makeknown */
        }
        else if (!check_ability_resistance_success(mtmp, A_CON, save_adj))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(0);
            play_sfx_sound_at_location(SFX_ACQUIRE_BLINDNESS, mtmp->mx, mtmp->my);
            increase_mon_property_verbosely(mtmp, BLINDED, duration);
            special_effect_wait_until_end(0);
        }
        break;
    case WAN_SLOW_MONSTER:
    case SPE_SLOW_MONSTER:
    case SPE_MASS_SLOW:
        res = 1;
        if (origmonst)
        {
            int skill_level = get_spell_skill_level(otyp, origmonst, mtmp);
            if (skill_level > P_UNSKILLED)
                save_adj -= 2 * (skill_level - P_UNSKILLED);
        }
        if (!check_ability_resistance_success(mtmp, A_WIS, save_adj)) {
            if (disguised_mimic)
                seemimic(mtmp);

            play_sfx_sound_at_location(SFX_ACQUIRE_SLOW, mtmp->mx, mtmp->my);
            increase_mon_property_verbosely(mtmp, SLOWED, duration);
            m_dowear(mtmp, FALSE); /* might want speed boots */
            if (u.uswallow && (mtmp == u.ustuck) && is_whirly(mtmp->data)) {
                You_ex(ATR_NONE, CLR_MSG_SPELL, "disrupt %s!", mon_nam(mtmp));
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A huge hole opens up...");
                expels(mtmp, mtmp->data, TRUE);
            }
            else
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                special_effect_wait_until_action(0);
                special_effect_wait_until_end(0);
            }
        }
        break;
    case SPE_HASTE_MONSTER:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        play_sfx_sound_at_location(SFX_ACQUIRE_HASTE, mtmp->mx, mtmp->my);
        increase_mon_property_verbosely(mtmp, VERY_FAST, otmp->oclass == WAND_CLASS ? rn1(10, 100 + 60 * bcsign(otmp)) : duration);
        special_effect_wait_until_end(0);
        break;
    case SPE_HOLD_MONSTER:
    case SPE_MASS_HOLD:
        res = 1;
        if (origmonst)
        {
            int skill_level = get_spell_skill_level(otyp, origmonst, mtmp);
            if (skill_level > P_UNSKILLED)
                save_adj -= 2 * (skill_level - P_UNSKILLED);
        }
        if (!check_ability_resistance_success(mtmp, A_WIS, save_adj))
        {
            if (disguised_mimic)
                seemimic(mtmp);

            play_sfx_sound_at_location(SFX_ACQUIRE_PARALYSIS, mtmp->mx, mtmp->my);
            increase_mon_property_verbosely(mtmp, PARALYZED, duration);
            if (u.uswallow && (mtmp == u.ustuck) && is_whirly(mtmp->data)) 
            {
                You_ex(ATR_NONE, CLR_MSG_SPELL, "disrupt %s!", mon_nam(mtmp));
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A huge hole opens up...");
                expels(mtmp, mtmp->data, TRUE);
            }
            else
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                special_effect_wait_until_action(0);
                special_effect_wait_until_end(0);
            }
        }
        else
        {
            play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s resists!", Monnam(mtmp));
        }
        break;
    case SPE_HOLD_UNDEAD:
        res = 1;
        if (origmonst)
        {
            int skill_level = get_spell_skill_level(otyp, origmonst, mtmp);
            if (skill_level > P_UNSKILLED)
                save_adj -= 2 * (skill_level - P_UNSKILLED);
        }
        if (!(is_undead(mtmp->data) || is_vampshifter(mtmp)))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected.", Monnam(mtmp));
        }
        else if (!check_ability_resistance_success(mtmp, A_WIS, save_adj))
        {
            if (disguised_mimic)
                seemimic(mtmp);

            play_sfx_sound_at_location(SFX_ACQUIRE_PARALYSIS, mtmp->mx, mtmp->my);
            increase_mon_property_verbosely(mtmp, UNDEAD_IMMOBILITY, duration);
            if (u.uswallow && (mtmp == u.ustuck) && (is_undead(mtmp->data) || is_vampshifter(mtmp)) && is_whirly(mtmp->data))
            {
                You_ex(ATR_NONE, CLR_MSG_SPELL, "disrupt %s!", mon_nam(mtmp));
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A huge hole opens up...");
                expels(mtmp, mtmp->data, TRUE);
            }
            else
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                special_effect_wait_until_action(0);
                special_effect_wait_until_end(0);
            }
        }
        else
        {
            play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s resists!", Monnam(mtmp));
        }
        break;
    case WAN_SPEED_MONSTER:
    {
        res = 1;
        if (disguised_mimic)
            seemimic(mtmp);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        play_sfx_sound_at_location(SFX_ACQUIRE_HASTE, mtmp->mx, mtmp->my);
        special_effect_wait_until_action(0);
        boolean visible_effect = increase_mon_property_verbosely(mtmp, VERY_FAST, duration);
        if (visible_effect)
            makeknown(WAN_SPEED_MONSTER);
        m_dowear(mtmp, FALSE); /* might want speed boots */
        if (is_tame(mtmp))
            helpful_gesture = TRUE;
        special_effect_wait_until_end(0);
        break;
    }
    case SPE_SILENCE:
        res = 1;
        if (!check_ability_resistance_success(mtmp, A_CHA, save_adj))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(0);
            increase_mon_property_verbosely(mtmp, SILENCED, duration);
            special_effect_wait_until_end(0);
        }
        break;
    case WAN_UNDEAD_TURNING:
    case SPE_TURN_UNDEAD:
        res = 1;
        wake = FALSE;
        if (is_undead(mtmp->data) || is_vampshifter(mtmp)) {
            reveal_invis = TRUE;
            wake = TRUE;
            context.bypasses = TRUE; /* for make_corpse() */
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(0);
            (void)inflict_spell_damage(mtmp, otmp, origmonst, dmg, AD_CLRC, TELL);
            if (!DEADMONSTER(mtmp))
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_FEAR, mtmp->mx, mtmp->my);
                monflee(mtmp, duration, FALSE, TRUE);
            }
            special_effect_wait_until_end(0);
        }
        break;
    case SPE_FEAR:
        res = 1;
        if (origmonst)
        {
            int skill_level = get_spell_skill_level(otyp, origmonst, mtmp);
            if (skill_level > P_UNSKILLED)
                save_adj -= 2 * (skill_level - P_UNSKILLED);
        }
        if (!DEADMONSTER(mtmp) && !resists_fear(mtmp) && !check_ability_resistance_success(mtmp, A_WIS, save_adj))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(0);
            make_mon_fearful(mtmp, duration);
            special_effect_wait_until_end(0);
        }
        break;
    case WAN_RESURRECTION:
    case SPE_RESURRECTION:
        wake = FALSE;
        if (revive_from_inventory(mtmp))
        {
            res = 1;
            wake = TRUE;
        }
        break;
    case SPE_NEGATE_UNDEATH:
        wake = FALSE;
        if (is_undead(mtmp->data) || is_vampshifter(mtmp)) {
            res = 1;
            reveal_invis = TRUE;
            wake = TRUE;
            dmg = max(mtmp->mhp, dmg);
            context.bypasses = TRUE; /* for make_corpse() */
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(0);
            if (!check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, TRUE, dmg, AD_CLRC, TELL_LETHAL_STYLE))
            {
                if (!DEADMONSTER(mtmp))
                {
                    play_sfx_sound_at_location(SFX_ACQUIRE_FEAR, mtmp->mx, mtmp->my);
                    monflee(mtmp, duration, FALSE, TRUE);
                }
            }
            special_effect_wait_until_end(0);
        }
        break;
    case SPE_BANISH_DEMON:
        wake = FALSE;
        if (is_demon(mtmp->data))
        {
            res = 1;
            reveal_invis = TRUE;
            wake = TRUE;
            dmg = max(mtmp->mhp, dmg);
            context.bypasses = TRUE; /* for make_corpse() */
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(0);
            if (!check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, TRUE, dmg, AD_CLRC, TELL_LETHAL_STYLE))
            {
                if (!DEADMONSTER(mtmp))
                {
                    play_sfx_sound_at_location(SFX_ACQUIRE_FEAR, mtmp->mx, mtmp->my);
                    monflee(mtmp, duration, FALSE, TRUE);
                }
            }
            special_effect_wait_until_end(0);
        }
        break;
    case SPE_CHARM_MONSTER:
    case SPE_DOMINATE_MONSTER:
        res = 1;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        helpful_gesture = (maybe_tame(mtmp, otmp, &youmonst) == 1 || is_tame(mtmp));
        special_effect_wait_until_end(0);
        break;
    case SPE_CONTROL_UNDEAD:
        res = 1;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        helpful_gesture = (maybe_controlled(mtmp, otmp, &youmonst) == 1 || is_tame(mtmp));
        special_effect_wait_until_end(0);
        break;
    case WAN_POLYMORPH:
    case SPE_POLYMORPH:
    case POT_POLYMORPH:
        res = 1;
        if (is_long_worm_with_tail(mtmp->data) && has_mcorpsenm(mtmp)) 
        {
            /* if a long worm has mcorpsenm set, it was polymophed by
               the current zap and shouldn't be affected if hit again */
            ;
        } 
        else if (resists_magic(mtmp))
        {
            /* magic missile resistance protects from polymorph traps, so make
               it guard against involuntary polymorph attacks too... */
            play_sfx_sound_at_location(SFX_POLYMORPH_FAIL, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
        }
        else if (!check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, FALSE, 0, 0, NOTELL))
        {
            boolean polyspot = (otyp != POT_POLYMORPH),
                    give_msg = (!Hallucination
                                && (canseemon(mtmp)
                                    || (u.uswallow && mtmp == u.ustuck)));

            /* dropped inventory (due to death by system shock,
               or loss of wielded weapon and/or worn armor due to
               limitations of new shape) won't be hit by this zap */
            if (polyspot)
                for (obj = mtmp->minvent; obj; obj = obj->nobj)
                    bypass_obj(obj);

            /* natural shapechangers aren't affected by system shock
               (unless protection from shapechangers is interfering
               with their metabolism...) */
            if (mtmp->cham == NON_PM && !rn2(25))
            {
                if (canseemon(mtmp)) 
                {
                    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_SHUDDER);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s shudders!", Monnam(mtmp));
                    learn_it = TRUE;
                }
                /* context.bypasses = TRUE; ## for make_corpse() */
                /* no corpse after system shock */
                xkilled(mtmp, XKILL_GIVEMSG | XKILL_NOCORPSE);
            } 
            else if (newcham(mtmp, (struct permonst *) 0, 0, polyspot, give_msg) != 0
                       /* if shapechange failed because there aren't
                          enough eligible candidates (most likely for
                          vampshifter), try reverting to original form */
                       || (mtmp->cham >= LOW_PM && newcham(mtmp, &mons[mtmp->cham], mtmp->cham_subtype, polyspot, give_msg) != 0)) 
            {
                if (give_msg && (canspotmon(mtmp)
                    || (u.uswallow && mtmp == u.ustuck)))
                {
                    learn_it = TRUE;
                    if (!(u.uswallow && mtmp == u.ustuck))
                    {
                        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                        special_effect_wait_until_action(0);
                        special_effect_wait_until_end(0);
                    }
                }
            }

            /* do this even if polymorphed failed (otherwise using
               flags.mon_polycontrol prompting to force mtmp to remain
               'long worm' would prompt again if zap hit another segment) */
            if (!DEADMONSTER(mtmp) && is_long_worm_with_tail(mtmp->data)) 
            {
                int mnum = mtmp->mnum;
                if (!has_mcorpsenm(mtmp))
                    newmcorpsenm(mtmp);
                /* flag to indicate that mtmp became a long worm
                   on current zap, so further hits (on mtmp's new
                   tail) don't do further transforms */
                MCORPSENM(mtmp) = mnum; // PM_LONG_WORM;
                /* flag to indicate that cleanup is needed; object
                   bypass cleanup also clears mon->mextra->mcorpsenm
                   for all long worms on the level */
                context.bypasses = TRUE;
            }
        }
        else
        {
            play_sfx_sound_at_location(SFX_POLYMORPH_FAIL, mtmp->mx, mtmp->my);
        }
        break;
    case WAN_CANCELLATION:
    case SPE_CANCELLATION:
        res = 1;
        if (disguised_mimic)
            seemimic(mtmp);
        if (mtmp->cham && !mtmp->mprops[UNCHANGING])
            revert_mon_polymorph(mtmp, FALSE, TRUE, TRUE);
        if (!has_cancellation_resistance(mtmp))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(0);
            (void)cancel_monst(mtmp, otmp, TRUE, TRUE, FALSE, duration);
            (void)nonadditive_increase_mon_property_verbosely(mtmp, CANCELLATION_RESISTANCE, 10);
            special_effect_wait_until_end(0);
        }
        break;
    case WAN_DISJUNCTION:
    case SPE_DISJUNCTION:
        res = 1;
        if (disguised_mimic)
            seemimic(mtmp);
        if (mtmp->cham && !mtmp->mprops[UNCHANGING])
            revert_mon_polymorph(mtmp, FALSE, TRUE, TRUE);
        /* Unaffected by cancellation resistance */
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        (void)cancel_monst(mtmp, otmp, TRUE, TRUE, FALSE, duration);
        special_effect_wait_until_end(0);
        break;
    case SPE_LOWER_MAGIC_RESISTANCE:
    case SPE_DIMINISH_MAGIC_RESISTANCE:
    case SPE_ABOLISH_MAGIC_RESISTANCE:
    case SPE_NEGATE_MAGIC_RESISTANCE:
    case SPE_FORBID_SUMMONING:
        res = 1;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        (void)add_temporary_property(mtmp, otmp, TRUE, TRUE, FALSE, d(objects[otmp->otyp].oc_spell_dur_dice, objects[otmp->otyp].oc_spell_dur_diesize) + objects[otmp->otyp].oc_spell_dur_plus);
        special_effect_wait_until_end(0);
        break;
    case WAN_TELEPORTATION:
    case SPE_TELEPORT_MONSTER:
        res = 1;
        if (disguised_mimic)
            seemimic(mtmp);
        reveal_invis = !u_teleport_mon(mtmp, TRUE);
        break;
    case WAN_MAKE_INVISIBLE: {
        res = 1;
        int oldinvis = is_invisible(mtmp);

        if (disguised_mimic)
            seemimic(mtmp);
        
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        increase_mon_property_verbosely(mtmp, INVISIBILITY, duration);
        special_effect_wait_until_end(0);

        if (!oldinvis && knowninvisible(mtmp)) {
            reveal_invis = TRUE;
            learn_it = TRUE;
        }
        break;
    }
    case WAN_LOCKING:
    case SPE_WIZARD_LOCK:
        wake = closeholdingtrap(mtmp, &learn_it);
        if(wake)
            res = 1;
        break;
    case SPE_PROBE:
    case WAN_PROBING:
        res = 1;
        wake = FALSE;
        reveal_invis = TRUE;
        probe_monster(mtmp);
        learn_it = TRUE;
        break;
    case WAN_OPENING:
    case SPE_KNOCK:
        wake = FALSE; /* don't want immediate counterattack */
        if (u.uswallow && mtmp == u.ustuck) {
            res = 1;
            if (is_animal(mtmp->data)) {
                if (Blind)
                    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "a sudden rush of air!");
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s opens its mouth!", Monnam(mtmp));
            }
            expels(mtmp, mtmp->data, TRUE);
            /* zap which hits steed will only release saddle if it
               doesn't hit a holding or falling trap; playability
               here overrides the more logical target ordering */
        } else if (openholdingtrap(mtmp, &learn_it)) {
            res = 1;
            break;
        } else if (openfallingtrap(mtmp, TRUE, &learn_it)) {
            /* mtmp might now be on the migrating monsters list */
            res = 1;
            break;
        } else if ((obj = which_armor(mtmp, W_SADDLE)) != 0) {
            res = 1;
            char buf[BUFSZ];

            Sprintf(buf, "%s %s", s_suffix(Monnam(mtmp)),
                    distant_name(obj, xname));
            if (cansee(mtmp->mx, mtmp->my)) {
                if (!canspotmon(mtmp))
                    Strcpy(buf, An(distant_name(obj, xname)));
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s falls to the %s.", buf,
                      surface(mtmp->mx, mtmp->my));
            } else if (canspotmon(mtmp)) {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s falls off.", buf);
            }
            obj_extract_self(obj);
            mdrop_obj(mtmp, obj, FALSE, TRUE);
        }
        break;
    case JAR_OF_MEDICINAL_SALVE:
    case SPE_CURE_SICKNESS:
    {
        res = 1;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        play_sfx_sound_at_location(SFX_CURE_AILMENT, mtmp->mx, mtmp->my);
        special_effect_wait_until_action(0);

    cure_sickness_here:
        {
            boolean was_sick = is_sick(mtmp);
            boolean had_sick = !!mtmp->mprops[SICK];
            mtmp->mprops[SICK] &= ~(M_INTRINSIC_ACQUIRED | M_TIMEOUT);

            boolean was_food_poisoned = is_food_poisoned(mtmp);
            boolean had_food_poisoned = !!mtmp->mprops[FOOD_POISONED];
            mtmp->mprops[FOOD_POISONED] &= ~(M_INTRINSIC_ACQUIRED | M_TIMEOUT);

            boolean was_turning_into_slime = is_turning_into_slime(mtmp);
            boolean had_slimed = !!mtmp->mprops[SLIMED];
            mtmp->mprops[SLIMED] &= ~(M_INTRINSIC_ACQUIRED | M_TIMEOUT);

            boolean was_mummy_rotted = is_mummy_rotted(mtmp);
            boolean had_mummy_rot = !!mtmp->mprops[MUMMY_ROT];
            mtmp->mprops[MUMMY_ROT] &= ~(M_INTRINSIC_ACQUIRED | M_TIMEOUT);

            if (!is_sick(mtmp) && !is_food_poisoned(mtmp) && !is_turning_into_slime(mtmp) && !is_mummy_rotted(mtmp) && (was_sick || was_food_poisoned || was_turning_into_slime || was_mummy_rotted))
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s looks much better!", Monnam(mtmp));
            else if (!has_sick(mtmp) && had_sick)
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s is cured of its terminal illness!", Monnam(mtmp));
            else if (!has_food_poisoned(mtmp) && had_food_poisoned)
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s is cured of its food poisoning!", Monnam(mtmp));
            else if (!has_mummy_rot(mtmp) && had_mummy_rot)
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s is cured of its mummy rot!", Monnam(mtmp));
            else if (!has_slimed(mtmp) && had_slimed)
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s is cured of its sliming!", Monnam(mtmp));
            else if (!surpress_noeffect_message)
                pline("Nothing much seems to happen to %s.", mon_nam(mtmp));
        }
        special_effect_wait_until_end(0);
        newsym(mtmp->mx, mtmp->my);
        flush_screen(1);
        break;
    }
    case SPE_CURE_BLINDNESS:
    {
        res = 1;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        play_sfx_sound_at_location(SFX_CURE_AILMENT, mtmp->mx, mtmp->my);
        special_effect_wait_until_action(0);
        boolean was_blinded = is_blinded(mtmp);
        boolean had_blinded = !!mtmp->mprops[BLINDED];
        mtmp->mprops[BLINDED] &= ~(M_INTRINSIC_ACQUIRED | M_TIMEOUT);
        if (!is_blinded(mtmp) && was_blinded)
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s can see again!", Monnam(mtmp));
        else if (!mtmp->mprops[BLINDED] && had_blinded)
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s is cured of its underlying blindness!", Monnam(mtmp));
        else
            pline("Nothing much seems to happen to %s.", mon_nam(mtmp));

        special_effect_wait_until_end(0);
        newsym(mtmp->mx, mtmp->my);
        flush_screen(1);
        break;
    }
    case SPE_CURE_PETRIFICATION:
cure_petrification_here:
    {
        res = 1;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        play_sfx_sound_at_location(SFX_CURE_AILMENT, mtmp->mx, mtmp->my);
        special_effect_wait_until_action(0);
        boolean was_stoning = is_stoning(mtmp);
        boolean had_stoned = !!mtmp->mprops[STONED];
        mtmp->mprops[STONED] &= ~(M_INTRINSIC_ACQUIRED | M_TIMEOUT);
        if (!is_stoning(mtmp) && was_stoning)
        {
            learn_it = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s stops solidifying!", Monnam(mtmp));
        }
        else if (!mtmp->mprops[STONED] && had_stoned)
        {
            learn_it = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s is cured of its petrification!", Monnam(mtmp));
        }
        else
            pline("Nothing much seems to happen to %s.", mon_nam(mtmp));
        special_effect_wait_until_end(0);
        newsym(mtmp->mx, mtmp->my);
        flush_screen(1);
        break;
    }
    case JAR_OF_EXTRA_HEALING_SALVE:
    case JAR_OF_GREATER_HEALING_SALVE:
    case JAR_OF_PRODIGIOUS_HEALING_SALVE:
    case GRAIL_OF_HEALING:
    case SPE_MINOR_HEALING:
    case SPE_HEALING:
    case SPE_EXTRA_HEALING:
    case SPE_GREATER_HEALING:
    case SPE_PRODIGIOUS_HEALING:
    case SPE_FULL_HEALING:
        res = 1;
        reveal_invis = TRUE;
        if (otyp == GRAIL_OF_HEALING && (is_undead(mtmp->data) || is_demon(mtmp->data) || is_vampshifter(mtmp)))
        {
            dmg = d(48, 6);
            context.bypasses = TRUE; /* for make_corpse() */
            int hp_before = mtmp->mhp;
            deduct_monster_hp(mtmp, adjust_damage(dmg, &youmonst, mtmp, AD_CLRC, ADFLAGS_SPELL_DAMAGE));
            int hp_after = mtmp->mhp;

            int damagedealt = hp_before - hp_after;
            if (damagedealt > 0)
            {
                pline_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_orange2, "%s sustains %d damage!", Monnam(mtmp), damagedealt);
                display_m_being_hit(mtmp, HIT_GENERAL, damagedealt, 0UL, FALSE);
            }
            if (DEADMONSTER(mtmp))
            {
                if (m_using)
                    monkilled(mtmp, "", AD_CLRC, 0);
                else
                    killed(mtmp);
            }
        }
        else if (mtmp->data != &mons[PM_PESTILENCE] && is_living(mtmp->data))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            wake = FALSE; /* wakeup() makes the target angry */

            play_sfx_sound_at_location(SFX_HEALING, mtmp->mx, mtmp->my);
            special_effect_wait_until_action(0);
            deduct_monster_hp(mtmp, -(d(objects[otyp].oc_wsdice, objects[otyp].oc_wsdam) + objects[otyp].oc_wsdmgplus));

            if (canseemon(mtmp)) {
                if (disguised_mimic) {
                    if (is_obj_mappear(mtmp,STRANGE_OBJECT)) {
                        /* it can do better now */
                        set_mimic_sym(mtmp);
                        newsym(mtmp->mx, mtmp->my);
                    } else
                        mimic_hit_msg(mtmp, otyp);
                } else
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s looks %s.", Monnam(mtmp),
                          otyp == SPE_PRODIGIOUS_HEALING || otyp == JAR_OF_PRODIGIOUS_HEALING_SALVE || otyp == GRAIL_OF_HEALING ? "vastly better" :
                        otyp == SPE_GREATER_HEALING || otyp == JAR_OF_GREATER_HEALING_SALVE ? "much, much better" :
                        otyp == SPE_EXTRA_HEALING || otyp == JAR_OF_EXTRA_HEALING_SALVE ? "much better" : 
                        otyp == SPE_FULL_HEALING ? "completely healed" : 
                        "better");
            }

            if (otyp == GRAIL_OF_HEALING)
            {
                if (has_stoned(mtmp))
                {
                    (void)set_mon_property_b(mtmp, STONED, 0, canseemon(mtmp));
                    if (canseemon(mtmp))
                        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s looks limber!", Monnam(mtmp));
                }

                if (has_slimed(mtmp))
                    (void)set_mon_property_b(mtmp, SLIMED, 0, canseemon(mtmp));

                if (is_tame(mtmp))
                {
                    if (EDOG(mtmp)->hungrytime < monstermoves + 500)
                        EDOG(mtmp)->hungrytime = monstermoves + 500;
                }
            }

            if (is_tame(mtmp) || is_peaceful(mtmp)) {
                adjalign(Role_if(PM_HEALER) ? 1 : sgn(u.ualign.type));
            }

            if (otyp == GRAIL_OF_HEALING)
            {
                surpress_noeffect_message = TRUE;
                goto cure_sickness_here;
            }

            special_effect_wait_until_end(0);
        }
        else if (mtmp->data == &mons[PM_PESTILENCE])
        { /* Pestilence */
            /* Pestilence will always resist; damage is half of 3d{4,8,12} */
            (void) check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, TRUE,
                          d(3, otyp == SPE_FULL_HEALING ? 12 : otyp == SPE_EXTRA_HEALING ? 8 : 4), AD_CLRC, TELL);
        }
        else
        { /* Undead and other nonliving such as golems */
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected.", Monnam(mtmp));
        }
        break;
    case SPE_REPLENISH_UNDEATH:
    case SPE_GREATER_UNDEATH_REPLENISHMENT:
        res = 1;
        reveal_invis = TRUE;
        if (is_undead(mtmp->data))
        {
            wake = FALSE; /* wakeup() makes the target angry */

            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
            play_sfx_sound_at_location(SFX_HEALING, mtmp->mx, mtmp->my);
            special_effect_wait_until_action(0);
            mtmp->mhp += dmg;

            if (mtmp->mhp > mtmp->mhpmax)
                mtmp->mhp = mtmp->mhpmax;

            if (canseemon(mtmp))
            {
                if (disguised_mimic) 
                {
                    if (is_obj_mappear(mtmp, STRANGE_OBJECT))
                    {
                        /* it can do better now */
                        set_mimic_sym(mtmp);
                        newsym(mtmp->mx, mtmp->my);
                    }
                    else
                        mimic_hit_msg(mtmp, otyp);
                }
                else
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s looks %s.", Monnam(mtmp),
                        otyp == SPE_GREATER_UNDEATH_REPLENISHMENT ? "much better" : "better");
            }
            special_effect_wait_until_end(0);
        }
        else
        { /* Non-undead */
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected.", Monnam(mtmp));
        }
        break;
    case WAN_LIGHT: /* (broken wand) */
        res = 1;
        if (flash_hits_mon(mtmp, otmp)) {
            learn_it = TRUE;
            reveal_invis = TRUE;
        }
        break;
    case WAN_SLEEP: /* (broken wand) */
        res = 1;
        /* [wakeup() doesn't rouse victims of temporary sleep,
           so it's okay to leave `wake' set to TRUE here] */
        reveal_invis = TRUE;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        if (sleep_monst(mtmp, otmp, origmonst, duration, 0, TRUE))
            slept_monst(mtmp);
        special_effect_wait_until_end(0);
        if (!Blind)
            learn_it = TRUE;
        break;
    case JAR_OF_BASILISK_BLOOD:
    case SPE_STONE_TO_FLESH:
        res = 1;
        if (mtmp->mnum == PM_STONE_GOLEM) {
            char *name = Monnam(mtmp);

            /* turn into flesh golem */
            if (newcham(mtmp, &mons[PM_FLESH_GOLEM], 0, FALSE, FALSE)) {
                if (canseemon(mtmp))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s turns to flesh!", name);
            } else {
                if (canseemon(mtmp))
                    pline("%s looks rather fleshy for a moment.", name);
            }
        } else
            wake = FALSE;
        if (otyp == JAR_OF_BASILISK_BLOOD)
            goto cure_petrification_here;
        break;
    case SPE_DRAIN_LEVEL:
    {
        res = 1;
        if (disguised_mimic)
            seemimic(mtmp);

        int basedmg = monbasehp_per_lvl(mtmp);
        dmg = basedmg + (int)monhpadj_per_lvl(mtmp);

        if (resists_drli(mtmp))
        {
            m_shieldeff(mtmp);
        }
        else if (!check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, 0, dmg, AD_DRLI, NOTELL) && !DEADMONSTER(mtmp))
        {
            double damage = adjust_damage(dmg, origmonst, mtmp, AD_DRLI, ADFLAGS_SPELL_DAMAGE);
            double bdamage = adjust_damage(basedmg, origmonst, mtmp, AD_DRLI, ADFLAGS_SPELL_DAMAGE);

            deduct_monster_hp(mtmp, damage);
            mtmp->mbasehpdrain -= (int)floor(bdamage);
            mtmp->mhpmax -= (int)floor(damage);

            /* die if already level 0, regardless of hit points */
            if (DEADMONSTER(mtmp) || mtmp->mhpmax <= 0 || mtmp->m_lev < 1)
            {
                killed(mtmp);
            }
            else
            {
                mtmp->m_lev--;
                if (canseemon(mtmp))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly seems weaker!", Monnam(mtmp));
            }
            update_mon_maxhp(mtmp);
        }
        break;
    }
    case WAN_NOTHING:
        wake = FALSE;
        break;
    default:
        //impossible("What an interesting effect (%d)", otyp);
        break;
    }

    if (wake)
    {
        if (!DEADMONSTER(mtmp))
        {
            wakeup(mtmp, helpful_gesture ? FALSE : TRUE);
            if(!is_peaceful(mtmp))
            {
               m_respond(mtmp);
                if (mtmp->isshk && !*u.ushops)
                    hot_pursuit(mtmp);
            }
        }
        else if (M_AP_TYPE(mtmp))
            seemimic(mtmp); /* might unblock if mimicing a boulder/door */
    }

    /* note: bhitpos won't be set if swallowed, but that's okay since
     * reveal_invis will be false.  We can't use mtmp->mx, my since it
     * might be an invisible worm hit on the tail.
     */
    if (reveal_invis) 
    {
        if (!DEADMONSTER(mtmp) && cansee(bhitpos.x, bhitpos.y)
            && !canspotmon(mtmp))
            map_invisible(bhitpos.x, bhitpos.y);
    }

    /* if effect was observable then discover the wand type provided
       that the wand itself has been seen */
    if (learn_it)
        learnwand(otmp);
    return res;
}

void
probe_monster(mtmp)
struct monst *mtmp;
{
    mstatusline(mtmp);
    if (!notonhead)
    {
        monsterdescription(mtmp);
        //display_monster_information(mtmp);
        display_monster_inventory(mtmp, TRUE);
    }
}

int probe_object(obj)
struct obj* obj;
{
    int res = !obj->dknown;
    /* target object has now been "seen (up close)" */
    obj->dknown = 1;
    if (Is_container(obj) || obj->otyp == STATUE)
    {
        obj->cknown = obj->lknown = obj->tknown = 1;
        if (!obj->cobj)
        {
            pline("%s empty.", Tobjnam(obj, "are"));
        }
        else if (SchroedingersBox(obj))
        {
            /* we don't want to force alive vs dead
               determination for Schroedinger's Cat here,
               so just make probing be inconclusive for it */
            You("aren't sure whether %s has %s or its corpse inside.",
                the(xname(obj)),
                /* unfortunately, we can't tell whether rndmonnam()
                   picks a form which can't leave a corpse */
                an(Hallucination ? rndmonnam((char*)0) : "cat"));
            obj->cknown = 0;
        }
        else
        {
            struct obj* o;
            /* view contents (not recursively) */
            for (o = obj->cobj; o; o = o->nobj)
                o->dknown = 1; /* "seen", even if blind */
            (void)display_cinventory(obj);
        }
        res = 1;
    }
    else if (is_obj_rotting_corpse(obj))
    {
        res = corpsedescription(obj);
    }
    return res;
}

void display_monster_information(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return;

    winid datawin = WIN_ERR;

    datawin = create_nhwindow(NHW_MENU);

    print_monster_intrinsics(datawin, mtmp, mtmp->data);
    print_monster_statistics(datawin, mtmp, mtmp->data);

    display_nhwindow(datawin, FALSE);
    destroy_nhwindow(datawin), datawin = WIN_ERR;

}

void print_monster_intrinsics(datawin, mtmp, ptr)
winid datawin;
struct monst* mtmp;
struct permonst* ptr;
{
    if (!ptr)
        return;

    if (datawin == WIN_ERR)
        return;

    boolean is_you = (mtmp == &youmonst);
    char buf[BUFSZ];

    Sprintf(buf, "%s abilities:", !mtmp || is_you ? "Innate" : "Current"); // , s_suffix(noit_Monnam(mtmp)));
    putstr(datawin, ATR_HEADING, buf);

    int abilcnt = 0;

    for (int i = 1; i <= LAST_PROP; i++)
    {
        boolean has_innate = FALSE;
        boolean has_innate2 = FALSE;
        boolean has_extrinsic = FALSE;
        boolean has_instrinsic_acquired = FALSE;
        boolean has_temporary = FALSE;
        unsigned short temp_dur = 0;

        unsigned long ibit = prop_to_innate(i);
        unsigned long ibit2 = prop_to_innate2(i);

        if (ptr->mresists & ibit)
            has_innate = TRUE;

        if (ptr->mresists2 & ibit2)
            has_innate2 = TRUE;

        if (mtmp && (mtmp->mprops[i] & M_EXTRINSIC) != 0)
            has_extrinsic = TRUE;

        if (mtmp && (mtmp->mprops[i] & M_INTRINSIC_ACQUIRED) != 0)
            has_instrinsic_acquired = TRUE;
        
        temp_dur = mtmp  ? (mtmp->mprops[i] & M_TIMEOUT) : 0;
        if (temp_dur)
            has_temporary = TRUE;

        boolean has_property = has_innate || has_innate2 || has_extrinsic || has_instrinsic_acquired || has_temporary;

        if (has_property)
        {
            char endbuf[BUFSZ];
            char endbuf2[BUFSZ];
            Strcpy(endbuf, "");
            Strcpy(endbuf2, "");

#if 0
            if (has_innate || has_innate2)
            {
                if (strcmp(endbuf, ""))
                    Strcat(endbuf, ", ");

                Strcat(endbuf, "innate");
            }
#endif

            if (has_extrinsic)
            {
                if (strcmp(endbuf, ""))
                    Strcat(endbuf, ", ");

                Strcat(endbuf, "extrinsic");
            }

            if (has_instrinsic_acquired)
            {
                if (strcmp(endbuf, ""))
                    Strcat(endbuf, ", ");

                Strcat(endbuf, "acquired");
            }

            if (has_temporary)
            {
                if (strcmp(endbuf, ""))
                    Strcat(endbuf, ", ");

                Sprintf(eos(endbuf), "temporary for %d rounds", temp_dur);
            }

            if (strcmp(endbuf, ""))
            {
                *endbuf = highc(*endbuf);
                Sprintf(endbuf2, " (%s)", endbuf);
            }

            char namebuf[BUFSZ] = "";
            Strcpy(namebuf, get_property_name(i));
            *namebuf = highc(*namebuf);

            abilcnt++;
            Sprintf(buf, " %2d - %s%s", abilcnt, namebuf, endbuf2);            
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
    }

    if (!abilcnt)
    {
        Strcpy(buf, " (None)");        
        putstr(datawin, 0, buf);
    }

    Sprintf(buf, "Classifications:");// , noit_mon_nam(mtmp));    
    putstr(datawin, ATR_HEADING, buf);

    abilcnt = 0;

    for (int j = 1; j <= NUM_MFLAGS; j++)
    {
        for (int i = 0; i < NUM_UNSIGNED_LONG_BITS; i++)
        {
            unsigned long bit = 0x00000001UL;
            if (i > 0)
                bit = bit << i;

            unsigned long mflags = 
                j == 1 ? ptr->mflags1 : j == 2 ? ptr->mflags2 : j == 3 ? ptr->mflags3 : j == 4 ? ptr->mflags4 :
                j == 5 ? ptr->mflags5 : j == 6 ? ptr->mflags6 : j == 7 ? ptr->mflags7 : j == 8 ? ptr->mflags8 :
                ptr->mflags1; /* Fall back case*/

            if (mflags & bit)
            {
                char descbuf[BUFSZ] = "";
                Strcpy(descbuf, get_mflag_description(bit, FALSE, j));
                if (strcmp(descbuf, ""))
                {
                    *descbuf = highc(*descbuf);
                    abilcnt++;
                    Sprintf(buf, " %2d - %s", abilcnt, descbuf);                    
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                }
            }
        }
    }

    /* Heads */
    if (!(ptr->heads == 1 && (!mtmp || mtmp->heads_left == 1)))
    {
        char headbuf[BUFSZ];
        switch (ptr->heads)
        {
        case 0:
            Strcpy(headbuf, "Headless");
            break;
        case 1:
            Strcpy(headbuf, "Single-headed");
            break;
        case 2:
            Strcpy(headbuf, "Two-headed");
            break;
        case 3:
            Strcpy(headbuf, "Three-headed");
            break;
        default:
            Sprintf(headbuf, "%d-headed", ptr->heads);
            break;
        }

        if (mtmp && (mtmp->heads_left != ptr->heads || mtmp->heads_tamed > 0))
        {
            Strcat(headbuf, " (");
            if(mtmp->heads_left != ptr->heads)
                Sprintf(eos(headbuf), "%d head%s left", mtmp->heads_left, plur(mtmp->heads_left));
            if (mtmp->heads_tamed > 0)
            {
                if (mtmp->heads_left != ptr->heads)
                    Sprintf(eos(headbuf), ", %d tamed", mtmp->heads_tamed);
                else
                    Sprintf(eos(headbuf), "%d head%s tamed", mtmp->heads_tamed, plur(mtmp->heads_tamed));
            }
            Strcat(headbuf, ")");
        }
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, headbuf);        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (!abilcnt)
    {
        Strcpy(buf, " (None)");        
        putstr(datawin, 0, buf);
    }


    Sprintf(buf, "Notable:");// , noit_mon_nam(mtmp));    
    putstr(datawin, ATR_HEADING, buf);

    abilcnt = 0;

    if (mtmp ? mon_hates_silver(mtmp) : hates_silver(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Vulnerable to silver weapons");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (mtmp ? mon_hates_blessed(mtmp) : hates_blessed(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Vulnerable to blessed weapons");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (mtmp ? mon_hates_cursed(mtmp) : hates_cursed(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Vulnerable to cursed weapons");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (mtmp ? mon_hates_light(mtmp) : hates_light(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Vulnerable to lit weapons");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    abilcnt++;
    Sprintf(buf, " %2d - %s", abilcnt, (mtmp ? mon_eschews_cursed(mtmp) : eschews_cursed(ptr) || hates_cursed(ptr)) ? "Eschews cursed items" : "Does not eschew cursed items");    
    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

    if (mtmp ? mon_eschews_silver(mtmp) : eschews_silver(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Eschews silver items");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (mtmp ? mon_eschews_blessed(mtmp) : eschews_blessed(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Eschews blessed items");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (mtmp ? mon_eschews_light(mtmp) : hates_light(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Eschews lit items");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (is_hell_hound(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Loves cursed food");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (is_non_eater(ptr))
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, "Does not eat");        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }


    int zombietype = mon_to_zombie((int)(ptr - &mons[0]));
    int mummytype = mon_to_mummy((int)(ptr - &mons[0]));

    if (zombietype > NON_PM || mummytype > NON_PM)
    {
        abilcnt++;
        Sprintf(buf, " %2d - %s", abilcnt, 
            zombietype > NON_PM && mummytype > NON_PM ? "Zombifiable and mummifiable corpse" : 
            zombietype > NON_PM ? "Zombifiable corpse" : "Mummifiable corpse");
        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (!abilcnt)
    {
        Strcpy(buf, " (None)");        
        putstr(datawin, 0, buf);
    }
}

void print_monster_wearables(datawin, mtmp, ptr)
winid datawin;
struct monst* mtmp UNUSED;
struct permonst* ptr;
{
    if (!ptr)
        return;

    if (datawin == WIN_ERR)
        return;

    int type_count = 0;
    char buf[BUFSZ];
    Strcpy(buf, "Wearable item types:");
    putstr(datawin, ATR_HEADING, buf);

    boolean wears_objects = can_wear_objects(ptr);
    if (!wears_objects)
    {
        type_count++;
        Sprintf(buf, " %2d - %s", type_count, can_wear_saddle(ptr) ? "Saddles" : "None");
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }
    else
    {
        boolean wears_suit = can_wear_suit(ptr);
        boolean wears_robe = can_wear_robe(ptr);
        boolean wears_cloak = can_wear_cloak(ptr);
        boolean wears_shirt = can_wear_shirt(ptr);
        boolean wears_helmet = can_wear_helmet(ptr);
        boolean wears_gloves = can_wear_gloves(ptr);
        boolean wears_bracers = can_wear_bracers(ptr);
        boolean wears_boots = can_wear_boots(ptr);
        boolean wears_amulet = can_wear_amulet(ptr);
        boolean wears_rings = can_wear_rings(ptr);
        boolean wears_blindfold = can_wear_blindfold(ptr);
        boolean wears_shield = can_wear_shield(ptr);
        boolean wields_weapons = can_wield_weapons(ptr);

        boolean all_types = wears_suit && wears_robe && wears_cloak && wears_shirt && wears_helmet && wears_gloves && wears_bracers && wears_boots && wears_amulet && wears_rings 
            && wears_blindfold && wears_shield && wields_weapons;

        if (all_types)
        {
            type_count++;
            Sprintf(buf, " %2d - %s", type_count, can_wear_saddle(ptr) ? "All types including saddles" : "All types");
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        else
        {
            if (wields_weapons)
            {
                type_count++;
                Sprintf(buf, " %2d - Weapons", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_shield)
            {
                type_count++;
                Sprintf(buf, " %2d - Shields", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_suit)
            {
                type_count++;
                Sprintf(buf, " %2d - Suits of armor", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_robe)
            {
                type_count++;
                Sprintf(buf, " %2d - Robes", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_cloak)
            {
                type_count++;
                Sprintf(buf, " %2d - Cloaks", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_shirt)
            {
                type_count++;
                Sprintf(buf, " %2d - Shirts", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_helmet)
            {
                type_count++;
                Sprintf(buf, " %2d - %s", type_count, has_horns(ptr) ? "Flimsy helmets suitable with horns" : "Helmets");
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST,  buf);
            }
            if (wears_gloves)
            {
                type_count++;
                Sprintf(buf, " %2d - Gloves", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_bracers)
            {
                type_count++;
                Sprintf(buf, " %2d - Bracers", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_boots)
            {
                type_count++;
                Sprintf(buf, " %2d - Boots", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_amulet)
            {
                type_count++;
                Sprintf(buf, " %2d - Amulets", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_rings)
            {
                type_count++;
                Sprintf(buf, " %2d - Rings", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (wears_blindfold)
            {
                type_count++;
                Sprintf(buf, " %2d - Blindfolds", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            type_count++;
            Sprintf(buf, " %2d - Some miscellaneous magic items", type_count);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

            /* Additional ones */
            if (can_wear_saddle(ptr))
            {
                type_count++;
                Sprintf(buf, " %2d - Saddles", type_count);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
        }
    }
}

void print_monster_status(datawin, mtmp)
winid datawin;
struct monst* mtmp;
{
    if (!mtmp || datawin == WIN_ERR || !iflags.using_gui_tiles)
        return;

    xchar sx = mtmp == &youmonst ? u.ux : mtmp->mx;
    xchar sy = mtmp == &youmonst ? u.uy : mtmp->my;
    struct layer_info li = isok(sx, sy) ? layers_at(sx, sy) : zerolayerinfo;
    //unsigned long layerflags = li.layer_flags;
    unsigned long layermflags = li.monster_flags;
    boolean loc_is_you = mtmp == &youmonst; // (layerflags & LFLAGS_UXUY) != 0; //So you can separately see your steed stats properly
    boolean ispeaceful = is_peaceful(mtmp) && !is_tame(mtmp);
    boolean ispet = is_tame(mtmp);
    boolean isdetected = (layermflags & LMFLAGS_DETECTED) != 0;
    int condition_count = 0;

    char buf[BUFSZ];
    char sbuf[BUFSZ] = "";
    Strcpy(buf, "Status marks:");
    putstr(datawin, ATR_HEADING, buf);
    
    /* Petmark and other status marks */
    unsigned long m_status_bits = get_m_status_bits(mtmp, loc_is_you, ispeaceful, ispet, isdetected);
    for (int statusorder_idx = 0; statusorder_idx < SIZE(statusmarkorder); statusorder_idx++)
    {
        int status_mark = (int)statusmarkorder[statusorder_idx];
        const char* statusname = 0;
        if (status_mark < MAX_STATUS_MARKS && (statusname = get_status_name(mtmp, status_mark)) != 0)
        {
            unsigned long status_bit = 1UL << status_mark;
            if(m_status_bits & status_bit)
            {
                condition_count++;
                if ((windowprocs.wincap2 & WC2_SPECIAL_SYMBOLS) != 0)
                    Sprintf(sbuf, " &status-%d;", status_mark);
                else
                    *sbuf = 0;

                Sprintf(buf, " %2d - %s%s", condition_count, statusname, sbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
        }
    }

    /* Conditions */
    unsigned long m_conditions = get_m_condition_bits(mtmp);
    for (int cond = 0; cond < ui_tile_component_array[CONDITION_MARKS].number; cond++)
    {
        int condition_bit = 1 << cond;
        if (m_conditions & condition_bit)
        {
            if (cond < NUM_BL_CONDITIONS && condition_names[cond])
            {
                condition_count++;
                if ((windowprocs.wincap2 & WC2_SPECIAL_SYMBOLS) != 0)
                    Sprintf(sbuf, " &cond-%d;", cond);
                else
                    *sbuf = 0;
                Sprintf(buf, " %2d - %s%s", condition_count, condition_names[cond], sbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
        }
    }

    /* Buffs */
    for (int propidx = 1; propidx <= LAST_PROP; propidx++)
    {
        if (!property_definitions[propidx].show_buff)
            continue;

        long duration = loc_is_you ? (u.uprops[propidx].intrinsic & TIMEOUT) : (long)(mtmp->mprops[propidx] & M_TIMEOUT);
        if (duration == 0L)
            continue;

        condition_count++;
        if ((windowprocs.wincap2 & WC2_SPECIAL_SYMBOLS) != 0)
            Sprintf(sbuf, " &buff-%d;", propidx);
        else
            *sbuf = 0;

        char pbuf[BUFSZ];
        Strcpy(pbuf, get_property_name(propidx));
        *pbuf = highc(*pbuf);
        Sprintf(buf, " %2d - %s%s", condition_count, pbuf, sbuf);
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (!condition_count)
    {
        putstr(datawin, ATR_NONE, " (None)");
    }
    
}

void print_monster_statistics(datawin, mtmp, ptr)
winid datawin;
struct monst* mtmp;
struct permonst* ptr;
{
    if (!ptr)
        return;

    if (datawin == WIN_ERR)
        return;

    char buf[BUFSZ];

    boolean are_the_same = !mtmp ? TRUE : (
        ptr->str == M_ACURR(mtmp, A_STR) &&
        ptr->dex == M_ACURR(mtmp, A_DEX) &&
        ptr->con == M_ACURR(mtmp, A_CON) &&
        ptr->intl == M_ACURR(mtmp, A_INT) &&
        ptr->wis == M_ACURR(mtmp, A_WIS) &&
        ptr->cha == M_ACURR(mtmp, A_CHA)
        );

    if(are_the_same)
        Sprintf(buf, "Attribute scores:");
    else
        Sprintf(buf, "Base attribute scores:");

    
    putstr(datawin, ATR_HEADING, buf);

    Sprintf(buf, " St:%s Dx:%d Co:%d In:%d Wi:%d Ch:%d",
        get_strength_string(ptr->str),
        ptr->dex,
        ptr->con,
        ptr->intl,
        ptr->wis,
        ptr->cha
    );
    
    putstr(datawin, 0, buf);

    if (!are_the_same && mtmp)
    {
        Sprintf(buf, "Current attribute scores:");
        
        putstr(datawin, ATR_HEADING, buf);

        Sprintf(buf, " St:%s Dx:%d Co:%d In:%d Wi:%d Ch:%d",
            get_strength_string(M_ACURR(mtmp, A_STR)),
            M_ACURR(mtmp, A_DEX),
            M_ACURR(mtmp, A_CON),
            M_ACURR(mtmp, A_INT),
            M_ACURR(mtmp, A_WIS),
            M_ACURR(mtmp, A_CHA)
        );

        
        putstr(datawin, 0, buf);
    }
}


void
display_monster_inventory(mtmp, probing)
struct monst* mtmp;
boolean probing;
{
    struct obj* otmp;

    if (mtmp->minvent) 
    {
        for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj) 
        {
            otmp->dknown = 1; /* treat as "seen" */
            if (probing && (Is_container(otmp) || otmp->otyp == STATUE)) 
            {
                otmp->lknown = otmp->tknown = 1;
                if (!SchroedingersBox(otmp))
                    otmp->cknown = 1;
            }
        }
        (void)display_minventory(mtmp, MINV_ALL | MINV_NOLET | PICK_NONE,
            (char*)0);
    }
    else 
    {
        pline("%s is not carrying anything%s.", noit_Monnam(mtmp),
            (u.uswallow && mtmp == u.ustuck) ? " besides you" : "");
    }

}

/*
 * Return the object's physical location.  This only makes sense for
 * objects that are currently on the level (i.e. migrating objects
 * are nowhere).  By default, only things that can be seen (in hero's
 * inventory, monster's inventory, or on the ground) are reported.
 * By adding BURIED_TOO and/or CONTAINED_TOO flags, you can also get
 * the location of buried and contained objects.  Note that if an
 * object is carried by a monster, its reported position may change
 * from turn to turn.  This function returns FALSE if the position
 * is not available or subject to the constraints above.
 */
boolean
get_obj_location(obj, xp, yp, locflags)
struct obj *obj;
xchar *xp, *yp;
int locflags;
{
    switch (obj->where) {
    case OBJ_INVENT:
        *xp = u.ux;
        *yp = u.uy;
        return TRUE;
    case OBJ_FLOOR:
        *xp = obj->ox;
        *yp = obj->oy;
        return TRUE;
    case OBJ_MINVENT:
        if (obj->ocarry->mx) {
            *xp = obj->ocarry->mx;
            *yp = obj->ocarry->my;
            return TRUE;
        }
        break; /* !mx => migrating monster */
    case OBJ_BURIED:
        if (locflags & BURIED_TOO) {
            *xp = obj->ox;
            *yp = obj->oy;
            return TRUE;
        }
        break;
    case OBJ_CONTAINED:
        if (locflags & CONTAINED_TOO)
            return get_obj_location(obj->ocontainer, xp, yp, locflags);
        break;
    }
    *xp = *yp = 0;
    return FALSE;
}

boolean
get_mon_location(mon, xp, yp, locflags)
struct monst *mon;
xchar *xp, *yp;
int locflags; /* non-zero means get location even if monster is buried */
{
    if (mon == &youmonst) {
        *xp = u.ux;
        *yp = u.uy;
        return TRUE;
    } else if (mon->mx > 0 && (!mon->mburied || locflags)) {
        *xp = mon->mx;
        *yp = mon->my;
        return TRUE;
    } else { /* migrating or buried */
        *xp = *yp = 0;
        return FALSE;
    }
}


boolean
get_region_location(reg, xp, yp, locflags)
struct nhregion* reg;
xchar* xp, * yp;
int locflags; /* Unused */
{
    if (!reg)
        return FALSE;

    if (locflags)
    {
        /* Unused */
    }

    if (hero_inside(reg))
    {
        *xp = u.ux;
        *yp = u.uy;
        return TRUE;
    }

    float hearvalue = 0.0f;
    xchar fx = 0, fy = 0;
    for (int i = 0; i < reg->nrects; i++)
    {
        if (isok(reg->rects[i].lx, reg->rects[i].ly) && isok(reg->rects[i].hx, reg->rects[i].hy))
        {
            for (int x = reg->rects[i].lx; x <= reg->rects[i].hx; x++)
            {
                for (int y = reg->rects[i].ly; y <= reg->rects[i].hy; y++)
                {
                    float h = hearing_array[x][y];
                    if (h > hearvalue || i == 0)
                    {
                        hearvalue = h;
                        fx = (xchar)x;
                        fy = (xchar)y;
                        if (hearvalue == 1.0f)
                        {
                            /* Stop immediately */
                            *xp = fx;
                            *yp = fy;
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    *xp = fx;
    *yp = fy;
    if (fx == 0 && fy == 0)
        return FALSE;
    else
        return TRUE;
}

/* used by revive() and animate_statue() */
struct monst *
montraits(obj, cc, adjacentok, mnum_override, mnum_replaceundead, mmflags)
struct obj *obj;
coord *cc;
boolean adjacentok; /* False: at obj's spot only, True: nearby is allowed */
int mnum_override, mnum_replaceundead; /* Use this mnum instead */
unsigned long mmflags;
{
    struct monst *mtmp = (struct monst *) 0;
    struct monst *mtmp2 = (struct monst *) 0;

    if (has_omonst(obj))
        mtmp2 = get_mtraits(obj, TRUE);

    if (mtmp2) 
    {
        /* save_mtraits() validated mtmp2->mnum */
        if (mnum_override >= LOW_PM || (mnum_replaceundead >= LOW_PM && is_undead(&mons[mnum_replaceundead])) || !mtmp2->data)
        {
            int used_mnum = mnum_override >= LOW_PM ? mnum_override : (mnum_replaceundead >= LOW_PM && is_undead(&mons[mnum_replaceundead])) ? mnum_replaceundead : mtmp2->mnum;
            mtmp2->mnum = used_mnum;
            mtmp2->subtype = 0;
            mtmp2->data = &mons[used_mnum];
        }

        if (mtmp2->mhpmax <= 0 && !is_rider(mtmp2->data))
            return (struct monst *) 0;
        
        mtmp = makemon2(mtmp2->data, cc->x, cc->y,
                       (mmflags | MM_NO_MONSTER_INVENTORY | MM_NOWAIT | MM_NOCOUNTBIRTH
                        | (adjacentok ? MM_ADJACENTOK : 0)), MM2_REVIVING);
        if (!mtmp)
            return mtmp;

        /* Get these ones from mtmp */
        mtmp2->minvent = mtmp->minvent; /*redundant*/
        
        /* monster ID is available if the monster died in the current
           game, but will be zero if the corpse was in a bones level
           (we cleared it when loading bones) */
        if (mtmp->m_id) {
            mtmp2->m_id = mtmp->m_id;
            /* might be bringing quest leader back to life */
            if (quest_status.leader_is_dead
                /* leader_is_dead implies leader_m_id is valid */
                && mtmp2->m_id == quest_status.leader_m_id)
                quest_status.leader_is_dead = FALSE;
        }
        mtmp2->mx = mtmp->mx;
        mtmp2->my = mtmp->my;
        mtmp2->mux = mtmp->mux;
        mtmp2->muy = mtmp->muy;
        mtmp2->mw = mtmp->mw;
        mtmp2->wormno = mtmp->wormno;
        mtmp2->worn_item_flags = mtmp->worn_item_flags;
        mtmp2->weapon_strategy = mtmp->weapon_strategy;
        mtmp2->mtrapseen = mtmp->mtrapseen;
        mtmp2->mflee = mtmp->mflee;
        mtmp2->mburied = mtmp->mburied;
        mtmp2->mundetected = mtmp->mundetected;
        mtmp2->mflee_timer = mtmp->mflee_timer;
        mtmp2->mlstmv = mtmp->mlstmv;
        mtmp2->m_ap_type = mtmp->m_ap_type;
        mtmp2->leaves_no_corpse = mtmp->leaves_no_corpse;
        mtmp2->delayed_killer_by_you = mtmp->delayed_killer_by_you;
        /* set these ones explicitly */
        mtmp2->mrevived = mtmp2->mrevived + (mtmp->mrevived < MAX_MONST_REVIVALS ? 1 : 0);
        mtmp2->mavenge = 0;
        mtmp2->meating = 0;
        mtmp2->mleashed = 0;
        mtmp2->mtrapped = 0;
        mtmp2->msleeping = 0;
        mtmp2->mfrozen = 0;
        mtmp2->mcanmove = 1;
        /* most cancelled monsters return to normal,
           but some need to stay cancelled */
        if (!dmgtype(mtmp2->data, AD_SEDU)
            && (!SYSOPT_SEDUCE || !dmgtype(mtmp2->data, AD_SSEX)))
        {
            mtmp2->mprops[CANCELLED] = 0;
        }
        mtmp2->mprops[NO_MAGIC_RESISTANCE] = 0;
        mtmp2->mprops[ONE_FOURTH_MAGIC_RESISTANCE] = 0;
        mtmp2->mprops[HALVED_MAGIC_RESISTANCE] = 0;
        mtmp2->mprops[THREE_FOURTHS_MAGIC_RESISTANCE] = 0;
        mtmp2->mprops[SUMMON_FORBIDDEN] = 0;
        mtmp2->mprops[TITAN_STRENGTH] = 0;
        mtmp2->mprops[DIVINE_DEXTERITY] = 0;
        mtmp2->mprops[DIVINE_ENDURANCE] = 0;
        mtmp2->mprops[DIVINE_INTELLECT] = 0;
        mtmp2->mprops[DIVINE_WISDOM] = 0;
        mtmp2->mprops[DIVINE_CHARISMA] = 0;
        mtmp2->mprops[BLINDED] = 0;
        mtmp2->mprops[STUNNED] = 0;
        mtmp2->mprops[CONFUSION] = 0;

        if (mtmp2->isshk) 
        {
            neweshk(mtmp);
            *ESHK(mtmp) = *ESHK(mtmp2);
            if (ESHK(mtmp2)->bill_p != 0
                && ESHK(mtmp2)->bill_p != (struct bill_x *) -1000)
                ESHK(mtmp)->bill_p = &(ESHK(mtmp)->bill[0]);
            mtmp->isshk = 1;
        }

        /* grow the heads back */
        if (mtmp2->data->heads > 0 && mtmp->heads_left == 0)
            mtmp->heads_left = mtmp2->data->heads;

        /* heal the monster */
        if (mtmp->mbasehpmax > mtmp2->mbasehpmax&& is_rider(mtmp2->data))
            mtmp2->mbasehpmax = mtmp->mbasehpmax;

        update_mon_maxhp(mtmp);
        update_mon_maxhp(mtmp2);
        mtmp2->mhp = mtmp2->mhpmax;

        replmon(mtmp, mtmp2);
        newsym(mtmp2->mx, mtmp2->my); /* Might now be invisible */

        /* in case Protection_from_shape_changers is different
           now than it was when the traits were stored */
        restore_cham(mtmp2);
    }
    return mtmp2;
}

/*
 * get_container_location() returns the following information
 * about the outermost container:
 * loc argument gets set to:
 *      OBJ_INVENT      if in hero's inventory; return 0.
 *      OBJ_FLOOR       if on the floor; return 0.
 *      OBJ_BURIED      if buried; return 0.
 *      OBJ_MINVENT     if in monster's inventory; return monster.
 * container_nesting is updated with the nesting depth of the containers
 * if applicable.
 */
struct monst *
get_container_location(obj, loc, container_nesting)
struct obj *obj;
int *loc;
int *container_nesting;
{
    if (!obj || !loc)
        return 0;

    if (container_nesting)
        *container_nesting = 0;
    while (obj && obj->where == OBJ_CONTAINED) {
        if (container_nesting)
            *container_nesting += 1;
        obj = obj->ocontainer;
    }
    if (obj) {
        *loc = obj->where; /* outermost container's location */
        if (obj->where == OBJ_MINVENT)
            return obj->ocarry;
    }
    return (struct monst *) 0;
}

/*
 * Attempt to revive the given corpse, return the revived monster if
 * successful.  Note: this does NOT use up the corpse if it fails.
 * If corpse->quan is more than 1, only one corpse will be affected
 * and only one monster will be resurrected.
 */
struct monst *
revive(corpse, by_hero, animateintomon, replaceundead)
struct obj *corpse;
boolean by_hero;
int animateintomon;
boolean replaceundead;
{
    struct monst *mtmp = 0;
    struct permonst *mptr = 0;
    struct obj *container;
    coord xy;
    xchar x, y;
    boolean one_of;
    int montype, container_nesting = 0;

    if (corpse->otyp != CORPSE) 
    {
        impossible("Attempting to revive %s?", xname(corpse));
        return (struct monst *) 0;
    }

    x = y = 0;

    if (corpse->where != OBJ_CONTAINED)
    {
        /* only for invent, minvent, or floor */
        container = 0;
        (void) get_obj_location(corpse, &x, &y, 0);
    }
    else 
    {
        /* deal with corpses in [possibly nested] containers */
        struct monst *carrier;
        int holder = OBJ_FREE;

        container = corpse->ocontainer;
        carrier =
            get_container_location(container, &holder, &container_nesting);
        switch (holder) 
        {
        case OBJ_MINVENT:
            x = carrier->mx, y = carrier->my;
            break;
        case OBJ_INVENT:
            x = u.ux, y = u.uy;
            break;
        case OBJ_FLOOR:
            (void) get_obj_location(corpse, &x, &y, CONTAINED_TOO);
            break;
        default:
            break; /* x,y are 0 */
        }
    }
    
    if (!x || !y
        /* Rules for revival from containers:
         *  - the container cannot be locked
         *  - the container cannot be heavily nested (>2 is arbitrary)
         *  - the container cannot be a statue or bag of holding
         *    (except in very rare cases for the latter)
         */
        || (container && (container->olocked || container_nesting > 2
                          || container->otyp == STATUE
                          || (container->otyp == BAG_OF_HOLDING && rn2(40))
                          || (container->otyp == BAG_OF_WIZARDRY && rn2(60))
                          || (container->otyp == BAG_OF_TREASURE_HAULING && rn2(80))
                          || (container->otyp == BAG_OF_THE_GLUTTON && rn2(80))
            )))
        return (struct monst *) 0;

    /* record the object's location now that we're sure where it is */
    corpse->ox = x, corpse->oy = y;

    /* prepare for the monster */
    if(animateintomon < 0)
    {
        montype = corpse->corpsenm;
        if (montype >= LOW_PM && is_undead(&mons[montype]) && replaceundead)
        {
            /* Human and dwarf corpses etc. but others need to be replaced here are ok */
            if (mons[montype].mlet == S_WRAITH)
                montype = PM_HUMAN;
            else if (montype == PM_VAMPIRE_MAGE)
                montype = PM_WIZARD;
            else if (montype == PM_SKELETON)
                montype = PM_HUMAN;
            else if (montype == PM_SKELETON_LORD)
                montype = PM_HUMAN;
            else if (montype == PM_SKELETON_KING)
                montype = PM_HUMAN;
            else if (montype == PM_GIANT_SKELETON)
                montype = PM_GIANT;
            else if (mons[montype].mlet == S_VAMPIRE)
                montype = PM_HUMAN;
        }
    }
    else
    {
        montype = animateintomon;
    }

    if(montype >= LOW_PM)
        mptr = &mons[montype];
    /* [should probably handle recorporealization first; if corpse and
       ghost are at same location, revived creature shouldn't be bumped
       to an adjacent spot by ghost which joins with it] */
    if (MON_AT(x, y)) 
    {
        if (enexto(&xy, x, y, mptr))
            x = xy.x, y = xy.y;
    }

    if ((mons[montype].mlet == S_EEL && !IS_POOL(levl[x][y].typ)) 
        || item_prevents_revival(montype)) 
    {
        if (by_hero && cansee(x, y))
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s twitches feebly.",
                upstart(corpse_xname(corpse, (const char *) 0, CXN_PFX_THE)));
        return (struct monst *) 0;
    }

    if (cant_revive(&montype, TRUE, corpse)) 
    {
        /* make a zombie or doppelganger instead */
        /* note: montype has changed; mptr keeps old value for newcham() */
        mtmp = makemon(montype < LOW_PM ? 0 : &mons[montype], x, y, MM_NO_MONSTER_INVENTORY | MM_NOWAIT | MM_PLAY_SUMMON_ANIMATION | MM_ANIMATE_DEAD_ANIMATION | MM_PLAY_SUMMON_SOUND);
        if (mtmp)
        {
            int subtype = 0;
            /* skip ghost handling */
            if (has_omid(corpse))
                free_omid(corpse);
            if (has_omonst(corpse))
            {
                struct monst* omon = OMONST(corpse);
                subtype = omon->subtype;
                free_omonst(corpse);
            }
            if (mtmp->cham == PM_DOPPELGANGER) {
                /* change shape to match the corpse */
                (void) newcham(mtmp, mptr, subtype, FALSE, FALSE);
            } else if (mtmp->data->mlet == S_LESSER_UNDEAD) 
            {
                mtmp->mhp = mtmp->mhpmax = 100;
                mtmp->mprops[VERY_FAST] |= M_INTRINSIC_ACQUIRED;
            }
        }
    } 
    else if (has_omonst(corpse))
    {
        /* use saved traits */
        xy.x = x, xy.y = y;
        mtmp = montraits(corpse, &xy, FALSE, animateintomon >= 0 ? montype : NON_PM, animateintomon < 0 && replaceundead ? montype : NON_PM, MM_PLAY_SUMMON_ANIMATION | MM_ANIMATE_DEAD_ANIMATION | MM_PLAY_SUMMON_SOUND);
        if (mtmp && mtmp->mtame && !mtmp->isminion && !mtmp->isfaithful)
            wary_dog(mtmp, TRUE);
    }
    else 
    {
        /* make a new monster */
        mtmp = makemon2(mptr, x, y, MM_NO_MONSTER_INVENTORY | MM_NOWAIT | MM_NOCOUNTBIRTH | MM_PLAY_SUMMON_ANIMATION | MM_ANIMATE_DEAD_ANIMATION | MM_PLAY_SUMMON_SOUND, MM2_REVIVING);
    }

    if (!mtmp)
        return (struct monst *) 0;

    /* hiders shouldn't already be re-hidden when they revive */
    if (mtmp->mundetected) 
    {
        mtmp->mundetected = 0;
        newsym(mtmp->mx, mtmp->my);
    }

    if (M_AP_TYPE(mtmp))
        seemimic(mtmp);

    one_of = (corpse->quan > 1L);
    if (one_of)
        corpse = splitobj(corpse, 1L);

    /* if this is caused by the hero there might be a shop charge */
    if (by_hero)
    {
        struct monst *shkp = 0;

        x = corpse->ox, y = corpse->oy;
        if (costly_spot(x, y)
            && (carried(corpse) ? corpse->unpaid : !corpse->no_charge))
            shkp = shop_keeper(*in_rooms(x, y, SHOPBASE));

        if (cansee(x, y))
        {
            char buf[BUFSZ];

            Strcpy(buf, one_of ? "one of " : "");
            /* shk_your: "the " or "your " or "<mon>'s " or "<Shk>'s ".
               If the result is "Shk's " then it will be ambiguous:
               is Shk the mon carrying it, or does Shk's shop own it?
               Let's not worry about that... */
            (void) shk_your(eos(buf), corpse);
            if (one_of)
                corpse->quan++; /* force plural */
            Strcat(buf, corpse_xname(corpse, (const char *) 0, CXN_NO_PFX));
            if (one_of) /* could be simplified to ''corpse->quan = 1L;'' */
                corpse->quan--;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s glows iridescently.", upstart(buf));
        } 
        else if (shkp)
        {
            /* need some prior description of the corpse since
               stolen_value() will refer to the object as "it" */
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A corpse is resuscitated.");
        }

        /* don't charge for shopkeeper's own corpse if we just revived him */
        if (shkp && mtmp != shkp)
            (void) stolen_value(corpse, x, y, is_peaceful(shkp),
                                FALSE);

        /* [we don't give any comparable message about the corpse for
           the !by_hero case because caller might have already done so] */
    }

    /* handle recorporealization of an active ghost */
    if (has_omid(corpse))
    {
        unsigned m_id;
        struct monst *ghost;
        struct obj *otmp;

        (void) memcpy((genericptr_t) &m_id, (genericptr_t) OMID(corpse),
                      sizeof m_id);
        ghost = find_mid(m_id, FM_FMON);
        if (ghost && ghost->data == &mons[PM_GHOST]) 
        {
            if (canseemon(ghost))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is suddenly drawn into its former body!",
                      Monnam(ghost));
            /* transfer the ghost's inventory along with it */
            while ((otmp = ghost->minvent) != 0) 
            {
                obj_extract_self(otmp);
                add_to_minv(mtmp, otmp);
            }

            /* tame the revived monster if its ghost was tame */
            if (ghost->mtame && !mtmp->mtame)
            {
                if (tamedog(mtmp, (struct obj *) 0, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, FALSE, FALSE)) {
                    /* ghost's edog data is ignored */
                    mtmp->mtame = ghost->mtame;
                }
            }

            /* was ghost, now alive, it's all very confusing */
            mtmp->mprops[CONFUSION] |= M_INTRINSIC_ACQUIRED;
            /* separate ghost monster no longer exists */
            mongone(ghost);
        }
        free_omid(corpse);
    }

    /* monster retains its name */
    /* //This should come from OMONST -- JG
    if (has_oname(corpse) && !unique_corpstat(mtmp->data))
    {
        mtmp = christen_monst(mtmp, ONAME(corpse));
        if(corpse->nknown)
            mtmp->u_know_mname = 1;
    }
    if (has_uoname(corpse) && !unique_corpstat(mtmp->data))
    {
        mtmp = u_name_monst(mtmp, UONAME(corpse));
    }
    */
    /* partially eaten corpse yields wounded monster */
    if (corpse->oeaten)
        mtmp->mhp = eaten_stat(mtmp->mhp, corpse);
    /* track that this monster was revived at least once */
    mtmp->mrevived = 1;

    if (animateintomon >= 0)
    {
        //Animated are tamed
        tamedog(mtmp, (struct obj*) 0, TAMEDOG_FORCE_NON_UNIQUE, FALSE, 0, FALSE, FALSE);
        mtmp->disregards_enemy_strength = TRUE;
        mtmp->disregards_own_health = TRUE;
    }

    /* finally, get rid of the corpse--it's gone now */
    switch (corpse->where) {
    case OBJ_INVENT:
        useup(corpse);
        break;
    case OBJ_FLOOR:
        /* in case MON_AT+enexto for invisible mon */
        x = corpse->ox, y = corpse->oy;
        /* not useupf(), which charges */
        if (corpse->quan > 1L)
            corpse = splitobj(corpse, 1L);
        delobj(corpse);
        newsym(x, y);
        break;
    case OBJ_MINVENT:
        m_useup(corpse->ocarry, corpse);
        break;
    case OBJ_CONTAINED:
        obj_extract_self(corpse);
        obfree(corpse, (struct obj *) 0);
        break;
    default:
        panic("revive");
    }

    flush_screen(1);
    special_effect_wait_until_end(context.makemon_spef_idx);

    return mtmp;
}


boolean
item_prevents_revival(montype)
int montype;
{
    struct obj* uitem;

    //Add then extrinsics from all carried items
    for (uitem = invent; uitem; uitem = uitem->nobj)
    {
        if (uitem->oartifact && artifact_has_flag(uitem, AF_PREVENTS_REVIVAL_OF_MON))
        {
            if (artifact_has_flag(uitem, AF_DFLAG1))
            {
                if (mons[montype].mflags1 & artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
            else if (artifact_has_flag(uitem, AF_DFLAG2))
            {
                if (mons[montype].mflags2 & artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
            else if (artifact_has_flag(uitem, AF_DMONS))
            {
                if ((unsigned long)montype == artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
            else if (artifact_has_flag(uitem, AF_DCLAS))
            {
                if ((unsigned long)mons[montype].mlet == artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
            else if (artifact_has_flag(uitem, AF_DALIGN))
            {
                if (sgn((int)mons[montype].maligntyp) != artilist[uitem->oartifact].alignment)
                    return TRUE;
            }
            else
            {
                if ((unsigned long)montype == artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
        }

        if (!object_uses_spellbook_wand_flags_and_properties(uitem))
        {
            int otyp = uitem->otyp;
            boolean inappr = inappropriate_character_type(uitem);
            boolean worn = is_obj_worn(uitem);

            if ((worn || (!worn && (objects[otyp].oc_flags & O1_OFLAG_POWERS_APPLY_WHEN_CARRIED)))
                && ((!inappr && !(objects[otyp].oc_flags & (O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    || (objects[otyp].oc_flags & O1_OFLAG_POWERS_APPLY_TO_ALL_CHARACTERS)
                    || (inappr && (objects[otyp].oc_flags & (O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    )
                )
            {
                if (objects[otyp].oc_flags3 & O3_PREVENTS_REVIVAL_OF_PERMITTED_TARGETS)
                {
                    if (objects[otyp].oc_target_permissions == ALL_TARGETS)
                        return TRUE;

                    if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M1_FLAG)
                    {
                        if (mons[montype].mflags1 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M2_FLAG)
                    {
                        if (mons[montype].mflags2 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M3_FLAG)
                    {
                        if (mons[montype].mflags3 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M4_FLAG)
                    {
                        if (mons[montype].mflags4 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M5_FLAG)
                    {
                        if (mons[montype].mflags5 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else
                    {
                        if ((unsigned long)mons[montype].mlet == objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                }
            }

        }

        /* Mythic */
        for (uchar j = 0; j <= 1; j++)
        {
            uchar mythic_quality = (j == 0 ? uitem->mythic_prefix : uitem->mythic_suffix);
            if (mythic_quality == 0)
                continue;

            const struct mythic_power_definition* mythic_powers = (j == 0 ? mythic_prefix_powers : mythic_suffix_powers);
            const struct mythic_definition* mythic_definitions = (j == 0 ? mythic_prefix_qualities : mythic_suffix_qualities);
            uchar max_mythic_powers = (j == 0 ? MAX_MYTHIC_PREFIX_POWERS : MAX_MYTHIC_SUFFIX_POWERS);

            for (uchar i = 0; i < max_mythic_powers; i++)
            {
                if (!mythic_powers[i].name)
                    break;

                unsigned long mythic_power_bit = 1UL << ((unsigned long)i);

                if ((mythic_definitions[mythic_quality].mythic_powers & mythic_power_bit) && mythic_power_applies_to_obj(uitem, mythic_powers[i].power_flags))
                {
                    if (mythic_powers[i].power_type == MYTHIC_POWER_TYPE_PREVENTS_REVIVAL && (mythic_powers[i].parameter3 == mons[montype].mlet || (mythic_powers[i].parameter4 & mons[montype].mflags2)))
                    {
                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}


boolean
item_prevents_summoning(montype)
int montype;
{
    struct obj* uitem;

    //Add then extrinsics from all carried items
    for (uitem = invent; uitem; uitem = uitem->nobj)
    {
        if (uitem->oartifact && artifact_has_flag(uitem, AF_PREVENTS_SUMMONING_OF_MON))
        {
            if (artifact_has_flag(uitem, AF_DFLAG1))
            {
                if (mons[montype].mflags1 & artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
            else if (artifact_has_flag(uitem, AF_DFLAG2))
            {
                if (mons[montype].mflags2 & artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
            else if (artifact_has_flag(uitem, AF_DMONS))
            {
                if ((unsigned long)montype == artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
            else if (artifact_has_flag(uitem, AF_DCLAS))
            {
                if ((unsigned long)mons[montype].mlet == artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
            else if (artifact_has_flag(uitem, AF_DALIGN))
            {
                if (sgn(mons[montype].maligntyp) != artilist[uitem->oartifact].alignment)
                    return TRUE;
            }
            else
            {
                if ((unsigned long)montype == artilist[uitem->oartifact].mtype)
                    return TRUE;
            }
        }

        if (!object_uses_spellbook_wand_flags_and_properties(uitem))
        {
            int otyp = uitem->otyp;
            boolean inappr = inappropriate_character_type(uitem);
            boolean worn = is_obj_worn(uitem);

            if ((worn || (!worn && (objects[otyp].oc_flags & O1_OFLAG_POWERS_APPLY_WHEN_CARRIED)))
                && ((!inappr && !(objects[otyp].oc_flags & (O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    || (objects[otyp].oc_flags & O1_OFLAG_POWERS_APPLY_TO_ALL_CHARACTERS)
                    || (inappr && (objects[otyp].oc_flags & (O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    )
                )
            {

                if (objects[otyp].oc_flags3 & O3_PREVENTS_SUMMONING_BY_PERMITTED_TARGETS)
                {
                    if (objects[otyp].oc_target_permissions == ALL_TARGETS)
                        return TRUE;

                    if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M1_FLAG)
                    {
                        if (mons[montype].mflags1 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M2_FLAG)
                    {
                        if (mons[montype].mflags2 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M3_FLAG)
                    {
                        if (mons[montype].mflags3 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M4_FLAG)
                    {
                        if (mons[montype].mflags4 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M5_FLAG)
                    {
                        if (mons[montype].mflags5 & objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                    else
                    {
                        if ((unsigned long)mons[montype].mlet == objects[otyp].oc_target_permissions)
                            return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}

STATIC_OVL void
revive_egg(obj)
struct obj *obj;
{
    /*
     * Note: generic eggs with corpsenm set to NON_PM will never hatch.
     */
    if (obj->otyp != EGG)
        return;
    if (obj->corpsenm != NON_PM && !dead_species(obj->corpsenm, TRUE))
        attach_egg_hatch_timeout(obj, 0L);
}

/* try to revive all corpses and eggs carried by `mon' */
int
revive_from_inventory(mon)
struct monst *mon;
{
    struct obj *otmp, *otmp2;
    struct monst *mtmp2;
    char owner[BUFSZ], corpse[BUFSZ];
    boolean youseeit;
    int res = 0;

    youseeit = (mon == &youmonst) ? TRUE : canseemon(mon);
    otmp2 = (mon == &youmonst) ? invent : mon->minvent;
    owner[0] = corpse[0] = '\0'; /* lint suppression */

    while ((otmp = otmp2) != 0) {
        otmp2 = otmp->nobj;
        if (otmp->otyp == EGG)
            revive_egg(otmp);
        if (otmp->otyp != CORPSE)
            continue;
        /* save the name; the object is liable to go away */
        if (youseeit) {
            Strcpy(corpse,
                   corpse_xname(otmp, (const char *) 0, CXN_SINGULAR));
            Shk_Your(owner, otmp); /* includes a trailing space */
        }

        /* for a stack, only one is revived */
        if ((mtmp2 = revive(otmp, !context.mon_moving, -1, FALSE)) != 0) {
            ++res;
            if (youseeit)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%s suddenly comes alive!", owner, corpse);
            else if (canseemon(mtmp2))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly appears!", Amonnam(mtmp2));
        }
    }
    return res;
}

/* cancel obj, possibly carried by you or a monster */
void
cancel_item(obj, update_inv)
register struct obj *obj;
boolean update_inv;
{
    int otyp = obj->otyp;

    if (!(objects[otyp].oc_flags5 & O5_NOT_CANCELLABLE) && (objects[otyp].oc_magic
        || (obj->charges && objects[otyp].oc_charged)
        || (obj->enchantment && (obj->oclass == ARMOR_CLASS
                         || obj->oclass == WEAPON_CLASS || is_weptool(obj) || objects[otyp].oc_enchantable))
        || otyp == POT_ACID
        || otyp == POT_SICKNESS
        || otyp == POT_POISON
        || (otyp == POT_WATER && (obj->blessed || obj->cursed))))
    {
        
//        if (otyp != WAN_CANCELLATION && otyp != WAN_DISJUNCTION /* can't cancel cancellation */)
//        {
            costly_alteration(obj, COST_CANCEL);
            obj->charges = (obj->oclass == WAND_CLASS) ? -1 : 0;
            obj->enchantment = 0;
//        }
        switch (obj->oclass) {
        case SCROLL_CLASS:
            costly_alteration(obj, COST_CANCEL);
            obj->otyp = SCR_BLANK_PAPER;
            obj->material = objects[obj->otyp].oc_material;
            obj->enchantment = 0;
            obj->special_quality = 0;
            break;
        case SPBOOK_CLASS:
            if (objects[otyp].oc_multigen_type == BOOKTYPE_SPELLBOOK
                && !objects[otyp].oc_unique && !(objects[otyp].oc_flags & O1_INDESTRUCTIBLE) && obj->oartifact == 0)
            {
                costly_alteration(obj, COST_CANCEL);
                obj->otyp = SPE_BLANK_PAPER;
                obj->material = objects[obj->otyp].oc_material;
            }
            break;
        case POTION_CLASS:
            costly_alteration(obj,
                              (otyp != POT_WATER)
                                  ? COST_CANCEL
                                  : obj->cursed ? COST_UNCURS : COST_UNBLSS);
            
            if (otyp == POT_SICKNESS || otyp == POT_SEE_INVISIBLE) 
            {
                /* sickness is "biologically contaminated" fruit juice;
                   cancel it and it just becomes fruit juice...
                   whereas see invisible tastes like "enchanted" fruit
                   juice, it similarly cancels */
                obj->otyp = POT_FRUIT_JUICE;
            } 
            else 
            {
                obj->otyp = POT_WATER;
                obj->odiluted = 0; /* same as any other water */
            }
            break;
        }
    }
    unbless(obj);
    uncurse(obj);
    obj->elemental_enchantment = 0;
    updateabon();
    updatemaxen();
    updatemaxhp();

    if (update_inv && obj->where == OBJ_INVENT)
        update_inventory();

    return;
}

/* Remove a positive enchantment or charge from obj,
 * possibly carried by you or a monster
 */
boolean
drain_item(obj, by_you)
struct obj *obj;
boolean by_you;
{
    //boolean u_ring;

    /* Is this a charged/enchanted object? */
    if (!obj
        || !(objects[obj->otyp].oc_charged || objects[obj->otyp].oc_enchantable || obj->oclass == WEAPON_CLASS
            || obj->oclass == ARMOR_CLASS || is_weptool(obj))
        || (obj->enchantment <= 0 && obj->charges <= 0))
        return FALSE;
    
    if (obj_resists(obj, 10, 90))
        return FALSE;

    /* Charge for the cost of the object */
    if (by_you)
        costly_alteration(obj, COST_DRAIN);

    /* Drain the object and any implied effects */
    if(objects[obj->otyp].oc_enchantable && obj->enchantment > 0)
        obj->enchantment--;

    if (objects[obj->otyp].oc_charged && obj->charges > 0)
        obj->charges--;

    context.botl = 1;

    updateabon();
    updatemaxen();
    updatemaxhp();

    if (context.botl)
        bot();
    if (carried(obj))
        update_inventory();
    return TRUE;
}

boolean
obj_resists(obj, ochance, achance)
struct obj *obj;
int ochance, achance; /* percent chance for ordinary objects, artifacts */
{
    if (is_obj_unremovable_from_the_game(obj)
        || is_obj_indestructible(obj))
    {
        return TRUE;
    }
    else 
    {
        int chance = rn2(100);

        return (boolean) (chance < (obj->oartifact ? achance : ochance));
    }
}

boolean
obj_shudders(obj)
struct obj *obj;
{
    int zap_odds;

    if (context.bypasses && obj->bypass)
        return FALSE;

    if (obj->oclass == WAND_CLASS)
        zap_odds = 3; /* half-life = 2 zaps */
    else if (obj->cursed)
        zap_odds = 3; /* half-life = 2 zaps */
    else if (obj->blessed)
        zap_odds = 12; /* half-life = 8 zaps */
    else
        zap_odds = 8; /* half-life = 6 zaps */

    /* adjust for "large" quantities of identical things */
    if (obj->quan > 4L)
        zap_odds /= 2;

    return (boolean) !rn2(zap_odds);
}

/* Use up at least minwt number of things made of material mat.
 * There's also a chance that other stuff will be used up.  Finally,
 * there's a random factor here to keep from always using the stuff
 * at the top of the pile.
 */
STATIC_OVL void
polyuse(objhdr, mat, minwt)
struct obj *objhdr;
int mat, minwt;
{
    register struct obj *otmp, *otmp2;

    for (otmp = objhdr; minwt > 0 && otmp; otmp = otmp2) {
        otmp2 = otmp->nexthere;
        if (context.bypasses && otmp->bypass)
            continue;
        if (otmp == uball || otmp == uchain)
            continue;
        if (obj_resists(otmp, 0, 0))
            continue; /* preserve unique objects */
        if (otmp->otyp == SCR_MAIL)
            continue;

        if (((int)otmp->material == mat)
            == (rn2(minwt + 1) != 0)) {
            /* appropriately add damage to bill */
            if (costly_spot(otmp->ox, otmp->oy)) {
                if (*u.ushops)
                    addtobill(otmp, FALSE, FALSE, FALSE);
                else
                    (void) stolen_value(otmp, otmp->ox, otmp->oy, FALSE,
                                        FALSE);
            }
            if (otmp->quan < LARGEST_INT)
                minwt -= (int) otmp->quan;
            else
                minwt = 0;
            delobj(otmp);
        }
    }
}

/*
 * Polymorph some of the stuff in this pile into a monster, preferably
 * a golem of the kind okind.
 */
STATIC_OVL void
create_polymon(obj, okind)
struct obj *obj;
int okind;
{
    struct permonst *mdat = (struct permonst *) 0;
    struct monst *mtmp;
    const char *material;
    int pm_index;

    if (context.bypasses) {
        /* this is approximate because the "no golems" !obj->nexthere
           check below doesn't understand bypassed objects; but it
           should suffice since bypassed objects always end up as a
           consecutive group at the top of their pile */
        while (obj && obj->bypass)
            obj = obj->nexthere;
    }

    /* no golems if you zap only one object -- not enough stuff */
    if (!obj || (!obj->nexthere && obj->quan == 1L))
        return;

    /* some of these choices are arbitrary */
    switch (okind) {
    case MAT_IRON:
    case MAT_STEEL:
    case MAT_METAL:
    case MAT_COPPER:
    case MAT_BRASS:
    case MAT_BRONZE:
    case MAT_PLATINUM:
    case MAT_ORICHALCUM:
    case MAT_ADAMANTIUM:
    case MAT_MITHRIL:
        pm_index = PM_IRON_GOLEM;
        material = "metal ";
        break;
    case MAT_SILVER:
        pm_index = PM_SILVER_GOLEM;
        material = "silver ";
        break;
    case MAT_MINERAL:
        pm_index = rn2(2) ? PM_STONE_GOLEM : PM_CLAY_GOLEM;
        material = "lithic ";
        break;
    case 0:
    case MAT_FLESH:
        /* there is no flesh type, but all food is type 0, so we use it */
        pm_index = PM_FLESH_GOLEM;
        material = "organic ";
        break;
    case MAT_WOOD:
        pm_index = PM_WOOD_GOLEM;
        material = "wood ";
        break;
    case MAT_LEATHER:
        pm_index = PM_LEATHER_GOLEM;
        material = "leather ";
        break;
    case MAT_CLOTH:
        pm_index = PM_ROPE_GOLEM;
        material = "cloth ";
        break;
    case MAT_TOOTH:
    case MAT_CHITIN:
    case MAT_BONE:
        pm_index = PM_BONE_GOLEM;
        material = "bony ";
        break;
    case MAT_PLANARRIFT:
        pm_index = PM_BLACK_LIGHT; /* nearest thing to "planar rift golem" */
        material = "void ";
        break;
    case MAT_GOLD:
        pm_index = PM_GOLD_GOLEM;
        material = "gold ";
        break;
    case MAT_GLASS:
        pm_index = PM_GLASS_GOLEM;
        material = "glassy ";
        break;
    case MAT_CRYSTAL:
        pm_index = PM_GLASS_GOLEM;
        material = "crystal ";
        break;
    case MAT_PAPER:
        pm_index = PM_PAPER_GOLEM;
        material = "paper ";
        break;
    case MAT_GEMSTONE:
        pm_index = PM_GEMSTONE_GOLEM;
        material = "gemstone ";
        break;
    case MAT_HARD_CRYSTAL:
        pm_index = PM_GEMSTONE_GOLEM;
        material = "crystal ";
        break;
    case MAT_MODRONITE:
        pm_index = PM_MODRON_PENTADRONE;
        material = "modronite ";
        break;
    default:
        /* if all else fails... */
        pm_index = PM_STRAW_GOLEM;
        material = "";
        break;
    }

    if (!(mvitals[pm_index].mvflags & MV_GENOCIDED))
        mdat = &mons[pm_index];

    mtmp = makemon(mdat, obj->ox, obj->oy, MM_PLAY_SUMMON_ANIMATION | MM_ANIMATE_DEAD_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
    polyuse(obj, okind, (int) mons[pm_index].cwt);

    if (mtmp && cansee(mtmp->mx, mtmp->my)) {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Some %sobjects meld, and %s arises from the pile!", material,
              a_monnam(mtmp));
    }
}

/* Assumes obj is on the floor. */
void
do_osshock(obj)
struct obj *obj;
{
    long i;

    if (obj->otyp == SCR_MAIL)
        return;

    obj_zapped = TRUE;

    if (poly_zapped < 0) {
        /* some may metamorphosize */
        for (i = obj->quan; i; i--)
            if (!rn2(Luck + 45)) {
                poly_zapped = obj->material;
                break;
            }
    }

    /* if quan > 1 then some will survive intact */
    if (obj->quan > 1L) {
        if (obj->quan > LARGEST_INT)
            obj = splitobj(obj, (long) rnd(30000));
        else
            obj = splitobj(obj, (long) rnd((int) obj->quan - 1));
    }

    /* appropriately add damage to bill */
    if (costly_spot(obj->ox, obj->oy)) {
        if (*u.ushops)
            addtobill(obj, FALSE, FALSE, FALSE);
        else
            (void) stolen_value(obj, obj->ox, obj->oy, FALSE, FALSE);
    }

    /* zap the object */
    delobj(obj);
}

/*
 * Polymorph the object to the given object ID.  If the ID is STRANGE_OBJECT
 * then pick random object from the source's class (this is the standard
 * "polymorph" case).  If ID is set to a specific object, inhibit fusing
 * n objects into 1.  This could have been added as a flag, but currently
 * it is tied to not being the standard polymorph case. The new polymorphed
 * object replaces obj in its link chains.  Return value is a pointer to
 * the new object.
 *
 * This should be safe to call for an object anywhere.
 */
struct obj *
poly_obj(obj, id)
struct obj *obj;
int id;
{
    struct obj *otmp;
    xchar ox = 0, oy = 0;
    long old_wornmask, new_wornmask = 0L;
    boolean can_merge = (id == STRANGE_OBJECT);
    int obj_location = obj->where;
    Strcpy(debug_buf_4, "poly_obj");

    if (obj->otyp == BOULDER)
        sokoban_guilt();
    if (id == STRANGE_OBJECT) 
    { /* preserve symbol */
        int try_limit = 3;
        unsigned magic_obj = objects[obj->otyp].oc_magic;

        if (obj->otyp == UNICORN_HORN && obj->degraded_horn)
            magic_obj = 0;

        /* Try up to 3 times to make the magic-or-not status of
           the new item be the same as it was for the old one. */
        otmp = (struct obj *) 0;
        do 
        {
            if (otmp)
                delobj(otmp);
            Strcpy(debug_buf_2, "polyobj");
            otmp = mkobj(obj->oclass, FALSE, FALSE);
        } while (--try_limit > 0
                 && objects[otmp->otyp].oc_magic != magic_obj);
    }
    else 
    {
        /* literally replace obj with this new thing */
        otmp = mksobj(id, FALSE, FALSE, FALSE);
/* Actually more things use corpsenm but they polymorph differently */
#define USES_CORPSENM(typ) \
    ((typ) == CORPSE || (typ) == STATUE || (typ) == FIGURINE)

        if (USES_CORPSENM(obj->otyp) && USES_CORPSENM(id))
            set_corpsenm(otmp, obj->corpsenm);
#undef USES_CORPSENM
    }

    /* preserve quantity */
    otmp->quan = obj->quan;
    /* preserve the shopkeepers (lack of) interest */
    otmp->no_charge = obj->no_charge;
    /* preserve inventory letter if in inventory */
    if (obj_location == OBJ_INVENT)
        otmp->invlet = obj->invlet;

    /* You can't send yourself 100 mail messages and then
     * polymorph them into useful scrolls
     */
    if (obj->otyp == SCR_MAIL) 
    {
        otmp->otyp = SCR_MAIL;
        otmp->special_quality = 1;
    }

    /* avoid abusing eggs laid by you */
    if (obj->otyp == EGG && (obj->speflags & SPEFLAGS_YOURS))
    {
        int mnum, tryct = 100;

        /* first, turn into a generic egg */
        if (otmp->otyp == EGG)
            kill_egg(otmp);
        else 
        {
            otmp->otyp = EGG;
            otmp->owt = weight(otmp);
        }
        otmp->corpsenm = NON_PM;
        otmp->speflags &= ~SPEFLAGS_YOURS;

        /* now change it into something laid by the hero */
        while (tryct--) 
        {
            mnum = can_be_hatched(random_monster(rn2));
            if (mnum != NON_PM && !dead_species(mnum, TRUE))
            {
                otmp->speflags |= SPEFLAGS_YOURS;            /* laid by hero */
                set_corpsenm(otmp, mnum); /* also sets hatch timer */
                break;
            }
        }
    }

    /* keep special fields (including charges on wands) */
    if (objects[otmp->otyp].oc_charged && objects[obj->otyp].oc_charged) //index(charged_objs, otmp->oclass))
    {
        otmp->charges = obj->charges;

        int maxcharges = get_obj_max_charge(otmp);
        if (otmp->charges > maxcharges)
            otmp->charges = maxcharges;
    }
    if (objects[otmp->otyp].oc_enchantable && objects[obj->otyp].oc_enchantable)
    {
        otmp->enchantment = obj->enchantment;

        int maxench = get_obj_max_enchantment(otmp);
        if (otmp->enchantment > maxench)
            otmp->enchantment = maxench;
    }
    otmp->recharged = obj->recharged;

    otmp->cursed = obj->cursed;
    otmp->blessed = obj->blessed;

    if (erosion_matters(otmp)) 
    {
        if (is_flammable(otmp) || is_rustprone(otmp))
            otmp->oeroded = obj->oeroded;

        if (is_corrodeable(otmp) || is_rottable(otmp))
            otmp->oeroded2 = obj->oeroded2;

        if (is_damageable(otmp))
            otmp->oerodeproof = obj->oerodeproof;
    }

    /* Keep chest/box traps and poisoned ammo if we may */
    if (obj->otrapped && Is_box(otmp))
        otmp->otrapped = TRUE;

    if (obj->opoisoned && is_poisonable(otmp))
        otmp->opoisoned = TRUE;

    if (obj->elemental_enchantment > 0)
        otmp->elemental_enchantment = 0; //Special enchantments do not get passed at the moment

    if (obj->exceptionality > 0)
        otmp->exceptionality = 0; //Special crafting does not get passed at the moment
    
    if (obj->mythic_prefix > 0)
        otmp->mythic_prefix = 0; //Mythic status does not get passed at the moment

    if (obj->mythic_suffix > 0)
        otmp->mythic_suffix = 0; //Mythic status does not get passed at the moment

    if (id == STRANGE_OBJECT && obj->otyp == CORPSE)
    {
        /* turn crocodile corpses into shoes */
        if (obj->corpsenm == PM_CROCODILE) 
        {
            otmp->otyp = LOW_BOOTS;
            otmp->material = objects[otmp->otyp].oc_material;
            otmp->oclass = ARMOR_CLASS;
            otmp->enchantment = 0;
            otmp->charges = 0;
            otmp->oeroded = 0;
            otmp->oerodeproof = TRUE;
            otmp->quan = 1L;
            otmp->cursed = FALSE;
        }
    }

    /* no box contents --KAA */
    if (Has_contents(otmp))
        delete_contents(otmp);

    /* 'n' merged objects may be fused into 1 object */
    if (otmp->quan > 1L && (!objects[otmp->otyp].oc_merge
                            || (can_merge && otmp->quan > (long) rn2(1000))))
        otmp->quan = 1L;

    switch (otmp->oclass) 
    {
    case TOOL_CLASS:
        if (otmp->otyp == MAGIC_LAMP) 
        {
            otmp->otyp = OIL_LAMP;
            otmp->age = 1500L; /* "best" oil lamp possible */
        }
        else if (otmp->otyp == MAGIC_MARKER) 
        {
            otmp->recharged = 1; /* degraded quality */
        }
        /* don't care about the recharge count of other tools */
        break;

    case WAND_CLASS:
        while (otmp->otyp == WAN_WISHING || otmp->otyp == WAN_POLYMORPH)
        {
            otmp->otyp = rnd_class(WAN_LIGHT, WAN_LIGHTNING);
            otmp->material = objects[otmp->otyp].oc_material;
        }
        /* altering the object tends to degrade its quality
           (analogous to spellbook `read count' handling) */
        if ((int) otmp->recharged < rn2(RECHARGE_LIMIT)) /* recharge_limit */
            otmp->recharged++;
        break;

    case POTION_CLASS:
        while (otmp->otyp == POT_POLYMORPH)
            otmp->otyp = rnd_class(POT_GAIN_ABILITY, POT_WATER);
        break;

    case SPBOOK_CLASS:
        while (otmp->otyp == SPE_POLYMORPH)
        {
            otmp->otyp = rnd_class(SPE_DIG, SPE_BLANK_PAPER);
            otmp->material = objects[otmp->otyp].oc_material;
        }
        /* reduce spellbook abuse; non-blank books degrade */
        if (otmp->otyp != SPE_BLANK_PAPER) 
        {
            otmp->spestudied = obj->spestudied + 1;
            if (otmp->spestudied > MAX_SPELL_STUDY) 
            {
                otmp->otyp = SPE_BLANK_PAPER;
                /* writing a new book over it will yield an unstudied
                   one; re-polymorphing this one as-is may or may not
                   get something non-blank */
                otmp->spestudied = rn2(otmp->spestudied);
            }
        }
        break;

    case GEM_CLASS:
        if (otmp->quan > (long) rnd(4)
            && obj->material == MAT_MINERAL
            && otmp->material != MAT_MINERAL)
        {
            otmp->otyp = ROCK; /* transmutation backfired */
            otmp->material = objects[otmp->otyp].oc_material;
            otmp->quan /= 2L;  /* some material has been lost */
        }
        break;
    }

    /* update the weight */
    otmp->owt = weight(otmp);

    /*
     * ** we are now done adjusting the object (except possibly wearing it) **
     */

    (void) get_obj_location(obj, &ox, &oy, BURIED_TOO | CONTAINED_TOO);
    old_wornmask = obj->owornmask & ~(W_ARTIFACT_CARRIED | W_ARTIFACT_INVOKED);
    /* swap otmp for obj */
    replace_object(obj, otmp);
    if (obj_location == OBJ_INVENT) 
    {
        /*
         * We may need to do extra adjustments for the hero if we're
         * messing with the hero's inventory.  The following calls are
         * equivalent to calling freeinv on obj and addinv on otmp,
         * while doing an in-place swap of the actual objects.
         */
        freeinv_core(obj);
        addinv_core1(otmp);
        addinv_core2(otmp);
        /*
         * Handle polymorph of worn item.  Stone-to-flesh cast on self can
         * affect multiple objects at once, but their new forms won't
         * produce any side-effects.  A single worn item dipped into potion
         * of polymorph can produce side-effects but those won't yield out
         * of sequence messages because current polymorph is finished.
         */
        if (old_wornmask) 
        {
            boolean was_twohanded = bimanual(obj);

            /* wearslot() returns a mask which might have multiple bits set;
               narrow that down to the bit(s) currently in use */
            new_wornmask = wearslot(otmp) & old_wornmask;
            remove_worn_item(obj, TRUE);
            /* if the new form can be worn in the same slot, make it so */
            if ((new_wornmask & W_WEP) != 0L)
            {
                if (was_twohanded || !bimanual(otmp) || !uarms)
                    setuwep(otmp, W_WEP);
            }
            else if ((new_wornmask & W_WEP2) != 0L)
            {
                if (was_twohanded || !bimanual(otmp) || !uwep)
                    setuwep(otmp, W_WEP2);
            }
            else if ((new_wornmask & W_SWAPWEP) != 0L)
            {
                if (was_twohanded || !bimanual(otmp) || !uswapwep2)
                    setuswapwep(otmp, W_SWAPWEP);
            }
            else if ((new_wornmask & W_SWAPWEP2) != 0L)
            {
                if (was_twohanded || !bimanual(otmp) || !uswapwep)
                    setuswapwep(otmp, W_SWAPWEP2);
            }
            else if ((new_wornmask & W_QUIVER) != 0L) 
            {
                setuqwep(otmp);
            } else if (new_wornmask)
            {
                setworn(otmp, new_wornmask);
                /* set_wear() might result in otmp being destroyed if
                   worn amulet has been turned into an amulet of change */
                set_wear(otmp);
                otmp = wearmask_to_obj(new_wornmask); /* might be Null */
            }
        } /* old_wornmask */
    } 
    else if (obj_location == OBJ_FLOOR) 
    {
        if (obj->otyp == BOULDER && otmp->otyp != BOULDER
            && !does_block(ox, oy, &levl[ox][oy]))
            unblock_vision_and_hearing_at_point(ox, oy);
        else if (obj->otyp != BOULDER && otmp->otyp == BOULDER)
            /* (checking does_block() here would be redundant) */
            block_vision_and_hearing_at_point(ox, oy);
    }

    /* note: if otmp is gone, billing for it was handled by useup() */
    if (((otmp && !carried(otmp)) || obj->unpaid) && costly_spot(ox, oy)) 
    {
        struct monst *shkp = shop_keeper(*in_rooms(ox, oy, SHOPBASE));

        if ((!obj->no_charge
             || (Has_contents(obj)
                 && (contained_cost(obj, shkp, 0L, FALSE, FALSE) != 0L)))
            && inhishop(shkp))
        {
            if (is_peaceful(shkp)) 
            {
                if (*u.ushops
                    && (*in_rooms(u.ux, u.uy, 0)
                        == *in_rooms(shkp->mx, shkp->my, 0))
                    && !costly_spot(u.ux, u.uy))
                {
                    make_angry_shk(shkp, ox, oy);
                } 
                else 
                {
                    play_simple_monster_sound(shkp, MONSTER_SOUND_TYPE_GET_ANGRY);
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s gets angry!", Monnam(shkp));
                    hot_pursuit(shkp);
                }
            } 
            else
                Norep_ex(ATR_NONE, CLR_MSG_WARNING, "%s is furious!", Monnam(shkp));
        }
    }
    delobj(obj);
    return otmp;
}

/* stone-to-flesh spell hits and maybe transforms or animates obj */
int
stone_to_flesh_obj(obj)
struct obj *obj;
{
    int res = 1; /* affected object by default */
    struct permonst *ptr;
    struct monst *mon, *shkp;
    struct obj *item;
    xchar oox, ooy;
    boolean smell = FALSE, golem_xform = FALSE;

    if (obj->material != MAT_MINERAL && obj->material != MAT_GEMSTONE)
        return 0;
    /* Heart of Ahriman usually resists; ordinary items rarely do */
    if (obj_resists(obj, 2, 98))
        return 0;

    (void) get_obj_location(obj, &oox, &ooy, 0);
    /* add more if stone objects are added.. */
    switch (objects[obj->otyp].oc_class) {
    case ROCK_CLASS: /* boulders and statues */
    case TOOL_CLASS: /* figurines */
        if (obj->otyp == BOULDER) {
            obj = poly_obj(obj, HUGE_CHUNK_OF_MEAT);
            smell = TRUE;
        } else if (obj->otyp == STATUE || obj->otyp == FIGURINE) {
            ptr = &mons[obj->corpsenm];
            if (is_golem(ptr)) {
                golem_xform = (ptr != &mons[PM_FLESH_GOLEM]);
            } else if (vegetarian(ptr)) {
                /* Don't animate monsters that aren't flesh */
                obj = poly_obj(obj, MEATBALL);
                smell = TRUE;
                break;
            }
            if (obj->otyp == STATUE) {
                /* animate_statue() forces all golems to become flesh golems */
                mon = animate_statue(obj, oox, ooy, ANIMATE_SPELL, (int *) 0);
            } else { /* (obj->otyp == FIGURINE) */
                if (golem_xform)
                    ptr = &mons[PM_FLESH_GOLEM];
                mon = makemon(ptr, oox, ooy, MM_NO_MONSTER_INVENTORY);
                if (mon) {
                    if (costly_spot(oox, ooy)
                        && (carried(obj) ? obj->unpaid : !obj->no_charge)) {
                        shkp = shop_keeper(*in_rooms(oox, ooy, SHOPBASE));
                        stolen_value(obj, oox, ooy,
                                     (shkp && is_peaceful(shkp)), FALSE);
                    }
                    if (obj->timed)
                        obj_stop_timers(obj);
                    if (carried(obj))
                        useup(obj);
                    else
                        delobj(obj);
                    if (cansee(mon->mx, mon->my))
                        pline_The("figurine %sanimates!",
                                  golem_xform ? "turns to flesh and " : "");
                }
            }
            if (mon) {
                ptr = mon->data;
                /* this golem handling is redundant... */
                if (is_golem(ptr) && ptr != &mons[PM_FLESH_GOLEM])
                    (void) newcham(mon, &mons[PM_FLESH_GOLEM], 0, TRUE, FALSE);
            } else if ((ptr->geno & (G_NOCORPSE | G_UNIQ)) != 0) {
                /* didn't revive but can't leave corpse either */
                res = 0;
            } else {
                /* unlikely to get here since genociding monsters also
                   sets the G_NOCORPSE flag; drop statue's contents */
                while ((item = obj->cobj) != 0) {
                    bypass_obj(item); /* make stone-to-flesh miss it */
                    obj_extract_self(item);
                    place_object(item, oox, ooy);
                }
                obj = poly_obj(obj, CORPSE);
            }
        } else { /* miscellaneous tool or unexpected rock... */
            res = 0;
        }
        break;
    /* maybe add weird things to become? */
    case RING_CLASS: /* some of the rings are stone */
        obj = poly_obj(obj, MEAT_RING);
        smell = TRUE;
        break;
    case WAND_CLASS: /* marble wand */
        obj = poly_obj(obj, MEAT_STICK);
        smell = TRUE;
        break;
    case GEM_CLASS: /* stones & gems */
        obj = poly_obj(obj, MEATBALL);
        smell = TRUE;
        break;
    case WEAPON_CLASS: /* crysknife */
        /*FALLTHRU*/
    default:
        res = 0;
        break;
    }

    if (smell) {
        /* non-meat eaters smell meat, meat eaters smell its flavor;
           monks are considered non-meat eaters regardless of behavior;
           other roles are non-meat eaters if they haven't broken
           vegetarian conduct yet (or if poly'd into non-carnivorous/
           non-omnivorous form, regardless of whether it's herbivorous,
           non-eating, or something stranger) */
        if (Role_if(PM_MONK) || !u.uconduct.unvegetarian
            || !carnivorous(youmonst.data))
            Norep("You smell the odor of meat.");
        else
            Norep("You smell a delicious smell.");
    }
    newsym(oox, ooy);
    return res;
}

/*
 * Object obj was hit by the effect of the wand/spell otmp.  Return
 * non-zero if the wand/spell had any effect.
 */
int
bhito(obj, otmp, origmonst)
struct obj *obj, *otmp;
struct monst* origmonst;
{
    int res = 0; /* affected object by default */
    boolean learn_it = FALSE, maybelearnit;

    if (!origmonst)
    {
        /* Do nothing, this is ok */
    }

    /* fundamental: a wand effect hitting itself doesn't do anything;
       otherwise we need to guard against accessing otmp after something
       strange has happened to it (along the lines of polymorph or
       stone-to-flesh [which aren't good examples since polymorph wands
       aren't affected by polymorph zaps and stone-to-flesh isn't
       available in wand form, but the concept still applies...]) */
    if (obj == otmp)
        return 0;

    if (obj->bypass) 
    {
        /* The bypass bit is currently only used as follows:
         *
         * POLYMORPH - When a monster being polymorphed drops something
         *             from its inventory as a result of the change.
         *             If the items fall to the floor, they are not
         *             subject to direct subsequent polymorphing
         *             themselves on that same zap.  This makes it
         *             consistent with items that remain in the monster's
         *             inventory.  They are not polymorphed either.
         * UNDEAD_TURNING - When an undead creature gets killed via
         *             undead turning, prevent its corpse from being
         *             immediately revived by the same effect.
         * STONE_TO_FLESH - If a statue can't be revived, its
         *             contents get dropped before turning it into
         *             meat; prevent those contents from being hit.
         * retouch_equipment() - bypass flag is used to track which
         *             items have been handled (bhito isn't involved).
         * menu_drop(), askchain() - inventory traversal where multiple
         *             Drop can alter the invent chain while traversal
         *             is in progress (bhito isn't involved).
         * destroy_item(), destroy_mitem() - inventory traversal where
         *             item destruction can trigger drop or destruction of
         *             other item(s) and alter the invent or mon->minvent
         *             chain, possibly recursively.
         *
         * The bypass bit on all objects is reset each turn, whenever
         * context.bypasses is set.
         *
         * We check the obj->bypass bit above AND context.bypasses
         * as a safeguard against any stray occurrence left in an obj
         * struct someplace, although that should never happen.
         */
        if (context.bypasses)
        {
            return 0;
        } 
        else 
        {
            debugpline1("%s for a moment.", Tobjnam(obj, "pulsate"));
            obj->bypass = 0;
        }
    }

    /*
     * Some parts of this function expect the object to be on the floor
     * obj->{ox,oy} to be valid.  The exception to this (so far) is
     * for the STONE_TO_FLESH spell.
     */
    if (!(obj->where == OBJ_FLOOR || otmp->otyp == SPE_STONE_TO_FLESH))
        impossible("bhito: obj is not floor or Stone To Flesh spell");

    if (obj == uball) 
    {
        res = 0;
    } 
    else if (obj == uchain)
    {
        if (otmp->otyp == WAN_OPENING || otmp->otyp == SPE_KNOCK)
        {
            learn_it = TRUE;
            unpunish();
        } else
            res = 0;
    } 
    else
        switch (otmp->otyp) 
        {
        case WAN_POLYMORPH:
        case SPE_POLYMORPH:
            res = 1;
            if (obj->otyp == WAN_POLYMORPH || obj->otyp == SPE_POLYMORPH
                || obj->otyp == POT_POLYMORPH || obj_resists(obj, 5, 95)) 
            {
                res = 0;
                break;
            }
            /* KMH, conduct */
            if ((otmp->otyp != WAN_POLYMORPH || otmp->where == OBJ_INVENT) && (!u.uconduct.polypiles++))
                livelog_printf(LL_CONDUCT, "polymorphed %s first object", uhis());

            /* any saved lock context will be dangerously obsolete */
            if (Is_box(obj))
                (void) boxlock(obj, otmp);

            if (obj_shudders(obj))
            {
                boolean cover =
                    ((obj == level.objects[u.ux][u.uy]) && u.uundetected
                     && hides_under(youmonst.data));

                if (cansee(obj->ox, obj->oy))
                    learn_it = TRUE;
                do_osshock(obj);
                /* eek - your cover might have been blown */
                if (cover)
                    (void) hideunder(&youmonst);
                break;
            }
            obj = poly_obj(obj, STRANGE_OBJECT);
            newsym(obj->ox, obj->oy);
            break;
        case SPE_PROBE:
        case WAN_PROBING:
            res = probe_object(obj);
            if (res)
                learn_it = TRUE;
            break;
        case WAN_STRIKING:
        case SPE_FORCE_BOLT:
        case SPE_FORCE_STRIKE:
            /* learn the type if you see or hear something break
               (the sound could be implicit) */
            maybelearnit = cansee(obj->ox, obj->oy) || !Deaf;
            if (obj->otyp == BOULDER) 
            {
                res = 1;
                if (cansee(obj->ox, obj->oy))
                    pline_The("boulder falls apart.");
                else
                    You_hear("a crumbling sound.");
                fracture_rock(obj, TRUE);
            } 
            else if (obj->otyp == STATUE) 
            {
                res = 1;
                if (break_statue(obj)) 
                {
                    if (cansee(obj->ox, obj->oy))
                    {
                        if (Hallucination)
                            pline_The("%s shatters.", rndmonnam(NULL));
                        else
                            pline_The("statue shatters.");
                    } 
                    else
                        You_hear("a crumbling sound.");
                }
            } 
            else
            {
                int oox = obj->ox;
                int ooy = obj->oy;
                if (context.mon_moving
                        ? !breaks(obj, obj->ox, obj->oy)
                        : !hero_breaks(obj, obj->ox, obj->oy, FALSE))
                    maybelearnit = FALSE; /* nothing broke */
                else
                {
                    newsym_force(oox, ooy);
                }
                res = 0;
            }
            if (maybelearnit)
                learn_it = TRUE;
            break;
        case WAN_CANCELLATION:
        case SPE_CANCELLATION:
        case WAN_DISJUNCTION:
        case SPE_DISJUNCTION:
            res = 1;
            cancel_item(obj, TRUE);
#ifdef TEXTCOLOR
            newsym(obj->ox, obj->oy); /* might change color */
#endif
            break;
        case SPE_DRAIN_LEVEL:
            res = 1;
            (void) drain_item(obj, TRUE);
            break;
        case WAN_TELEPORTATION:
        case SPE_TELEPORT_MONSTER:
            res = 1;
            if(!level.flags.noteleport)
                (void) rloco(obj);
            break;
        case WAN_MAKE_INVISIBLE:
            break;
        case SPE_LOWER_MAGIC_RESISTANCE:
        case SPE_DIMINISH_MAGIC_RESISTANCE:
        case SPE_ABOLISH_MAGIC_RESISTANCE:
        case SPE_NEGATE_MAGIC_RESISTANCE:
        case SPE_FORBID_SUMMONING:
        case WAN_UNDEAD_TURNING:
        case SPE_TURN_UNDEAD:
        case SPE_FEAR:
        case SPE_BANISH_DEMON:
        case SPE_NEGATE_UNDEATH:
        case SPE_POWER_WORD_KILL:
        case SPE_POWER_WORD_STUN:
        case SPE_POWER_WORD_BLIND:
        case SPE_SUNLIGHT_BEAM:
        case SPE_RAY_OF_RADIANCE:
            break;
        case WAN_RESURRECTION:
        case SPE_RESURRECTION:
            if (obj->otyp == EGG) 
            {
                res = 1;
                revive_egg(obj);
            }
            else if (obj->otyp == CORPSE) 
            {
                res = 1;
                struct monst* mtmp;
                xchar ox, oy;
                int corpsenm = corpse_revive_type(obj);
                char* corpsname = cxname_singular(obj);

                /* get corpse's location before revive() uses it up */
                if (!get_obj_location(obj, &ox, &oy, 0))
                    ox = obj->ox, oy = obj->oy; /* won't happen */

                mtmp = revive(obj, TRUE, -1, TRUE);

                if (!mtmp) 
                {
                    res = 0; /* no monster implies corpse was left intact */
                }
                else
                {
                    int newmnum = mtmp->mnum;

                    if (cansee(ox, oy))
                    {
                        if (canspotmon(mtmp)) 
                        {
                            boolean newcorpsetype = (corpsenm != newmnum && corpsenm > NON_PM);
                            char cbuf[BUFSZ] = "";
                            if (newcorpsetype && corpsname && strcmp(corpsname, ""))
                                Sprintf(cbuf, " from %s", the(corpsname));

                            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s is resurrected%s!",
                                upstart(noname_monnam(mtmp, newcorpsetype ? ARTICLE_A : ARTICLE_THE)),
                                cbuf);

                            learn_it = TRUE;
                        }
                        else 
                        {
                            /* saw corpse but don't see monster: maybe
                               mtmp is invisible, or has been placed at
                               a different spot than <ox,oy> */
                            if (!is_mname_proper_name(&mons[corpsenm]))
                                corpsname = The(corpsname);
                            pline("%s disappears.", corpsname);
                        }
                    }
                    else 
                    {
                        /* couldn't see corpse's location */
                        if (Role_if(PM_HEALER) && !Deaf
                            && !is_not_living(&mons[corpsenm]))
                        {
                            if (!is_mname_proper_name(&mons[corpsenm]))
                                corpsname = an(corpsname);
                            if (!Hallucination)
                                You_hear("%s reviving.", corpsname);
                            else
                                You_hear("a defibrillator.");
                            learn_it = TRUE;
                        }
                        if (canspotmon(mtmp))
                            /* didn't see corpse but do see monster: it
                               has been placed somewhere other than <ox,oy>
                               or blind hero spots it with ESP */
                            pline("%s appears.", Monnam(mtmp));
                    }
                    if (learn_it)
                        exercise(A_WIS, TRUE);
                }
            }
            break;
        case SPE_RAISE_MINOR_ZOMBIE:
            if (obj->otyp == CORPSE && obj->corpsenm > NON_PM)
            {
                int zombietype = mon_to_zombie(obj->corpsenm);
                if (zombietype > NON_PM && zombietype != PM_GIANT_ZOMBIE && zombietype != PM_ETTIN_ZOMBIE && animate_corpse(obj, zombietype))
                {
                    /* success */
                    res = 1;
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s twitches for a moment, but nothing else happens.", The(cxname(obj)));
                }
            }
            break;
        case SPE_RAISE_GIANT_ZOMBIE:
            if (obj->otyp == CORPSE && obj->corpsenm > NON_PM)
            {
                int zombietype = mon_to_zombie(obj->corpsenm);

                if (zombietype > NON_PM && animate_corpse(obj, zombietype)) /* Raise giant zombie alternatively works also for smaller zombies as an added benefit */
                {
                    /* success */
                    res = 1;
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s twitches for a moment, but nothing else happens.", The(cxname(obj)));
                }
            }
            break;
        case SPE_CREATE_MINOR_MUMMY:
            if (obj->otyp == CORPSE && obj->corpsenm >= LOW_PM)
            {
                int zombietype = mon_to_mummy(obj->corpsenm);
                
                if (zombietype > NON_PM && zombietype != PM_ETTIN_MUMMY && zombietype != PM_GIANT_MUMMY && animate_corpse(obj, zombietype))
                {
                    /* success */
                    res = 1;
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s twitches for a moment, but nothing else happens.", The(cxname(obj)));
                }
            }
            break;
        case SPE_CREATE_GIANT_MUMMY:
            if (obj->otyp == CORPSE && obj->corpsenm >= LOW_PM)
            {
                int zombietype = mon_to_mummy(obj->corpsenm);

                if (zombietype > NON_PM && animate_corpse(obj, zombietype))
                { /* Works for smaller mummies as an added benefit */
                    /* success */
                    res = 1;
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s twitches for a moment, but nothing else happens.", The(cxname(obj)));
                }
            }
            break;
        case SPE_CREATE_DRACOLICH:
            if (obj->otyp == CORPSE && obj->corpsenm >= LOW_PM)
            {
                int zombietype = NON_PM;
                if ((obj->corpsenm >= PM_GRAY_DRAGON && obj->corpsenm <= PM_ANCIENT_YELLOW_DRAGON)
                    || obj->corpsenm == PM_GOLD_DRAGON ||  obj->corpsenm == PM_ANCIENT_GOLD_DRAGON
                    || obj->corpsenm == PM_BAHAMUT || obj->corpsenm == PM_TIAMAT
                    )
                {
                    if ((mons[obj->corpsenm].geno & G_UNIQ))
                        zombietype = PM_ELDER_DRACOLICH;
                    else
                        zombietype = PM_DRACOLICH;
                }

                if (zombietype > NON_PM && animate_corpse(obj, zombietype))
                {
                    res = 1;
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s twitches for a moment, but nothing else happens.", The(cxname(obj)));
                }
            }
            break;
        case SPE_CREATE_ELDER_DRACOLICH:
            if (obj->otyp == CORPSE && obj->corpsenm >= LOW_PM)
            {
                int zombietype = NON_PM;
                if ((obj->corpsenm >= PM_ANCIENT_GRAY_DRAGON && obj->corpsenm <= PM_ANCIENT_YELLOW_DRAGON)
                    || obj->corpsenm == PM_ANCIENT_GOLD_DRAGON
                    || obj->corpsenm == PM_BAHAMUT || obj->corpsenm == PM_TIAMAT
                    )
                {
                    zombietype = PM_ELDER_DRACOLICH;
                }

                if (zombietype > NON_PM && animate_corpse(obj, zombietype))
                {
                    res = 1;
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s twitches for a moment, but nothing else happens.", The(cxname(obj)));
                }
            }
            break;
        case WAN_OPENING:
        case SPE_KNOCK:
        case WAN_LOCKING:
        case SPE_WIZARD_LOCK:
            if (Is_box(obj))
                res = boxlock(obj, otmp);
            else
                res = 0;
            if (res)
                learn_it = TRUE;
            break;
        case WAN_SLOW_MONSTER: /* no effect on objects */
        case SPE_SLOW_MONSTER:
        case SPE_MASS_SLOW:
        case SPE_HASTE_MONSTER:
        case SPE_HOLD_MONSTER:
        case SPE_MASS_HOLD:
        case SPE_HOLD_UNDEAD:
        case WAN_SPEED_MONSTER:
        case SPE_SILENCE:
        case SPE_MAGIC_ARROW:
        case SPE_ARROW_OF_DIANA:
        case WAN_NOTHING:
        case SPE_CHARM_MONSTER:
        case SPE_CONTROL_UNDEAD:
        case SPE_CURE_BLINDNESS:
        case SPE_CURE_SICKNESS:
        case SPE_CURE_PETRIFICATION:
        case SPE_MINOR_HEALING:
        case JAR_OF_MEDICINAL_SALVE:
        case JAR_OF_EXTRA_HEALING_SALVE:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case SPE_HEALING:
        case SPE_EXTRA_HEALING:
        case SPE_GREATER_HEALING:
        case SPE_PRODIGIOUS_HEALING:
        case SPE_FULL_HEALING:
        case SPE_REPLENISH_UNDEATH:
        case SPE_GREATER_UNDEATH_REPLENISHMENT:
        case SPE_SHOCKING_TOUCH:
        case SPE_BURNING_HANDS:
        case SPE_FREEZING_TOUCH:
        case SPE_INCINERATE:
        case SPE_ELECTROCUTE:
        case SPE_GLACIAL_GRASP:
        case SPE_HEAVENLY_TOUCH:
        case SPE_TOUCH_OF_DIVINITY:
        case SPE_TOUCH_OF_DEATH:
        case SPE_TOUCH_OF_PETRIFICATION:
        case SPE_FLESH_TO_STONE:
        case SPE_GAZE_OF_PETRIFICATION:
            res = 0;
            break;
        case JAR_OF_BASILISK_BLOOD:
        case SPE_STONE_TO_FLESH:
            learn_it = TRUE;
            res = stone_to_flesh_obj(obj);
            break;
        default:
            //impossible("What an interesting effect (%d)", otmp->otyp);
            break;
        }
    /* if effect was observable then discover the wand type provided
       that the wand itself has been seen */
    if (learn_it)
        learnwand(otmp);
    return res;
}

/* returns nonzero if something was hit */
int
bhitpile(obj, origmonst, fhito, tx, ty, zz, hit_only_one, stop_at_first_hit_object)
struct obj *obj;
struct monst* origmonst;
int FDECL((*fhito), (OBJ_P, OBJ_P, MONST_P));
int tx, ty;
schar zz;
uchar hit_only_one;
boolean stop_at_first_hit_object;
{
    int hitanything = 0;
    register struct obj *otmp, *next_obj;
    int bucstatus = !obj || obj->cursed ? -1 : obj->blessed ? 1 : 0;
    int bhitlimit = hit_only_one == 1 ? 1 : 
        hit_only_one == 2 ? (bucstatus == -1 ? 1 : bucstatus == 0 ? 2 : 3) : 
        hit_only_one == 3 ? (bucstatus == -1 ? 1 : bucstatus == 0 ? 4 : 7) :
        hit_only_one == 4 ? (bucstatus == -1 ? 2 : bucstatus == 0 ? 4 : 6) :
        0;

    if (obj->otyp == SPE_FORCE_BOLT || obj->otyp == SPE_FORCE_STRIKE || obj->otyp == WAN_STRIKING)
    {
        struct trap *t = t_at(tx, ty);

        /* We can't settle for the default calling sequence of
           bhito(otmp) -> break_statue(otmp) -> activate_statue_trap(ox,oy)
           because that last call might end up operating on our `next_obj'
           (below), rather than on the current object, if it happens to
           encounter a statue which mustn't become animated. */
        if (t && t->ttyp == STATUE_TRAP
            && activate_statue_trap(t, tx, ty, TRUE))
            learnwand(obj);
    }

    poly_zapped = -1;
    for (otmp = level.objects[tx][ty]; otmp; otmp = next_obj) 
    {
        context.bhitcount++;
        if (hit_only_one && hitanything > bhitlimit)
            break;

        next_obj = otmp->nexthere;
        /* for zap downwards, don't hit object poly'd hero is hiding under */
        if (zz > 0 && u.uundetected && otmp == level.objects[u.ux][u.uy]
            && hides_under(youmonst.data))
            continue;

        hitanything += (*fhito)(otmp, obj, origmonst);

        if (hitanything && stop_at_first_hit_object)
            break;
    }
    if (poly_zapped >= 0)
        create_polymon(level.objects[tx][ty], poly_zapped);

    return hitanything;
}

/*
 * zappable - returns 1 if zap is available, 0 otherwise.
 *            it removes a charge from the wand if zappable.
 * added by GAN 11/03/86
 */
int
zappable(wand)
register struct obj *wand;
{
    if (wand->charges < 0 || (wand->charges == 0 && rn2(121)))
        return 0;
    if (wand->charges == 0)
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "wrest one last charge from the worn-out wand.");
    wand->charges--;
    return 1;
}

/*
 * zapnodir - zaps a NODIR wand/spell.
 * added by GAN 11/03/86
 */
void
zapnodir(obj)
register struct obj *obj;
{
    boolean known = FALSE;
    int monstid = 0;
    int archoncount = 0;
    int angelcount = 0;
    int aleaxcount = 0;
    char buf[BUFSZ];
    char aleaxbuf[BUFSZ];
    char angelbuf[BUFSZ];
    char archonbuf[BUFSZ];
    struct monst* mtmp = (struct monst*)0;
    struct obj* otmp = (struct obj*)0;
    int duration = get_obj_spell_duration(obj);

    switch (obj->otyp)
    {
    case WAN_LIGHT:
    case SPE_LIGHT:
        litroom(TRUE, obj);
        if (!Blind)
            known = TRUE;
        if (lightdamage(obj, TRUE, 5) > 0)
            known = TRUE;
        break;
    case SPE_BLACK_BLADE_OF_DISASTER:
        known = TRUE;
        You_ex(ATR_NONE, CLR_MSG_SPELL, "chant an invocation:");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Sword of Cold and Darkness, free yourself from the heaven's bonds.");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Become one with my power, one with my body,");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "And let us walk the path of destruction together!");
        play_sfx_sound(SFX_ITEM_APPEARS);
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "A sword-shaped planar rift forms before you!");
        summonblackblade(obj);
        break;
    case SPE_MAGE_ARMOR:
        known = TRUE;
        play_sfx_sound(SFX_ITEM_APPEARS);
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "An armor-shaped force field forms before you!");
        summonmagearmor(obj);
        break;
    case SPE_CREATE_FOOD:
    {
        known = TRUE;
        otmp = mksobj(FOOD_RATION, FALSE, FALSE, FALSE);
        if (otmp)
        {
            play_sfx_sound(SFX_ITEM_APPEARS);
            (void)hold_another_object(otmp, "You drop %s!",
                doname(otmp), "A delicious food ration forms before you!");
        }
        break;
    }
    case SPE_CREATE_FRUITS:
    {
        known = TRUE;
        int roll = rnd(7);
        int fruittype = APPLE;
        int fruitnum = 6;

        switch (roll)
        {
        case 1:
            fruittype = CLOUDBERRY;
            fruitnum = rnd(2);
            break;
        case 2:
            fruittype = DRAGON_FRUIT;
            fruitnum = rnd(2);
            break;
        case 3:
            fruittype = FIG;
            fruitnum = rnd(2);
            break;
        case 4:
            fruittype = APPLE;
            fruitnum = rnd(3) + 4;
            break;
        case 5:
            fruittype = PEAR;
            fruitnum = rnd(3) + 4;
            break;
        case 6:
            fruittype = ORANGE;
            fruitnum = rnd(3) + 4;
            break;
        case 7:
            fruittype = MELON;
            fruitnum = rnd(2);
            break;
        default:
            break;
        }

        otmp = mksobj(fruittype, FALSE, FALSE, FALSE);
        if (otmp)
        {
            play_sfx_sound(SFX_ITEM_APPEARS);
            otmp->quan = fruitnum;
            otmp->owt = weight(otmp);
            (void)hold_another_object(otmp, "You drop %s!",
                doname(otmp), fruitnum == 1 ? "A delicious fruit appears out of nowhere!" : "Delicious fruits appear out of nowhere!");
        }
        break;
    }
    case SPE_CREATE_WATER:
    {
        known = TRUE;
        otmp = mksobj(POT_WATER, FALSE, FALSE, FALSE);
        if (otmp)
        {
            play_sfx_sound(SFX_ITEM_APPEARS);
            (void)hold_another_object(otmp, "You drop %s!",
                doname(otmp), "A potion appears out of thin air!");
        }
        break;
    }
    case SPE_ARMAGEDDON:
        known = TRUE;
        You_ex(ATR_NONE, CLR_MSG_SPELL, "chant an invocation:");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Vas Kal An Mani...");
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "The air begins to take an odd dull color.");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "...In Corp Hur Tym!");
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "Suddenly an eerie silence fills the air!");
        armageddon();
        break;
    case SPE_WISH:
        known = TRUE;
        makewish(FALSE, TRUE);
        break;
    case SPE_TIME_STOP:
        known = TRUE;
        timestop(duration);
        break;
    case SPE_ANIMATE_AIR:
        known = TRUE;
        if (Is_waterlevel(&u.uz))
            pline("Unfortunately, nothing happens.");
        else
            (void)summoncreature(obj->otyp, PM_AIR_ELEMENTAL, "The air around you starts to swirl and forms into %s.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);

        break;
    case SPE_ANIMATE_EARTH:
        known = TRUE;
        if (Is_waterlevel(&u.uz) || Is_airlevel(&u.uz))
            pline("Unfortunately, nothing happens.");
        else
            (void)summoncreature(obj->otyp, PM_EARTH_ELEMENTAL, "The earth near you starts to swirl and forms into %s.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_ANIMATE_FIRE:
        known = TRUE;
        if (Is_waterlevel(&u.uz))
            pline("Unfortunately, nothing happens.");
        else
            (void)summoncreature(obj->otyp, PM_FIRE_ELEMENTAL, "A flickering flame appears out of thin air and forms into %s.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH); 
        break;
    case SPE_ANIMATE_WATER:
        known = TRUE;
        if (Is_firelevel(&u.uz))
            pline("Unfortunately, nothing happens.");
        else
            (void)summoncreature(obj->otyp, PM_WATER_ELEMENTAL, "Water condensates from thin air and forms into %s.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);

        break;
    case SPE_GREATER_ANIMATE_AIR:
        known = TRUE;
        if (Is_waterlevel(&u.uz))
            pline("Unfortunately, nothing happens.");
        else
            (void)summoncreature(obj->otyp, PM_ELDER_AIR_ELEMENTAL, "The air around you starts to swirl and forms into %s.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_GREATER_ANIMATE_EARTH:
        known = TRUE;
        if (Is_waterlevel(&u.uz) || Is_airlevel(&u.uz))
            pline("Unfortunately, nothing happens.");
        else
            (void)summoncreature(obj->otyp, PM_ELDER_EARTH_ELEMENTAL, "The earth near you starts to swirl and forms into %s.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_GREATER_ANIMATE_FIRE:
        known = TRUE;
        if (Is_waterlevel(&u.uz))
            pline("Unfortunately, nothing happens.");
        else
            (void)summoncreature(obj->otyp, PM_ELDER_FIRE_ELEMENTAL, "A flickering flame appears out of thin air and forms into %s.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_GREATER_ANIMATE_WATER:
        known = TRUE;
        if (Is_firelevel(&u.uz))
            pline("Unfortunately, nothing happens.");
        else
            (void)summoncreature(obj->otyp, PM_ELDER_WATER_ELEMENTAL, "Water condensates from thin air and forms into %s.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_SUMMON_DRAGON:
        mtmp = summoncreature(obj->otyp, PM_GRAY_DRAGON + rn2(PM_YELLOW_DRAGON - PM_GRAY_DRAGON + 1), "%s appears in a puff of smoke.", MM_SUMMON_IN_SMOKE_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_SUMMON_ANCIENT_DRAGON:
        mtmp = summoncreature(obj->otyp, PM_ANCIENT_GRAY_DRAGON + rn2(PM_ANCIENT_YELLOW_DRAGON - PM_ANCIENT_GRAY_DRAGON + 1), "%s appears in a puff of smoke.", MM_SUMMON_IN_SMOKE_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_CREATE_GOLD_GOLEM:
    case SPE_CREATE_GLASS_GOLEM:
    case SPE_CREATE_GEMSTONE_GOLEM:
    case SPE_CREATE_SILVER_GOLEM:
    case SPE_CREATE_CLAY_GOLEM:
    case SPE_CREATE_STONE_GOLEM:
    case SPE_CREATE_IRON_GOLEM:
    case SPE_CREATE_WOOD_GOLEM:
    case SPE_CREATE_PAPER_GOLEM:
        switch (obj->otyp)
        {
        case SPE_CREATE_GOLD_GOLEM:
            monstid = PM_GOLD_GOLEM;
            break;
        case SPE_CREATE_GLASS_GOLEM:
            monstid = PM_GLASS_GOLEM;
            break;
        case SPE_CREATE_GEMSTONE_GOLEM:
            monstid = PM_GEMSTONE_GOLEM;
            break;
        case SPE_CREATE_SILVER_GOLEM:
            monstid = PM_SILVER_GOLEM;
            break;
        case SPE_CREATE_CLAY_GOLEM:
            monstid = PM_CLAY_GOLEM;
            break;
        case SPE_CREATE_STONE_GOLEM:
            monstid = PM_STONE_GOLEM;
            break;
        case SPE_CREATE_IRON_GOLEM:
            monstid = PM_IRON_GOLEM;
            break;
        case SPE_CREATE_WOOD_GOLEM:
            monstid = PM_WOOD_GOLEM;
            break;
        case SPE_CREATE_PAPER_GOLEM:
            monstid = PM_PAPER_GOLEM;
            break;
        default:
            monstid = PM_STRAW_GOLEM;
            break;
        }
        (void)summoncreature(obj->otyp, monstid, "%s forms before you.", MM_SUMMON_IN_SMOKE_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_SUMMON_DEMON:
        known = TRUE;
        summondemon(obj->otyp);
        break;
    case SPE_CALL_DEMOGORGON:
        known = TRUE;
        You_ex(ATR_NONE, CLR_MSG_SPELL, "chant an invocation:");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Lord of the Abyss, Prince of Demons,");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "I call to thee, and I pledge myself to thee!");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "By the deluge of this blood sacrifice,");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Come forth and walk this plane once more!");
        summondemogorgon(obj->otyp);
        break;
    case SPE_FAITHFUL_HOUND:
        mtmp = summoncreature(obj->otyp, PM_LARGE_DOG, "%s appears out of nowhere.", MM_SUMMON_IN_SMOKE_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_CALL_HIERARCH_MODRON:
        (void)summoncreature(obj->otyp, (rn2(100) < (u.ulevel - 1) * 1) ? PM_MODRON_TERTIAN : (rn2(100) < (u.ulevel - 1) * 5) ? PM_MODRON_QUARTON : PM_MODRON_QUINTON, "%s appears in a cloud of smoke.", MM_SUMMON_IN_SMOKE_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
        break;
    case SPE_STICK_TO_SNAKE:
        mtmp = summoncreature(obj->otyp, PM_SNAKE, "You throw the stick you prepared in the front of you. It turns into %s!", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_STICK_TO_COBRA:
        mtmp = summoncreature(obj->otyp, PM_COBRA, "You throw the stick you prepared in the front of you. It turns into %s!", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_STICK_TO_PYTHON:
        mtmp = summoncreature(obj->otyp, PM_PYTHON, "You throw the stick you prepared in the front of you. It turns into %s!", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_STICK_TO_BOA:
        mtmp = summoncreature(obj->otyp, PM_BOA_CONSTRICTOR, "You throw the stick you prepared in the front of you. It turns into %s!", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_STICK_TO_GIANT_ANACONDA:
        mtmp = summoncreature(obj->otyp, PM_GIANT_ANACONDA, "You throw the stick you prepared in the front of you. It turns into %s!", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_CELESTIAL_DOVE:
        mtmp = summoncreature(obj->otyp, PM_CELESTIAL_DOVE, "%s descends from the heavens.", MM_LAWFUL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_CELESTIAL_EAGLE:
        mtmp = summoncreature(obj->otyp, PM_CELESTIAL_EAGLE, "%s descends from the heavens.", MM_LAWFUL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_SUMMON_PHOENIX:
        mtmp = summoncreature(obj->otyp, PM_PHOENIX, "%s descends from the heavens.", MM_LAWFUL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_SUMMON_GOLD_DRAGON:
        mtmp = summoncreature(obj->otyp, PM_GOLD_DRAGON, "%s descends from the heavens.", MM_LAWFUL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_SUMMON_ANCIENT_GOLD_DRAGON:
        mtmp = summoncreature(obj->otyp, PM_ANCIENT_GOLD_DRAGON, "%s descends from the heavens.", MM_LAWFUL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_CALL_BAHAMUT:
        known = TRUE;
        You_ex(ATR_NONE, CLR_MSG_SPELL, "chant an invocation:");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "O Platinum Dragon, King of Good Dragons,");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "I call to thee, and I pledge myself to thee!");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "By this meagre offering, please hear this call!");
        summonbahamut(obj->otyp);
        break;
    case SPE_SUMMON_TREANT:
        mtmp = summoncreature(obj->otyp, PM_TREANT, "%s appears before you.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_SUMMON_ELDER_TREANT:
        mtmp = summoncreature(obj->otyp, PM_ELDER_TREANT, "%s appears before you.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_SUMMON_GIANT_ANT:
    case SPE_SUMMON_SOLDIER_ANT:
    case SPE_SUMMON_DIREWOLF:
    case SPE_SUMMON_BISON:
    case SPE_SUMMON_PEGASUS:
    case SPE_SUMMON_ROC:
    case SPE_SUMMON_GRIZZLY_BEAR:
    case SPE_SUMMON_OWLBEAR:
    case SPE_SUMMON_OWLBEAR_PATRIARCH:
    case SPE_SUMMON_COCKATRICE:
    case SPE_SUMMON_GIANT_COCKATRICE:
    case SPE_SUMMON_GARGANTUAN_COCKATRICE:
    case SPE_SUMMON_GIANT_SPIDER:
    case SPE_SUMMON_PHASE_SPIDER:
    case SPE_SUMMON_PURPLE_WORM:
    case SPE_SUMMON_GARGANTUAN_BEETLE:
    case SPE_SUMMON_RAVEN:
    case SPE_SUMMON_WINTER_WOLF:
        mtmp = summoncreature(obj->otyp, get_summon_monster_type(obj->otyp), "%s appears before you.", MM_NEUTRAL_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_GUARDIAN_ANGEL:
        You_ex(ATR_NONE, CLR_MSG_SPELL, "recite an ancient prayer to %s.", u_gname());
        //gain_guardian_angel(TRUE);
        mtmp = summoncreature(obj->otyp, PM_ANGEL, "%s descends from the heavens.", MM_EMIN_COALIGNED | MM_LAWFUL_SUMMON_ANIMATION,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_SUMMON_ARCHON:
        mtmp = summoncreature(obj->otyp, PM_ARCHON, "%s descends from the heavens.", MM_EMIN_COALIGNED | MM_LAWFUL_SUMMON_ANIMATION,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_HEAVENLY_SERPENT:
        mtmp = summoncreature(obj->otyp, PM_COUATL, "%s descends from the heavens.", MM_EMIN_COALIGNED | MM_LAWFUL_SUMMON_ANIMATION, 
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_DIVINE_MOUNT:
        mtmp = summoncreature(obj->otyp, PM_KI_RIN, "%s appears before you.", MM_EMIN_COALIGNED | MM_LAWFUL_SUMMON_ANIMATION, 
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
        if (mtmp)
        {
            otmp = mksobj(SADDLE, TRUE, FALSE, FALSE);
            if (otmp)
            {
                otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
                put_saddle_on_mon(otmp, mtmp);
            }
        }
        break;
    case SPE_HEAVENLY_ARMY:
        You_ex(ATR_NONE, CLR_MSG_SPELL, "recite an old prayer to %s...", u_gname());
        for (int n = d(2, 4); n > 0; n--)
        {
            int roll = rn2(3);
            switch (roll)
            {
            case 0:
                monstid = PM_KI_RIN;
                break;
            case 1:
                monstid = PM_ANGEL;
                break;
            case 2:
                monstid = PM_ARCHON;
                break;
            default:
                monstid = PM_COUATL;
                break;
            }
            mtmp = summoncreature(obj->otyp, monstid, "", MM_EMIN_COALIGNED | MM_LAWFUL_SUMMON_ANIMATION, 
                SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_PACIFIST | SUMMONCREATURE_FLAGS_FAITHFUL);
            if (mtmp)
            {
                if (monstid == PM_ARCHON)
                    archoncount++;
                else if (monstid == PM_ANGEL)
                    angelcount++;
                else
                    aleaxcount++;
            }
        }

        if (archoncount > 0 || angelcount > 0 || aleaxcount > 0)
        {
            if (archoncount == 0)
                Strcpy(archonbuf, "");
            else if (archoncount == 1)
                Strcpy(archonbuf, "an Archon");
            else
                Sprintf(archonbuf, "%d Archons", archoncount);

            if (angelcount == 0)
                Strcpy(angelbuf, "");
            else if (angelcount == 1)
                Strcpy(angelbuf, "an Angel");
            else
                Sprintf(angelbuf, "%d Angels", angelcount);

            if (aleaxcount == 0)
                Strcpy(aleaxbuf, "");
            else if (aleaxcount == 1)
                Strcpy(aleaxbuf, "an Aleax");
            else
                Sprintf(aleaxbuf, "%d Aleaxes", aleaxcount);

            if (archoncount == 1)
                *archonbuf = highc(*archonbuf);
            else if (archoncount == 0 && angelcount == 1)
                *angelbuf = highc(*angelbuf);
            else if (archoncount == 0 && angelcount == 0 && aleaxcount == 1)
                *aleaxbuf = highc(*aleaxbuf);

            if (archoncount > 0 && angelcount > 0 && aleaxcount > 0)
            {
                Sprintf(buf, "%s, %s, and %s", archonbuf, angelbuf, aleaxbuf);
            }
            else if(archoncount > 0 && angelcount > 0) // No Aleax
            {
                Sprintf(buf, "%s and %s", archonbuf, angelbuf);
            }
            else if (archoncount > 0 && aleaxcount > 0) // No Angels
            {
                Sprintf(buf, "%s and %s", archonbuf, aleaxbuf);
            }
            else if (angelcount > 0 && aleaxcount > 0) // No Archon
            {
                Sprintf(buf, "%s and %s", angelbuf, aleaxbuf);
            }
            else if (archoncount > 0) // Only Archon
            {
                Sprintf(buf, "%s", archonbuf);
            }
            else if (angelcount > 0) // Only Angel
            {
                Sprintf(buf, "%s", angelbuf);
            }
            else if (aleaxcount > 0) // No Aleax
            {
                Sprintf(buf, "%s", aleaxbuf);
            }
            if (aleaxcount + angelcount + archoncount == 1)
            {
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s descends from the heavens.", buf);
            }
            else
            {
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s descend from the heavens!", buf);
            }
        }

        break;
    case SPE_GREAT_YENDORIAN_SUMMONING: 
    {
        You_ex(ATR_NONE, CLR_MSG_SPELL, "successfully cast Great Yendorian Summoning.");
        int monstcount = 0;
        int vismonstcount = 0;
        struct monst* lastseenmon = (struct monst*)0;

        for (int n = d(1, 2); n > 0; n--)
        {
            monstid = pick_nasty(u.ulevel);
            mtmp = summoncreature(obj->otyp, monstid, "", MM_SUMMON_NASTY_ANIMATION, 
                SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_BLOODLUST);
            if (mtmp)
            {
                monstcount++;
                if (canseemon(mtmp))
                {
                    lastseenmon = mtmp;
                    vismonstcount++;
                }
            }
        }
        if (monstcount == 0)
            pline("However, nothing happens.");
        else if (vismonstcount == 0)
            pline("However, nothing seems to happen.");
        else if (vismonstcount == 1) {
            if (lastseenmon)
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s appears in a cloud of smoke!", Amonnam(lastseenmon));
        }
        else
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "Monsters appear in a cloud of smoke!");

        break;
    }
    case SPE_RAISE_SKELETON:
        mtmp = summoncreature(obj->otyp, PM_SKELETON, "%s raises before you and is ready to serve you.", MM_UNDEAD_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_RAISE_SKELETON_WARRIOR:
        mtmp = summoncreature(obj->otyp, PM_SKELETON_WARRIOR, "%s raises before you and is ready to serve you.", MM_UNDEAD_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_RAISE_SKELETON_LORD:
        mtmp = summoncreature(obj->otyp, PM_SKELETON_WARRIOR, "%s raises before you.", MM_UNDEAD_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_RAISE_SKELETON_KING:
        mtmp = summoncreature(obj->otyp, PM_SKELETON_KING, "%s raises before you.", MM_UNDEAD_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_RAISE_GIANT_SKELETON:
        mtmp = summoncreature(obj->otyp, PM_GIANT_SKELETON, "%s raises before you and is ready to serve you.", MM_UNDEAD_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
            SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH | SUMMONCREATURE_FLAGS_FAITHFUL);
        break;
    case SPE_CALL_GHOUL:
    {
        You_ex(ATR_NONE, CLR_MSG_SPELL, "call out for nearby ghouls in the dungeon.");
        int monstcount = 0;
        int vismonstcount = 0;
        struct monst* lastseenmon = (struct monst*)0;

        for (int n = d(2, 4); n > 0; n--)
        {
            monstid = PM_GHOUL;
            mtmp = summoncreature(obj->otyp, monstid, "", MM_UNDEAD_SUMMON_ANIMATION | MM_NO_MONSTER_INVENTORY,
                SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
            if (mtmp)
            {
                monstcount++;
                if (canseemon(mtmp))
                {
                    lastseenmon = mtmp;
                    vismonstcount++;
                }
            }
        }
        if (monstcount == 0)
            pline("However, nothing happens.");
        else if (vismonstcount == 0)
            pline("However, nothing seems to happen.");
        else if (vismonstcount == 1)
        {
            if (lastseenmon)
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s crawls out of nowhere!", Amonnam(lastseenmon));
        }
        else if (vismonstcount == 2)
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "Two ghouls crawl out of nowhere!");
        else
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "Several ghouls crawl out of nowhere!");


        break;
    }
    case SPE_MASS_RAISE_ZOMBIE:
    {
        You_ex(ATR_NONE, CLR_MSG_SPELL, "successfully cast the voodoo animation spell.");
        int zombietype;
        int monstcount = 0;
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        struct obj* sobj;
        sobj = fobj;

        while(sobj)
        {
            zombietype = NON_PM;
            if ((radius < 0 || dist2(u.ux, u.uy, sobj->ox, sobj->oy) <= radius * (radius + 1)) //+1 to make a bit larger and square-like
                && cansee(sobj->ox, sobj->oy)
                && !IS_STWALL(levl[sobj->ox][sobj->oy].typ))
            {
                if (sobj->otyp == CORPSE && sobj->corpsenm > NON_PM)
                {
                    zombietype = mon_to_zombie(sobj->corpsenm);
                    if (zombietype > NON_PM)
                    {
                        if (animate_corpse(sobj, zombietype))
                            monstcount++;
                        sobj = fobj; //The corpse got deleted, so move to beginning
                        continue;
                    }
                }
            }
            sobj = sobj->nobj;
        }
        if (monstcount == 0)
            pline("However, nothing happens.");
        break;
    }
    case SPE_MASS_CREATE_MUMMY:
    {
        You_ex(ATR_NONE, CLR_MSG_SPELL, "successfully permormed the embalming magic.");
        int zombietype;
        int monstcount = 0;
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        struct obj* sobj;
        sobj = fobj;

        while (sobj)
        {
            zombietype = -1;
            if ((radius < 0 || dist2(u.ux, u.uy, sobj->ox, sobj->oy) <= radius * (radius + 1)) //+1 to make a bit larger and square-like
                && cansee(sobj->ox, sobj->oy)
                && !IS_STWALL(levl[sobj->ox][sobj->oy].typ))
            {
                if (sobj->otyp == CORPSE && sobj->corpsenm > NON_PM)
                {
                    zombietype = mon_to_mummy(sobj->corpsenm);
                    if (zombietype > NON_PM)
                    {
                        if (animate_corpse(sobj, zombietype))
                            monstcount++;
                        sobj = fobj; //The corpse got deleted, so move to beginning
                        continue;
                    }
                }
            }
            sobj = sobj->nobj;
        }
        if(monstcount == 0)
            pline("However, nothing happens.");

        break;
    }
    case SPE_MASS_CREATE_DRACOLICH:
    {
        You_ex(ATR_NONE, CLR_MSG_SPELL, "successfully permormed the necromantic magic.");
        int zombietype;
        int monstcount = 0;
        int hatchlingcount = 0;
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        struct obj* sobj;
        sobj = fobj;

        while (sobj)
        {
            zombietype = -1;
            if ((radius < 0 || dist2(u.ux, u.uy, sobj->ox, sobj->oy) <= radius * (radius + 1)) //+1 to make a bit larger and square-like
                && cansee(sobj->ox, sobj->oy)
                && !IS_STWALL(levl[sobj->ox][sobj->oy].typ))
            {
                if (sobj->otyp == CORPSE && sobj->corpsenm >= LOW_PM)
                {
                    if ((obj->corpsenm >= PM_GRAY_DRAGON && obj->corpsenm <= PM_ANCIENT_YELLOW_DRAGON)
                        || obj->corpsenm == PM_GOLD_DRAGON || obj->corpsenm == PM_ANCIENT_GOLD_DRAGON
                        || obj->corpsenm == PM_BAHAMUT || obj->corpsenm == PM_TIAMAT
                        )
                    {
                        if ((mons[obj->corpsenm].geno & G_UNIQ) || (obj->corpsenm >= PM_ANCIENT_GRAY_DRAGON && obj->corpsenm <= PM_ANCIENT_YELLOW_DRAGON) || obj->corpsenm == PM_ANCIENT_GOLD_DRAGON)
                            zombietype = PM_ELDER_DRACOLICH;
                        else
                            zombietype = PM_DRACOLICH;

                        if (animate_corpse(sobj, zombietype))
                            monstcount++;
                        sobj = fobj; //The corpse got deleted, so move to the beginning
                        continue;
                    }
                    else if (sobj->corpsenm >= PM_GRAY_DRAGON_HATCHLING && sobj->corpsenm <= PM_YELLOW_DRAGON_HATCHLING)
                    {
                        hatchlingcount++;
                        pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s twitches for a moment, but nothing else happens.", The(cxname(sobj)));
                    }
                }
            }
            sobj = sobj->nobj;
        }

        if (monstcount == 0)
        {
            if(hatchlingcount == 0)
                pline("However, nothing happens.");
            else
                You_feel_ex(ATR_NONE, CLR_MSG_HINT, "the corpse%s might not be sufficiently large for the spell.", hatchlingcount > 1 ? "s" : "");
        }
        break;
    }    
    case SPE_SPHERE_OF_ANNIHILATION:
    {
        You_ex(ATR_NONE, CLR_MSG_SPELL, "chant an invocation:");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou who art darker than the blackest pitch, deeper than the deepest night.");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "I call upon thee, and swear myself to thee.");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Let the fools who stand before me be destroyed by the power you and I possess!");
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "Air begins to shine with strange golden color...");
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "Suddenly immense power blasts all around you!");

        int radius = (int)objects[obj->otyp].oc_spell_radius;

        boolean showmon = FALSE;
        for (int i = 0; i < 10; i++)
        {
            for (int x = u.ux - radius; x <= u.ux + radius; x++)
            {
                for (int y = u.uy - radius; y <= u.uy + radius; y++)
                {
                    if (isok(x, y))
                    {
                        if (i < 9)
                        {
                            if (cansee(x, y)) { /* Don't see anything if can't see the location */
                                if (showmon)
                                    newsym(x, y); /* restore the old information */
                                else
                                    show_glyph(x, y, cmap_to_glyph(S_unexplored));
                            }
                        }
                        else
                            newsym(x, y); /* restore the old information */
                    }
                }
            }
            flush_screen(1); /* make sure the glyph shows up */
            adjusted_delay_output();
            adjusted_delay_output();
            showmon = !showmon;
        }

        for (struct monst* mon = fmon; mon; mon = mon->nmon)
        {
            if (radius <= 0 || dist2(u.ux, u.uy, mon->mx, mon->my) <= radius * (radius + 1))
            {
                if (is_peaceful(mon))
                    setmangry(mon, FALSE);

                /* No other saving throw */
                //boolean magic_resistance_success = check_magic_resistance_and_inflict_damage(mon, (struct obj*)0, origmonst,  u.ulevel, 0, 0, NOTELL);

                if (resists_disint(mon))
                {
                    if (canspotmon(mon))
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                        m_shieldeff(mon);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected!", Monnam(mon));
                    }
                }
                else if (!DEADMONSTER(mon) && mon != u.usteed)
                {
                    maybe_disintegrate_mon(mon, 1, "sphere of annihilation");
                }
            }
        }

        break;
    }
    case SPE_CIRCLE_OF_SUNLIGHT:
    case SPE_CIRCLE_OF_RADIANCE:
    {
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        play_explosion_animation_at(u.ux, u.uy, EXPL_CIRCLE_OF_RADIANCE);
        play_sfx_sound(SFX_CIRCLE_OF_RADIANCE);
        explosion_wait_until_action();
        for (struct monst* mon = fmon; mon; mon = mon->nmon)
        {
            if (dist2(u.ux, u.uy, mon->mx, mon->my) <= radius * (radius + 1))
            {
                int dmg = get_spell_damage(obj->otyp, &youmonst, mon);
                if (is_undead(mon->data) || is_demon(mon->data) || is_vampshifter(mon) || hates_light(mon->data))
                {
                    if (is_peaceful(mon))
                        setmangry(mon, FALSE);

                    if (!DEADMONSTER(mon) && mon != u.usteed)
                    {
                        (void)check_magic_resistance_and_inflict_damage(mon, (struct obj*)0, &youmonst, TRUE, dmg, AD_CLRC, canspotmon(mon) ? TELL : NOTELL);
                    }
                }
            }
        }
        explosion_wait_until_end();
        break;
    }
    case SPE_CIRCLE_OF_FIRE:
    {
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        play_explosion_animation_at(u.ux, u.uy, EXPL_CIRCLE_OF_FIRE);
        play_sfx_sound(SFX_CIRCLE_OF_FIRE);
        explosion_wait_until_action();
        for (struct monst* mon = fmon; mon; mon = mon->nmon)
        {
            if (dist2(u.ux, u.uy, mon->mx, mon->my) <= radius * (radius + 1))
            {
                if (is_peaceful(mon))
                    setmangry(mon, FALSE);

                int dmg = get_spell_damage(obj->otyp, &youmonst, mon);
                if (is_mon_immune_to_fire(mon))
                {
                    if (canspotmon(mon))
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                        m_shieldeff(mon);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected!", Monnam(mon));
                    }
                }
                else if (!DEADMONSTER(mon) && mon != u.usteed)
                {
                    (void)check_magic_resistance_and_inflict_damage(mon, (struct obj*)0, &youmonst, TRUE, dmg, AD_FIRE, canspotmon(mon) ? TELL : NOTELL);
                }
            }
        }
        explosion_wait_until_end();
        break;
    }
    case SPE_CIRCLE_OF_FROST:
    {
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        play_explosion_animation_at(u.ux, u.uy, EXPL_CIRCLE_OF_FROST);
        play_sfx_sound(SFX_CIRCLE_OF_FROST);
        explosion_wait_until_action();
        for (struct monst* mon = fmon; mon; mon = mon->nmon)
        {
            if (dist2(u.ux, u.uy, mon->mx, mon->my) <= radius * (radius + 1))
            {
                if (is_peaceful(mon))
                    setmangry(mon, FALSE);

                int dmg = get_spell_damage(obj->otyp, &youmonst, mon);
                if (is_mon_immune_to_cold(mon))
                {
                    if (canspotmon(mon))
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                        m_shieldeff(mon);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected!", Monnam(mon));
                    }
                }
                else if (!DEADMONSTER(mon) && mon != u.usteed)
                {
                    (void)check_magic_resistance_and_inflict_damage(mon, (struct obj*)0, &youmonst, TRUE, dmg, AD_COLD, canspotmon(mon) ? TELL : NOTELL);
                }
            }
        }
        explosion_wait_until_end();
        break;
    }
    case SPE_CIRCLE_OF_LIGHTNING:
    {
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        play_explosion_animation_at(u.ux, u.uy, EXPL_CIRCLE_OF_LIGHTNING);
        play_sfx_sound(SFX_CIRCLE_OF_LIGHTNING);
        explosion_wait_until_action();
        for (struct monst* mon = fmon; mon; mon = mon->nmon)
        {
            if (dist2(u.ux, u.uy, mon->mx, mon->my) <= radius * (radius + 1))
            {
                if (is_peaceful(mon))
                    setmangry(mon, FALSE);

                int dmg = get_spell_damage(obj->otyp, &youmonst, mon);
                if (is_mon_immune_to_elec(mon))
                {
                    if (canspotmon(mon))
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                        m_shieldeff(mon);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected!", Monnam(mon));
                    }
                }
                else if (!DEADMONSTER(mon) && mon != u.usteed)
                {
                    (void)check_magic_resistance_and_inflict_damage(mon, (struct obj*)0, &youmonst, TRUE, dmg, AD_ELEC, canspotmon(mon) ? TELL : NOTELL);
                }
            }
        }
        explosion_wait_until_end();
        break;
    }    
    case SPE_CIRCLE_OF_MAGIC:
    {
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        play_explosion_animation_at(u.ux, u.uy, EXPL_CIRCLE_OF_MAGIC);
        play_sfx_sound(SFX_CIRCLE_OF_MAGIC);
        explosion_wait_until_action();
        for (struct monst* mon = fmon; mon; mon = mon->nmon)
        {
            if (dist2(u.ux, u.uy, mon->mx, mon->my) <= radius * (radius + 1))
            {
                if(is_peaceful(mon))
                    setmangry(mon, FALSE);

                int dmg = get_spell_damage(obj->otyp, &youmonst, mon);
                if (resists_magic(mon))
                {
                    if (canspotmon(mon))
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                        m_shieldeff(mon);
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected!", Monnam(mon));
                    }
                }
                else if (!DEADMONSTER(mon) && mon != u.usteed)
                {
                    (void)check_magic_resistance_and_inflict_damage(mon, (struct obj*)0, &youmonst, FALSE, dmg, AD_MAGM, canspotmon(mon) ? TELL : NOTELL);
                }
            }
        }
        explosion_wait_until_end();
        break;
    }
    case SPE_CIRCLE_OF_TELEPORTATION:
    {
        int radius = (int)objects[obj->otyp].oc_spell_radius;
        for (struct monst* mon = fmon; mon; mon = mon->nmon)
        {
            if (dist2(u.ux, u.uy, mon->mx, mon->my) <= radius * (radius + 1))
            {
                if (!DEADMONSTER(mon) && mon != u.usteed && mon != &youmonst
                    && !check_magic_resistance_and_inflict_damage(mon, (struct obj*)0, &youmonst, FALSE, 0, 0, canspotmon(mon) ? TELL : NOTELL))
                {
                    (void)u_teleport_mon(mon, TRUE);

                    if (is_peaceful(mon))
                        setmangry(mon, FALSE);
                }
            }
        }

        break;
    }
    case SPE_DETECT_UNSEEN:
    {
        int msg = Invisib && !Blind;
        incr_itimeout(&HSee_invisible, duration);
        set_mimic_blocking(); /* do special mimic handling */
        see_monsters();       /* see invisible monsters */
        newsym(u.ux, u.uy);   /* see yourself! */
        if (msg && !Blind) {  /* Blind possible if polymorphed */
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "can see through yourself, but you are visible!");
        }
    }
    /* FALLTHRU */
    case WAN_SECRET_DOOR_DETECTION:
        if (!findit())
            return;
        if (!Blind)
            known = TRUE;
        break;
    case WAN_TRAP_DETECTION:
        if (level.flags.nommap)
        {
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s spins as %s blocks the magic!", body_part(HEAD),
                something);
            play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            make_confused(itimeout_incr(HConfusion, rnd(30)), FALSE);
            break;
        }
        if (trap_detect(obj))
        {
            if (objects[WAN_TRAP_DETECTION].oc_name_known)
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
            return; /* nothing detected */
        }
        You_ex(ATR_NONE, CLR_MSG_SPELL, "become aware of the location of nearby traps!");
        known = TRUE;
        break;
    case WAN_ORE_DETECTION:
        if (object_detect(obj, GEM_CLASS))
        {
            if (objects[WAN_ORE_DETECTION].oc_name_known)
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
            return; /* nothing detected */
        }
        known = TRUE;
        break;
    case WAN_TOWN_PORTAL:
    {
        int number = obj->cursed ? 4 : obj->blessed ? 2 : 3;
        boolean hostilemonfound = FALSE;
        struct monst* hostilemon;
        for (hostilemon = fmon; hostilemon; hostilemon = hostilemon->nmon)
        {
            if (!is_peaceful(hostilemon) && couldsee(hostilemon->mx, hostilemon->my) && distu(hostilemon->mx, hostilemon->my) < number * number)
            {
                hostilemonfound = TRUE;
                break;
            }
        }
        if (hostilemonfound 
            || (context.last_turn_when_took_damage > 0 && moves >= context.last_turn_when_took_damage && moves <= context.last_turn_when_took_damage + number)
            || (obj->cursed && !rn2(3)))
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "The wand sparkles for a while, but nothing else happens.");
            break;
        }

        if (u.uhave.amulet)
        {
            /* Amulet of Yendor prevents any town portal */
            play_sfx_sound(SFX_MYSTERIOUS_FORCE_PREVENTS);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "A mysterious force prevents you from teleporting!");
            break;
        }

#define NUM_WAYPOINTS 4
        d_level* waypointlist[NUM_WAYPOINTS] = { &minetown_level, &sokoend_level, &stronghold_level, &orcus_level };

        boolean onwaypointlevel = FALSE;
        int wpidx;
        for (wpidx = 0; wpidx < NUM_WAYPOINTS; wpidx++)
        {
            if (on_level(&u.uz, waypointlist[wpidx]))
            {
                onwaypointlevel = TRUE;
                break;
            }
        }

        int wpcnt = 0;
        d_level* last_wp = 0;
        for (wpidx = 0; wpidx < NUM_WAYPOINTS; wpidx++)
        {
            mapseen* wpmapseen = find_mapseen(waypointlist[wpidx]);
            if (wpidx == 0 || wpmapseen != 0)
            {
                wpcnt++;
                if (wpmapseen != 0)
                    last_wp = waypointlist[wpidx];
            }
        }

        if (onwaypointlevel)
        {
            int wpcnt_others = wpcnt - 1;

            if (wpcnt_others == 0)
            {
                if (context.town_portal_return_level_set && !(obj->cursed && !rn2(3)))
                {
                    level_tele(0, 2, context.town_portal_return_level, context.town_portal_return_flags);
                }
                else
                {
                    level_tele(0, 0, zerodlevel, 0);
                }
            }
            else
            {
                winid menuwin;
                menu_item* selected = (menu_item*)0;
                int n = 0;

                menuwin = create_nhwindow(NHW_MENU);
                start_menu_ex(menuwin, GHMENU_STYLE_CHOOSE_SIMPLE);
                anything any = zeroany;

                for (wpidx = 0; wpidx < NUM_WAYPOINTS; wpidx++)
                {
                    if ((wpidx == 0 || find_mapseen(waypointlist[wpidx]) != 0) && !on_level(&u.uz, waypointlist[wpidx]))
                    {
                        any = zeroany;
                        any.a_int = wpidx + 1;
                        char wpbuf[BUFSZ] = "";
                        s_level* slev = Is_special(waypointlist[wpidx]);
                        if (slev)
                        {
                            const char* dname = dungeons[waypointlist[wpidx]->dnum].dname;
                            if (dname && !strncmp(dname, "The ", 4))
                                dname += 4;
                            if (dname)
                                Sprintf(wpbuf, "%s - %s", dname, slev->name);
                            else
                                Sprintf(wpbuf, "%s", slev->name);
                            add_menu(menuwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                                wpbuf, MENU_UNSELECTED);
                        }
                    }
                }

                any = zeroany;
                any.a_int = NUM_WAYPOINTS + 1;
                add_menu(menuwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                    "Last town portal level", MENU_UNSELECTED);

                end_menu(menuwin, "Pick a level to teleport to");
                n = select_menu(menuwin, PICK_ONE, &selected);
                if (n > 0)
                {
                    int selidx = selected->item.a_int;
                    if (selidx > 0 && selidx <= NUM_WAYPOINTS)
                        level_tele(0, 2, *waypointlist[selidx - 1], 0);
                    else if (selidx > NUM_WAYPOINTS)
                    {
                        if (context.town_portal_return_level_set && !(obj->cursed && !rn2(3)))
                        {
                            level_tele(0, 2, context.town_portal_return_level, context.town_portal_return_flags);
                        }
                        else
                        {
                            level_tele(0, 0, zerodlevel, 0);
                        }
                    }
                    free((genericptr_t)selected);
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "The wand sparkles for a while, but nothing else happens.");
                }

                destroy_nhwindow(menuwin);
            }
        }
        else
        {
            if(obj->cursed && !rn2(3))
            {
                level_tele(0, 0, zerodlevel, 0);
            }
            else
            {
                context.town_portal_return_level = u.uz;
                context.town_portal_return_level_set = TRUE;
                context.town_portal_return_flags = In_W_tower(u.ux, u.uy, &u.uz);

                if (wpcnt <= 1)
                {
                    level_tele(0, 2, last_wp ? *last_wp : minetown_level, 0);
                }
                else
                {
                    winid menuwin;
                    menu_item* selected = (menu_item*)0;
                    int n = 0;

                    menuwin = create_nhwindow(NHW_MENU);
                    start_menu_ex(menuwin, GHMENU_STYLE_CHOOSE_SIMPLE);
                    anything any = zeroany;

                    for (wpidx = 0; wpidx < NUM_WAYPOINTS; wpidx++)
                    {
                        if (wpidx == 0 || find_mapseen(waypointlist[wpidx]) != 0)
                        {
                            any = zeroany;
                            any.a_int = wpidx + 1;
                            char wpbuf[BUFSZ] = "";
                            s_level* slev = Is_special(waypointlist[wpidx]);
                            if (slev)
                            {
                                const char* dname = dungeons[waypointlist[wpidx]->dnum].dname;
                                if (dname && !strncmp(dname, "The ", 4))
                                    dname += 4;
                                if(dname)
                                    Sprintf(wpbuf, "%s - %s", dname, slev->name);
                                else
                                    Sprintf(wpbuf, "%s", slev->name);

                                add_menu(menuwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                                    wpbuf, MENU_UNSELECTED);
                            }
                        }
                    }

                    end_menu(menuwin, "Pick a waypoint to teleport to");
                    n = select_menu(menuwin, PICK_ONE, &selected);
                    if (n > 0)
                    {
                        int selidx = selected->item.a_int;
                        if(selidx > 0)
                            level_tele(0, 2, *waypointlist[selidx - 1], 0);
                        free((genericptr_t)selected);
                    }
                    else
                    {
                        pline_ex(ATR_NONE, CLR_MSG_WARNING, "The wand sparkles for a while, but nothing else happens.");
                    }

                    destroy_nhwindow(menuwin);
                }
                makeknown(obj->otyp);
            }
        }
        break;
    }
    case WAN_CREATE_MONSTER:
        known = create_critters(rn2(23) ? 1 : rn1(7, 2),
                                (struct permonst *) 0, TRUE);
        break;
    case WAN_WISHING:
        known = TRUE;
        if (Luck + rn2(5) < 0) {
            pline("Unfortunately, nothing happens.");
            break;
        }
        makewish(FALSE, TRUE);
        break;
    case WAN_IDENTIFY:
        if (invent)
        {
            if(!objects[obj->otyp].oc_name_known)
                pline("This is a wand of identify.");

            known = TRUE;
            (void)identify_pack(1, FALSE);
            update_inventory();
        }
        else
        {
            pline("Nothing seems to happens.");
        }
        break;
    case SPE_TELEPORT_SELF:
        tele();
        break;
    case SPE_CONTROLLED_TELEPORT:
        controlled_teleportation();
        break;
    case SPE_LEVEL_TELEPORT:
        level_tele(2, FALSE, zerodlevel, 0);
        break;
    case SPE_CONTROLLED_LEVEL_TELEPORT:
        level_tele(2, TRUE, zerodlevel, 0);
        break;
    case SPE_PORTAL:
        create_portal();
        break;
    case SPE_ENLIGHTENMENT:
    case WAN_ENLIGHTENMENT:
        known = TRUE;
        You_feel("self-knowledgeable...");
        display_nhwindow(WIN_MESSAGE, FALSE);
        enlightenment(MAGICENLIGHTENMENT, ENL_GAMEINPROGRESS);
        pline_The("feeling subsides.");
        exercise(A_WIS, TRUE);
        break;
    case SPE_COMMUNE:
        known = TRUE;
        You_ex(ATR_NONE, CLR_MSG_SPELL, "commune with %s...", u_gname());
        display_nhwindow(WIN_MESSAGE, FALSE);

        if (can_pray(FALSE))
            You_feel("that you can safely pray.");
        else
        {
            You_feel("that you cannot safely pray.");

            if (u.ugangr)
                You("also feel that %s is %sangry with you.", u_gname(), u.ugangr > 6 ? "extremely " : u.ugangr > 3 ? "very " : "");

            if (u.uprayer_timeout > 0)
            {
                You("query about your prayer conduct. The number %d appears before you.", u.uprayer_timeout / 10 + 1);

                if (u.uprayer_timeout > 300)
                    You_feel("that %s is quite tired of your constant whining.", u_gname());

                pline("It feels that %s wait %sbefore bothering %s again.",
                    u.uprayer_timeout >= 50 ? "it would be wise to" : "you must",
                    u.uprayer_timeout < 50 ? "a little longer " : u.uprayer_timeout > 200 ? "a long time " : "",
                    u_gname());
            }
        }

        You("query about your fortune.");

        if (Luck < 0)
            You("see a number of %d. You feel that it is %s unlucky number.",
                abs(Luck), abs(Luck) >= 10 ? "an extremely" : abs(Luck) >= 5 ? "a very" : "an");
        else if (Luck > 0)
            You("see a number of %d. You feel that it is %s lucky number.",
                Luck, Luck >= 10 ? "an extremely" : Luck >= 5 ? "a very" : "a");
        else
            Your("vision is neutral.");

        You("finish communing with %s.", u_gname());
        break;
    case SPE_PRAYER:
    {
        int orig_uprayer_timeout = u.uprayer_timeout;
        u.uprayer_timeout = 0;
        context.spellpray = TRUE;
        (void)dopray();
        context.spellpray = FALSE;
        u.uprayer_timeout = orig_uprayer_timeout;
        break;

    }
    case SPE_ABSOLUTION:
        You_ex(ATR_NONE, CLR_MSG_SPELL, "recite an aeon-old prayer to %s.", u_gname());
        (void)absolution_spell();
        break;
    }

    if (known)
    {
//        if (!objects[obj->otyp].oc_name_known)
//            more_experienced(0, 10);
        /* effect was observable; discover the wand type provided
           that the wand itself has been seen */
        learnwand(obj);
    }
}

STATIC_OVL void
backfire(otmp)
struct obj *otmp;
{
    int dmg;

    otmp->in_use = TRUE; /* in case losehp() is fatal */
    play_sfx_sound(SFX_EXPLOSION_MAGICAL);
    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s suddenly explodes!", The(xname(otmp)));
    dmg = d(otmp->charges + 2, 6);
    losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_MAGM, ADFLAGS_NONE), "exploding wand", KILLED_BY_AN);
    useup(otmp);
}

STATIC_VAR NEARDATA const char zap_syms[] = { WAND_CLASS, 0 };

/* 'z' command (or 'y' if numbed_pad==-1) */
int
dozap()
{
    register struct obj *obj;
    double damage;
    boolean taketurn = TRUE;

    if (check_capacity((char *) 0))
        return 0;
    obj = getobj(zap_syms, "zap", 0, "");
    if (!obj)
        return 0;

    if (obj->otyp == WAN_PROBING)
        taketurn = FALSE;

    if (obj->cooldownleft > 0)
    {
        play_sfx_sound(SFX_NOT_READY_YET);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot zap %s before its cooldown has expired.", the(cxname(obj)));
        return 0;
    }
    else if (Cancelled)
    {
        play_sfx_sound(SFX_CANCELLATION_IN_FORCE);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "magic is not flowing properly to allow for using a wand.");
        return 0;
    }

    check_unpaid(obj);

    /* zappable addition done by GAN 11/03/86 */
    if (!zappable(obj))
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline_ex1(ATR_NONE, CLR_MSG_FAIL, nothing_happens);

        //Mark empty query
        if((obj->speflags & SPEFLAGS_EMPTY_NOTICED) == 0 && obj->charges >= 0)
        {
            obj->speflags |= SPEFLAGS_EMPTY_NOTICED;
            boolean canstash = can_stash_objs();
            boolean markempty = TRUE;
            char markbuf[BUFSZ];
            char querybuf[BUFSZ];
            if (obj->dknown && obj->known)
            {
                Sprintf(markbuf, "%s empty. ", Tobjnam(obj, "are"));
                markempty = FALSE;
            }
            else if (has_uoname(obj) && strstri(UONAME(obj), "empty"))
            {
                Sprintf(markbuf, "%s marked empty. ", Tobjnam(obj, "are"));
                markempty = FALSE;
            }
            else
                Sprintf(markbuf, "Mark %s empty and then ", the(cxname(obj)));

            Sprintf(querybuf, "%s%sDrop it?", markbuf, canstash ? "Stash or " : "");
            char ans = yn_function_es(YN_STYLE_GENERAL, ATR_NONE, NO_COLOR, 
                markempty ? "Mark Empty" : obj->oclass == WAND_CLASS ? "Wand Empty" : "Item Empty", 
                querybuf, markempty ? (!canstash ? "dmq" : "sdmq") : (!canstash ? "dq" : "sdq"), 'q', 
                markempty ? (!canstash ? "Drop\nMark Only\nQuit" : "Stash\nDrop\nMark Only\nQuit") : (!canstash ? "Drop\nQuit" : "Stash\nDrop\nQuit"), 
                (const char*)0);

            if (ans != 'q' && markempty && !(has_uoname(obj) && strstri(UONAME(obj), "empty")))
            {
                //Name it empty
                if (has_uoname(obj))
                {
                    char emptybuf[PL_PSIZ + 10];
                    Sprintf(emptybuf, "%s empty", UONAME(obj));
                    (void)uoname(obj, emptybuf);
                }
                else
                    (void)uoname(obj, "empty");

                if (ans == 'm')
                {
                    prinv("Marked empty:", obj, 0L);
                    update_inventory();
                }
            }

            switch (ans)
            {
            case 'd':
                taketurn = TRUE;
                getobj_autoselect_obj = obj;
                (void)dodrop();
                getobj_autoselect_obj = (struct obj*)0;
                break;
            case 's':
                taketurn = TRUE;
                getobj_autoselect_obj = obj;
                (void)dostash();
                getobj_autoselect_obj = (struct obj*)0;
                break;
            case 'q':
            default:
                break;
            }
        }
    }
    else if (obj->cursed && !rn2(WAND_BACKFIRE_CHANCE)) 
    {
        backfire(obj); /* the wand blows up in your face! */
        exercise(A_STR, FALSE);
        return taketurn;
    }
    else if (!(objects[obj->otyp].oc_dir == NODIR) && !getdir((char *) 0)) 
    {
        play_sfx_sound(SFX_WALL_GLOWS_THEN_FADES);
        if (!Blind)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s glows and fades.", The(xname(obj)));
        /* make him pay for knowing !NODIR */
    } 
    else if (!u.dx && !u.dy && !u.dz
               && !(objects[obj->otyp].oc_dir == NODIR)) 
    {

        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_ZAP);

        if ((damage = zapyourself(obj, TRUE)) != 0) 
        {
            char buf[BUFSZ];

            Sprintf(buf, "zapped %sself with a wand", uhim());
            losehp(damage, buf, NO_KILLER_PREFIX);
        }
    }
    else
    {
        /*      Are we having fun yet?
         * weffects -> buzz(obj->otyp) -> zhitm (temple priest) ->
         * attack -> hitum -> known_hitum -> ghod_hitsu ->
         * buzz(AD_ELEC) -> destroy_item(WAND_CLASS) ->
         * useup -> obfree -> dealloc_obj -> free(obj)
         */
        update_u_facing(TRUE);
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_ZAP);

        current_wand = obj;
        weffects(obj);
        obj = current_wand;
        current_wand = 0;
    }
    if (obj && obj->charges < 0) 
    {
        play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
        pline("%s to dust.", Tobjnam(obj, "turn"));
        useup(obj);
    }
    update_inventory(); /* maybe used a charge */
    return taketurn;
}

double
zapyourself(obj, ordinary)
struct obj *obj;
boolean ordinary;
{
    if (!obj)
        return 0.0;

    boolean learn_it = FALSE;
    int basedmg = get_spell_damage(obj->otyp, &youmonst, &youmonst);
    int duration = get_obj_spell_duration(obj);
    double damage = 0;
    //boolean magic_resistance_success = check_magic_resistance_and_inflict_damage(&youmonst, obj, FALSE, 0, 0, NOTELL);
    int save_adj = get_saving_throw_adjustment(obj, &youmonst, obj->oclass == SPBOOK_CLASS && !(obj->speflags & SPEFLAGS_SERVICED_SPELL) ? &youmonst : (struct monst*)0);

    switch (obj->otyp) {
    case WAN_STRIKING:
    case SPE_FORCE_BOLT:
    case SPE_FORCE_STRIKE:
        learn_it = TRUE;
        if (Magic_missile_immunity || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_REFLECTS);
            u_shieldeff();
            damage = 0;
            pline("Boing!");
        }
        else
        {
            if (ordinary)
            {
                play_sfx_sound(SFX_MAGIC_ARROW_HIT);
                You("bash yourself!");
            } 
            else
                basedmg = d(1 + obj->charges, 6);

            damage = adjust_damage(basedmg, &youmonst, &youmonst, AD_PHYS, ADFLAGS_SPELL_DAMAGE);
            exercise(A_STR, FALSE);
        }
        break;

    case SPE_MAGIC_ARROW:
    case SPE_ARROW_OF_DIANA:
        learn_it = TRUE;
        if (Magic_missile_immunity || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_REFLECTS);
            u_shieldeff();
            damage = 0;
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "Boing!");
        }
        else
        {
            if (ordinary)
            {
                play_sfx_sound(SFX_MAGIC_ARROW_HIT);
                You("shoot yourself with a magical arrow!");
            }
            else
                basedmg = d(1 + obj->charges, 3);

            damage = adjust_damage(basedmg, &youmonst, &youmonst, AD_PHYS, ADFLAGS_SPELL_DAMAGE);
            exercise(A_STR, FALSE);
        }
        break;
    case SPE_SHOCKING_TOUCH:
    case SPE_ELECTROCUTE:
        learn_it = TRUE;
        if (!Shock_immunity && !Invulnerable)
        {
            play_sfx_sound(SFX_MONSTER_GETS_ZAPPED);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "shock yourself!");
            exercise(A_CON, FALSE);
            damage = adjust_damage(d(1, 4), &youmonst, &youmonst, AD_ELEC, ADFLAGS_SPELL_DAMAGE);
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_WARNING, "shock yourself, but seem unharmed.");
            ugolemeffects(AD_ELEC, damage);
            damage = 0;
        }
        break;
    case SPE_BURNING_HANDS:
    case SPE_INCINERATE:
        learn_it = TRUE;
        if (!Fire_immunity && !Invulnerable)
        {
            play_sfx_sound(SFX_MONSTER_ON_FIRE);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "burn yourself!");
            exercise(A_CON, FALSE);
            damage = adjust_damage(d(1, 4), &youmonst, &youmonst, AD_FIRE, ADFLAGS_SPELL_DAMAGE);
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_WARNING, "burn yourself, but seem unharmed.");
            ugolemeffects(AD_FIRE, damage);
            damage = 0;
        }
        break;
    case SPE_HEAVENLY_TOUCH:
    case SPE_TOUCH_OF_DIVINITY:
        learn_it = TRUE;
        if ((is_demon(youmonst.data) || is_undead(youmonst.data)) && !Invulnerable)
        {
            play_sfx_sound(SFX_MONSTER_IS_HIT_WITH_CELESTIAL_MAGIC);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "sear yourself!");
            exercise(A_CON, FALSE);
            damage = adjust_damage(d(1, 4), &youmonst, &youmonst, AD_FIRE, ADFLAGS_SPELL_DAMAGE);
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_WARNING, "burn yourself, but seem unharmed.");
            ugolemeffects(AD_FIRE, damage);
            damage = 0;
        }
        break;
    case SPE_FREEZING_TOUCH:
    case SPE_GLACIAL_GRASP:
        learn_it = TRUE;
        if (!Cold_immunity && !Invulnerable) 
        {
            play_sfx_sound(SFX_MONSTER_COVERED_IN_FROST);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "freeze yourself!");
            exercise(A_CON, FALSE);
            damage = adjust_damage(d(1, 4), &youmonst, &youmonst, AD_COLD, ADFLAGS_SPELL_DAMAGE);
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_WARNING, "freeze yourself, but seem unharmed.");
            ugolemeffects(AD_COLD, damage);
            damage = 0;
        }
        break;    
    case SPE_THUNDERSTRIKE:
    case SPE_LIGHTNING_BOLT:
    case WAN_LIGHTNING:
        learn_it = TRUE;
        damage = adjust_damage(basedmg, &youmonst, &youmonst, AD_ELEC, ADFLAGS_SPELL_DAMAGE);
        if (!Shock_immunity && !Invulnerable)
        {
            play_sfx_sound(SFX_MONSTER_GETS_ZAPPED);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "shock yourself!");
            display_u_being_hit(HIT_ELECTROCUTED, 0, 0UL);
            exercise(A_CON, FALSE);
        } 
        else 
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_WARNING, "zap yourself, but seem unharmed.");
            ugolemeffects(AD_ELEC, damage);
            damage = 0;
        }
        destroy_item(WAND_CLASS, AD_ELEC);
        destroy_item(RING_CLASS, AD_ELEC);
        (void) flashburn((long) rnd(100));
        item_destruction_hint(AD_ELEC, TRUE);
        break;
    case SPE_FIREBALL:
    case SPE_FIRE_STORM:
    case SPE_METEOR_SWARM:
    case SPE_ICE_STORM:
    case SPE_MAGICAL_IMPLOSION:
    case SPE_MAGIC_STORM:
    case SPE_THUNDERSTORM:
    case SPE_CELESTIAL_STORM:
    case SPE_WRATH_OF_GOD:
    case SPE_DEATHSPELL:
    {
        int expl_type = obj->otyp == SPE_METEOR_SWARM ? EXPL_METEOR_SWARM : objects[obj->otyp].oc_damagetype == AD_FIRE ? EXPL_FIERY : objects[obj->otyp].oc_damagetype == AD_COLD ? EXPL_FROSTY : EXPL_MAGICAL;
        You("conjure %s on top of yourself!", OBJ_CONTENT_NAME(obj->otyp) ? an(OBJ_CONTENT_NAME(obj->otyp)) : an(OBJ_NAME(objects[obj->otyp])));
        explode(u.ux, u.uy, objects[obj->otyp].oc_dir_subtype, &youmonst, objects[obj->otyp].oc_spell_dmg_dice, objects[obj->otyp].oc_spell_dmg_diesize, objects[obj->otyp].oc_spell_dmg_plus, obj->otyp, obj->oclass, expl_type);
        break;
    }
    case SPE_FLAME_BURST:
    case SPE_FIRE_BOLT:
    case WAN_FIRE:
    case FIRE_HORN:
        learn_it = TRUE;
        damage = adjust_damage(basedmg, &youmonst, &youmonst, AD_FIRE, ADFLAGS_SPELL_DAMAGE);
        if (Fire_immunity || Invulnerable) 
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_feel_ex(ATR_NONE, CLR_MSG_SUCCESS, "rather warm.");
            ugolemeffects(AD_FIRE, damage);
            damage = 0;
        }
        else 
        {
            play_sfx_sound(SFX_MONSTER_ON_FIRE);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "You've set yourself afire!");
            display_u_being_hit(HIT_ON_FIRE, 0, 0UL);
        }
        burn_away_slime();
        (void) burnarmor(&youmonst);
        destroy_item(SCROLL_CLASS, AD_FIRE);
        destroy_item(POTION_CLASS, AD_FIRE);
        destroy_item(SPBOOK_CLASS, AD_FIRE);
        destroy_item(FOOD_CLASS, AD_FIRE); /* only slime for now */
        item_destruction_hint(AD_FIRE, TRUE);
        break;

    case SPE_LOWER_WATER:
    case WAN_EVAPORATION:
        if (is_watery(youmonst.data))
        {
            if (!Upolyd || Unchanging)
            {
                learn_it = TRUE;
                Sprintf(killer.name, "shot %sself with an evaporation ray", uhim());
                killer.format = NO_KILLER_PREFIX;
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "irradiate yourself with evaporating energy!");
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "die.");
                /* They might survive with an amulet of life saving */
                done(DIED);
            }
            else
                rehumanize();
        }
        destroy_item(POTION_CLASS, AD_FIRE);
        destroy_item(POTION_CLASS, AD_FIRE);
        break;
    case SPE_GREATER_FROST_RAY:
    case WAN_COLD:
    case SPE_CONE_OF_COLD:
    case FROST_HORN:
        learn_it = TRUE;
        damage = adjust_damage(basedmg, &youmonst, &youmonst, AD_COLD, ADFLAGS_SPELL_DAMAGE);
        if (Cold_immunity || Invulnerable) 
        {
            u_shieldeff();
            You_feel("a little chill.");
            ugolemeffects(AD_COLD, damage);
            damage = 0;
        } else {
            play_sfx_sound(SFX_MONSTER_COVERED_IN_FROST);
            You("imitate a popsicle!");
            display_u_being_hit(HIT_FROZEN, 0, 0UL);
        }
        destroy_item(POTION_CLASS, AD_COLD);
        item_destruction_hint(AD_COLD, TRUE);
        break;

    case WAN_MAGIC_MISSILE:
    case SPE_MAGIC_MISSILE:
    case SPE_GREATER_MAGIC_MISSILE:
        learn_it = TRUE;
        damage = adjust_damage(basedmg, &youmonst, &youmonst, AD_MAGM, ADFLAGS_SPELL_DAMAGE);
        if (Magic_missile_immunity || Invulnerable) 
        {
            play_sfx_sound(SFX_GENERAL_REFLECTS);
            u_shieldeff();
            pline_The_ex(ATR_NONE, CLR_MSG_SUCCESS, "missiles bounce!");
            damage = 0;
        } else {
            play_sfx_sound(SFX_MAGIC_ARROW_HIT);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Idiot!  You've shot yourself!");
            display_u_being_hit(HIT_GENERAL, 0, 0UL);
        }
        break;

    case WAN_POLYMORPH:
    case SPE_POLYMORPH:
        damage = 0;
        if (!Unchanging) {
            learn_it = TRUE;
            polyself(0);
        }
        break;

    case WAN_CANCELLATION:
    case SPE_CANCELLATION:
    case WAN_DISJUNCTION:
    case SPE_DISJUNCTION:
        damage = 0;
        if (Upolyd)
            rehumanize();
        (void) cancel_monst(&youmonst, obj, TRUE, TRUE, TRUE, duration);
        if (obj->otyp != SPE_DISJUNCTION && obj->otyp != WAN_DISJUNCTION)
        {
            set_itimeout(&HCancellation_resistance, max(HCancellation_resistance & TIMEOUT, 10));
            refresh_u_tile_gui_info(TRUE);
        }
        break;

    case SPE_LOWER_MAGIC_RESISTANCE:
    case SPE_DIMINISH_MAGIC_RESISTANCE:
    case SPE_ABOLISH_MAGIC_RESISTANCE:
    case SPE_NEGATE_MAGIC_RESISTANCE:
    case SPE_FORBID_SUMMONING:
        (void)add_temporary_property(&youmonst, obj, TRUE, TRUE, TRUE, d(objects[obj->otyp].oc_spell_dur_dice, objects[obj->otyp].oc_spell_dur_diesize) + objects[obj->otyp].oc_spell_dur_plus);
        break;
    
    case SPE_DRAIN_LEVEL:
        if (!Drain_resistance) {
            learn_it = TRUE; /* (no effect for spells...) */
            losexp("life drainage");
            display_u_being_hit(HIT_DRAIN_LEVEL, 0, 0UL);
        }
        damage = 0; /* No additional damage */
        break;

    case WAN_MAKE_INVISIBLE: 
    {
        damage = 0;
        /* have to test before changing HInvis but must change
         * HInvis before doing newsym().
         */
        int msg = !Invis && !Blind && !Blocks_Invisibility;

        struct obj* block_otmp = what_gives_monster(&youmonst, BLOCKS_INVISIBILITY);

        if (block_otmp && block_otmp->oclass == ARMOR_CLASS) 
        {
            /* A mummy wrapping absorbs it and protects you */
            You_feel("rather itchy under %s.", yname(uarmo));
        }

        incr_itimeout(&HInvis, duration);
        refresh_u_tile_gui_info(TRUE);

#if 0
        if (ordinary || !rn2(10))
        { /* permanent */
            HInvis |= FROM_ACQUIRED;
        } else { /* temporary */
            incr_itimeout(&HInvis, d(obj->charges, 250));
            refresh_u_tile_gui_info(TRUE);
        }
#endif

        if (msg) {
            learn_it = TRUE;
            newsym(u.ux, u.uy);
            self_invis_message();
        }
        break;
    }

    case WAN_SPEED_MONSTER:
    {
        damage = 0;
        //boolean was_fast = Fast;
        boolean was_very_fast = Very_fast;
        incr_itimeout(&HVery_fast, duration);
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
        if (Very_fast && !was_very_fast && !Ultra_fast && !Super_fast && !Lightning_fast)
        {
            learn_it = TRUE;
            play_sfx_sound(SFX_ACQUIRE_HASTE);
            You("speed up.");
            exercise(A_DEX, TRUE);
        }
        break;
    }
    case WAN_SLEEP:
    case SPE_SLEEP:
        damage = 0;
        learn_it = TRUE;
        if (Sleep_resistance) 
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "don't feel sleepy!");
        }
        else 
        {
            play_sfx_sound(SFX_ACQUIRE_SLEEP);
            pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "sleep ray hits you!");
            fall_asleep(-duration, TRUE);
        }
        break;

    case WAN_SLOW_MONSTER:
    case SPE_SLOW_MONSTER:
    case SPE_MASS_SLOW:
        damage = 0;
        boolean was_slowed = Slowed;
        incr_itimeout(&HSlowed, duration);
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
        if (Slowed && !was_slowed)
        {
            learn_it = TRUE;
            play_sfx_sound(SFX_ACQUIRE_SLOW);
            You("slow down.");
        }
        //u_slow_down();
        break;
    case SPE_HASTE_MONSTER:
        damage = 0;
        boolean was_very_fast = Very_fast;
        incr_itimeout(&HVery_fast, obj->oclass == WAND_CLASS ? rn1(10, 100 + 60 * bcsign(obj)) : duration);
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
        if (Very_fast && !was_very_fast && !Ultra_fast && !Super_fast && !Lightning_fast)
        {
            learn_it = TRUE;
            play_sfx_sound(SFX_ACQUIRE_HASTE);
            You("speed up.");
        }
        break;
    case SPE_HOLD_MONSTER:
    case SPE_MASS_HOLD:
        damage = 0;
        if (!check_ability_resistance_success(&youmonst, A_WIS, save_adj))
        {
            boolean was_paralyzed = Paralyzed_or_immobile;
            incr_itimeout(&HParalyzed, duration);
            context.botl = context.botlx = TRUE;
            refresh_u_tile_gui_info(TRUE);
            if (Paralyzed_or_immobile && !was_paralyzed)
            {
                learn_it = TRUE;
                play_sfx_sound(SFX_ACQUIRE_PARALYSIS);
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are paralyzed!");
            }
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "resist!");
        }
        break;
    case SPE_HOLD_UNDEAD:
        damage = 0;
        if (!check_ability_resistance_success(&youmonst, A_WIS, save_adj))
        {
            boolean was_paralyzed = Paralyzed_or_immobile;
            incr_itimeout(&HUndead_immobility, duration);
            context.botl = context.botlx = TRUE;
            refresh_u_tile_gui_info(TRUE);
            if (Paralyzed_or_immobile && !was_paralyzed)
            {
                learn_it = TRUE;
                play_sfx_sound(SFX_ACQUIRE_PARALYSIS);
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are paralyzed!");
            }
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "resist!");
        }
        break;
    case SPE_SILENCE:
        damage = 0;
        boolean was_silenced = Silenced;
        incr_itimeout(&HSilenced, duration);
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
        if (Silenced && !was_silenced)
        {
            learn_it = TRUE;
            play_sfx_sound(SFX_ACQUIRE_SILENCE);
            Your("voice disappears.");
        }
        break;
    case WAN_TELEPORTATION:
    case SPE_TELEPORT_MONSTER:
        damage = 0;
        tele();
        /* same criteria as when mounted (zap_steed) */
        if ((Teleport_control && !Stunned) || !couldsee(u.ux0, u.uy0)
            || distu(u.ux0, u.uy0) >= 16)
            learn_it = TRUE;
        break;

    case WAN_DEATH:
    case SPE_FINGER_OF_DEATH:
    case SPE_TOUCH_OF_DEATH:
    case SPE_POWER_WORD_KILL:
        damage = 0;
        if (is_not_living(youmonst.data) || is_demon(youmonst.data) || Death_resistance) // || magic_resistance_success
        {
            pline((obj->otyp == WAN_DEATH)
                      ? "The wand shoots an apparently harmless beam at you."
                      : "You seem no deader than before.");
            break;
        }
        play_sfx_sound(SFX_MONSTER_IS_HIT_WITH_DEATH_MAGIC);
        learn_it = TRUE;
        Sprintf(killer.name, "shot %sself with a death ray", uhim());
        killer.format = NO_KILLER_PREFIX;
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "irradiate yourself with pure energy!");
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "die.");
        /* They might survive with an amulet of life saving */
        done(DIED);
        break;
    case SPE_TOUCH_OF_PETRIFICATION:
    case SPE_FLESH_TO_STONE:
    case WAN_PETRIFICATION:
        if (!Stoned && !Stone_resistance
            && !(poly_when_stoned(youmonst.data)
                && polymon(PM_STONE_GOLEM))) {
            int kformat = NO_KILLER_PREFIX;
            char kname[BUFSZ] = "";
            Sprintf(kname, "casting a petrification spell on %sself", uhim());
            make_stoned(5L, (char*)0, kformat, kname, HINT_KILLED_PETRIFICATION);
        }
        break;
    case SPE_GAZE_OF_PETRIFICATION:
        You("try to gaze at yourself but cannot!");
        break;
    case SPE_POWER_WORD_STUN:
    {
        if (!Stunned)
            play_sfx_sound(SFX_ACQUIRE_STUN);
        make_stunned((HStun & TIMEOUT) + duration, TRUE);
        break;
    }
    case SPE_POWER_WORD_BLIND:
    {
        if (!Blinded)
            play_sfx_sound(SFX_ACQUIRE_BLINDNESS);
        make_blinded((Blinded & TIMEOUT) + duration, (boolean)!Blind);
        break;
    }
    case SPE_SUNLIGHT_BEAM:
    case SPE_RAY_OF_RADIANCE:
        learn_it = TRUE;
        damage = adjust_damage(basedmg, &youmonst, &youmonst, objects[obj->otyp].oc_damagetype, ADFLAGS_SPELL_DAMAGE);
        if (is_undead(youmonst.data) || is_demon(youmonst.data) || hates_light(youmonst.data)) {
            pline("Idiot!  You've shot yourself with a %s!", OBJ_NAME(objects[obj->otyp]));
        }
        else {
            pline_The("%s has no effect on you.", OBJ_NAME(objects[obj->otyp]));
            damage = 0;
        }
        break;
    case SPE_DISINTEGRATE:
    case WAN_DISINTEGRATION:
        damage = 0;
        if (Disint_resistance || is_incorporeal(youmonst.data) || Invulnerable)
        {
            pline((obj->otyp == WAN_DISINTEGRATION)
                ? "The wand shoots an apparently harmless beam at you."
                : "You seem to exist as you did before.");
            break;
        }
        else if (uarms) {
            /* destroy shield; other possessions are safe */
            (void)destroy_arm(uarms);
            break;
        }
        else if (uarm) {
            /* destroy suit; if present, cloak and robe go too */
            if (uarmc)
                (void)destroy_arm(uarmc);
            if (uarmo)
                (void)destroy_arm(uarmo);
            (void)destroy_arm(uarm);
            break;
        }
        /* no shield or suit, you're dead; wipe out cloak
            and/or shirt in case of life-saving or bones */
        if (uarmc)
            (void)destroy_arm(uarmc);
        if (uarmo)
            (void)destroy_arm(uarmo);
        if (uarmu)
            (void)destroy_arm(uarmu);
        
        learn_it = TRUE;
        Sprintf(killer.name, "shot %sself with a disintegration ray", uhim());
        killer.format = NO_KILLER_PREFIX;
        play_sfx_sound(SFX_DISINTEGRATE);
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "irradiate yourself with disintegration field!");
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are disintegrated.");
        display_u_being_hit(HIT_DISINTEGRATED, 0, 0UL);
        /* when killed by disintegration breath, don't leave corpse */
        u.ugrave_arise = 3;
        done(DISINTEGRATION);
        break;
    case WAN_UNDEAD_TURNING:
    case SPE_TURN_UNDEAD:
        damage = 0;
        learn_it = TRUE;
        if (is_undead(youmonst.data)) {
            You_feel("frightened and %sstunned.",
                     Stunned ? "even more " : "");
            make_stunned((HStun & TIMEOUT) + (long) rnd(30), FALSE); //Not strictly the same effect, so keep hard coding for the time being
        } else
            You("don't feel much different than you did before.");
        break;
    case SPE_FEAR:
        if(!Fear_resistance)
            You("shudder in dread.");
        break;
    case WAN_RESURRECTION:
    case SPE_RESURRECTION:
        damage = 0;
        learn_it = TRUE;
        (void)revive_from_inventory(&youmonst);
        break;
    case JAR_OF_EXTRA_HEALING_SALVE:
    case JAR_OF_GREATER_HEALING_SALVE:
    case JAR_OF_PRODIGIOUS_HEALING_SALVE:
    case SPE_MINOR_HEALING:
    case SPE_HEALING:
    case SPE_EXTRA_HEALING:
    case SPE_GREATER_HEALING:
    case SPE_PRODIGIOUS_HEALING:
        if(is_living(youmonst.data))
        {
            //learn_it = TRUE; /* (no effect for spells...) */
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_HEALING);
            special_effect_wait_until_action(0);
            healup(basedmg, 0,
                obj->otyp == GRAIL_OF_HEALING, 
                (obj->blessed || (obj->otyp != SPE_EXTRA_HEALING && obj->otyp != SPE_HEALING && obj->otyp != SPE_MINOR_HEALING)),
                (obj->blessed || (obj->otyp != SPE_HEALING && obj->otyp != SPE_MINOR_HEALING)), 
                obj->otyp == GRAIL_OF_HEALING, 
                obj->otyp == GRAIL_OF_HEALING);
            
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "%sbetter.",
                obj->otyp == SPE_PRODIGIOUS_HEALING || obj->otyp == JAR_OF_PRODIGIOUS_HEALING_SALVE || obj->otyp == GRAIL_OF_HEALING ? "vastly " :
                obj->otyp == SPE_GREATER_HEALING || obj->otyp == JAR_OF_GREATER_HEALING_SALVE ? "much, much " :
                obj->otyp == SPE_EXTRA_HEALING || obj->otyp == JAR_OF_EXTRA_HEALING_SALVE ? "much " :
                "");

            if (obj->otyp == GRAIL_OF_HEALING)
            {
                if (Stoned)
                    fix_petrification();

                if (Slimed)
                    make_slimed(0L, (char*)0, 0, (char*)0, 0);
            }
            special_effect_wait_until_end(0);

        }
        else
            You_feel("no different than before.");
        damage = 0;
        break;
    case SPE_FULL_HEALING:
        if (is_living(youmonst.data))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_FULL_HEALING);
            special_effect_wait_until_action(0);
            learn_it = TRUE; /* (no effect for spells...) */
            healup(1000, 0, TRUE, TRUE, TRUE, TRUE, TRUE);
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "completely healed.");
            special_effect_wait_until_end(0);
        }
        else
            You_feel("no different than before.");


        damage = 0;
        break;
    case GRAIL_OF_HEALING:
        if (is_undead(youmonst.data) || is_demon(youmonst.data))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            special_effect_wait_until_action(0);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "The liquid burns inside you!");
            u.uen = 0;
            losehp(adjust_damage(d(48, 6), (struct monst*)0, &youmonst, AD_CLRC, ADFLAGS_NONE), "drinking from Holy Grail", KILLED_BY_AN);
            context.botl = 1;
            special_effect_wait_until_end(0);
        }
        else if (is_living(youmonst.data))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_FULL_HEALING);
            special_effect_wait_until_action(0);
            learn_it = TRUE;
            healup(basedmg, 0, TRUE, TRUE, TRUE, TRUE, TRUE);
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "truly completely healed.");

            if (u.ulycn >= LOW_PM || is_were(youmonst.data))
                you_unwere(TRUE);

            if(Stoned)
                make_stoned(0L, (char*)0, 0, (char*)0, 0);

            if (u.uhunger < 500)
            {
                int nut = 500 - u.uhunger;
                lesshungry(nut);
            }

            u.uen += basedmg / 2;
            if (u.uen > u.uenmax)
                u.uen = u.uenmax;
            else if (u.uen <= 0)
                u.uen = 0;
            context.botl = 1;
            special_effect_wait_until_end(0);
        }
        else
            You_feel("no different than before.");

        damage = 0;
        break;
    case SPE_CURE_BLINDNESS:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_CURE_AILMENT);
        special_effect_wait_until_action(0);
        healup(0, 0, FALSE, TRUE, FALSE, FALSE, FALSE);
        special_effect_wait_until_end(0);
        break;
    case JAR_OF_MEDICINAL_SALVE:
    case SPE_CURE_SICKNESS:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_CURE_DISEASE);
        special_effect_wait_until_action(0);
        if (Sick || FoodPoisoned || MummyRot)
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are no longer ill.");
        if (Slimed)
            make_slimed(0L, "The slime disappears!", 0, (char*)0, 0);
        healup(0, 0, TRUE, FALSE, FALSE, FALSE, FALSE);
        special_effect_wait_until_end(0);
        break;
    case SPE_CURE_PETRIFICATION:
        if (Stoned)
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            special_effect_wait_until_action(0);
            fix_petrification();
            special_effect_wait_until_end(0);
        }
        break;
    case SPE_REPLENISH_UNDEATH:
    case SPE_GREATER_UNDEATH_REPLENISHMENT:
        if (is_undead(youmonst.data))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_HEALING);
            special_effect_wait_until_action(0);
            learn_it = TRUE; /* (no effect for spells...) */
            healup(basedmg, 0, FALSE, (obj->blessed || obj->otyp == SPE_GREATER_UNDEATH_REPLENISHMENT), (obj->blessed || obj->otyp == SPE_GREATER_UNDEATH_REPLENISHMENT), FALSE, FALSE);
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "%sbetter.", obj->otyp == SPE_GREATER_UNDEATH_REPLENISHMENT ? "much " : "");
            special_effect_wait_until_end(0);
        }
        else
            You_feel("no different than before.");
        damage = 0;
        break;
    case WAN_LIGHT: /* (broken wand) */
        /* assert( !ordinary ); */
        damage = adjust_damage(d(obj->charges, 25), &youmonst, &youmonst, AD_ELEC, ADFLAGS_SPELL_DAMAGE);
        /*FALLTHRU*/
    case EXPENSIVE_CAMERA:
        if (basedmg == 0)
            basedmg = 5;
        damage = lightdamage(obj, ordinary, basedmg);
        damage += adjust_damage(rnd(25), &youmonst, &youmonst, AD_PHYS, ADFLAGS_NONE);
        if (flashburn((long)ceil(damage)))
            learn_it = TRUE;
        damage = 0; /* reset */
        break;
    case WAN_OPENING:
    case SPE_KNOCK:
        damage = 0;
        if (Punished) 
        {
            learn_it = TRUE;
            unpunish();
        }

        if (u.utrap)
        { /* escape web or bear trap */
            (void) openholdingtrap(&youmonst, &learn_it);
        } 
        else 
        {
            struct obj *otmp;
            /* unlock carried boxes */
            for (otmp = invent; otmp; otmp = otmp->nobj)
                if (Is_box(otmp))
                    (void) boxlock(otmp, obj);
            /* trigger previously escaped trapdoor */
            (void) openfallingtrap(&youmonst, TRUE, &learn_it);
        }

        break;
    case WAN_LOCKING:
    case SPE_WIZARD_LOCK:
        damage = 0;
        if (!u.utrap) 
        {
            (void) closeholdingtrap(&youmonst, &learn_it);
        }
        break;
    case WAN_DIGGING:
    case SPE_DIG:
    case SPE_DETECT_UNSEEN:
    case WAN_NOTHING:
        damage = 0;
        break;
    case SPE_PROBE:
    case WAN_PROBING: 
    {
        if (obj->otyp == WAN_PROBING && !objects[obj->otyp].oc_name_known)
            pline("This is a wand of probing.");

        damage = 0;
        struct obj *otmp = level.objects[u.ux][u.uy];
        int cnt = 0;
        for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
            cnt++;

        learn_it = TRUE;
        otmp = level.objects[u.ux][u.uy];
        boolean effectfinished = FALSE;
        while(otmp)
        {
            char qbuf[BUFSZ];
            Sprintf(qbuf, "There is %s here. Probe it?", acxname(otmp));
            int ans = cnt > 1 ? ynq(qbuf) : yn_query(qbuf);
            switch (ans)
            {
            case 'y':
                probe_object(otmp);
                effectfinished = TRUE;
                otmp = 0;
                break;
            case 'q':
                otmp = 0;
                break;
            default:
            case 'n':
                otmp = otmp->nexthere;
                break;
            }
        }

        if (effectfinished)
            break;

        for (otmp = invent; otmp; otmp = otmp->nobj) 
        {
            otmp->dknown = 1;
            if (Is_container(otmp) || otmp->otyp == STATUE) 
            {
                otmp->lknown = otmp->tknown = 1;
                if (!SchroedingersBox(otmp))
                    otmp->cknown = 1;
            }
        }
        ustatusline();
        enlightenment(MAGICENLIGHTENMENT, ENL_GAMEINPROGRESS);
        break;
    }
    case JAR_OF_BASILISK_BLOOD:
    case SPE_STONE_TO_FLESH: 
    {
        damage = 0;
        //struct obj *otmp, *onxt;
        //boolean didmerge;

        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);

        if (u.umonnum == PM_STONE_GOLEM) 
        {
            learn_it = TRUE;
            (void) polymon(PM_FLESH_GOLEM);
        }

        if (Stoned)
        {
            learn_it = TRUE;
            fix_petrification(); /* saved! */
        }
#if 0
        /* but at a cost.. */
        for (otmp = invent; otmp; otmp = onxt) 
        {
            onxt = otmp->nobj;
            if (bhito(otmp, obj, &youmonst))
                learn_it = TRUE;
        }
        /*
         * It is possible that we can now merge some inventory.
         * Do a highly paranoid merge.  Restart from the beginning
         * until no merges.
         */
        do
        {
            didmerge = FALSE;
            for (otmp = invent; !didmerge && otmp; otmp = otmp->nobj)
                for (onxt = otmp->nobj; onxt; onxt = onxt->nobj)
                    if (merged(&otmp, &onxt))
                    {
                        didmerge = TRUE;
                        break;
                    }
        } while (didmerge);
#endif

        special_effect_wait_until_end(0);

        break;
    }
    default:
        /* All other spells do not do anything */
        //impossible("zapyourself: object %d used?", obj->otyp);
        break;
    }
    /* if effect was observable then discover the wand type provided
       that the wand itself has been seen */
    if (learn_it)
        learnwand(obj);
    return damage;
}

/* called when poly'd hero uses breath attack against self */
void
ubreatheu(mattk)
struct attack *mattk;
{
    uchar adtyp = mattk->adtyp;
    int typ = get_ray_adtyp(adtyp);

    int dtyp = 20 + typ - 1;      /* breath by hero */
    const char *fltxt = flash_types[dtyp]; /* blast of <something> */

    zhitu(dtyp, (struct obj*)0, &youmonst, mattk->damn, mattk->damd, mattk->damp, fltxt);
}

/* light damages hero in gremlin form */
double
lightdamage(obj, ordinary, amt)
struct obj *obj;  /* item making light (fake book if spell) */
boolean ordinary; /* wand/camera zap vs wand destruction */
int amt;          /* pseudo-damage used to determine blindness duration */
{
    char buf[BUFSZ];
    const char *how;
    int dmg = amt;
    double damage = 0;

    if (dmg && youmonst.data == &mons[PM_GREMLIN]) 
    {
        /* reduce high values (from destruction of wand with many charges) */
        dmg = rnd(dmg);
        if (dmg > 10)
            dmg = 10 + rnd(dmg - 10);
        if (dmg > 20)
            dmg = 20;
        pline("Ow, that light hurts%c", (dmg > 2 || u.mh <= 5) ? '!' : '.');
        /* [composing killer/reason is superfluous here; if fatal, cause
           of death will always be "killed while stuck in creature form"] */
        if (obj->oclass == SCROLL_CLASS || obj->oclass == SPBOOK_CLASS)
            ordinary = FALSE; /* say blasted rather than zapped */
        how = (obj->oclass != SPBOOK_CLASS)
                  ? (const char *) ansimpleoname(obj)
                  : "spell of light";
        Sprintf(buf, "%s %sself with %s", ordinary ? "zapped" : "blasted",
                uhim(), how);
        /* might rehumanize(); could be fatal, but only for Unchanging */
        damage = adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE);
        losehp(damage, buf, NO_KILLER_PREFIX);
    }
    return damage;
}

/* light[ning] causes blindness */
boolean
flashburn(duration)
long duration;
{
    if (!resists_blnd(&youmonst) && !Flash_resistance) {
        if (!Blinded)
            play_sfx_sound(SFX_ACQUIRE_BLINDNESS);
        You(are_blinded_by_the_flash);
        make_blinded(duration, FALSE);
        if (!Blind)
            Your1(vision_clears);
        return TRUE;
    }
    return FALSE;
}

/* you've zapped a wand downwards while riding
 * Return TRUE if the steed was hit by the wand.
 * Return FALSE if the steed was not hit by the wand.
 */
STATIC_OVL boolean
zap_steed(obj)
struct obj *obj; /* wand or spell */
{
    int steedhit = FALSE;

    bhitpos.x = u.usteed->mx, bhitpos.y = u.usteed->my;
    notonhead = FALSE;
    switch (obj->otyp) {
    /*
     * Wands that are allowed to hit the steed
     * Carefully test the results of any that are
     * moved here from the bottom section.
     */
    case WAN_PROBING:
    case SPE_PROBE:
        probe_monster(u.usteed);
        learnwand(obj);
        steedhit = TRUE;
        break;
    case WAN_TELEPORTATION:
    case SPE_TELEPORT_MONSTER:
        /* you go together */
        tele();
        /* same criteria as when unmounted (zapyourself) */
        if ((Teleport_control && !Stunned) || !couldsee(u.ux0, u.uy0)
            || distu(u.ux0, u.uy0) >= 16)
            learnwand(obj);
        steedhit = TRUE;
        break;

    /* Default processing via bhitm() for these */
    case WAN_MAKE_INVISIBLE:
    case WAN_CANCELLATION:
    case SPE_CANCELLATION:
    case WAN_DISJUNCTION:
    case SPE_DISJUNCTION:
    case SPE_LOWER_MAGIC_RESISTANCE:
    case SPE_DIMINISH_MAGIC_RESISTANCE:
    case SPE_ABOLISH_MAGIC_RESISTANCE:
    case SPE_NEGATE_MAGIC_RESISTANCE:
    case SPE_FORBID_SUMMONING:
    case WAN_POLYMORPH:
    case SPE_POLYMORPH:
    case WAN_STRIKING:
    case SPE_FORCE_BOLT:
    case SPE_FORCE_STRIKE:
    case SPE_SHOCKING_TOUCH:
    case SPE_BURNING_HANDS:
    case SPE_FREEZING_TOUCH:
    case SPE_INCINERATE:
    case SPE_ELECTROCUTE:
    case SPE_GLACIAL_GRASP:
    case SPE_HEAVENLY_TOUCH:
    case SPE_TOUCH_OF_DIVINITY:
    case SPE_TOUCH_OF_DEATH:
    case SPE_TOUCH_OF_PETRIFICATION:
    case SPE_FLESH_TO_STONE:
    case SPE_GAZE_OF_PETRIFICATION:
    case SPE_POWER_WORD_KILL:
    case SPE_POWER_WORD_STUN:
    case SPE_POWER_WORD_BLIND:
    case SPE_SUNLIGHT_BEAM:
    case SPE_RAY_OF_RADIANCE:
    case SPE_MAGIC_ARROW:
    case SPE_ARROW_OF_DIANA:
    case WAN_SLOW_MONSTER:
    case SPE_SLOW_MONSTER:
    case SPE_MASS_SLOW:
    case SPE_HASTE_MONSTER:
    case SPE_HOLD_MONSTER:
    case SPE_MASS_HOLD:
    case SPE_HOLD_UNDEAD:
    case WAN_SPEED_MONSTER:
    case SPE_SILENCE:
    case SPE_CURE_BLINDNESS:
    case SPE_CURE_SICKNESS:
    case SPE_CURE_PETRIFICATION:
    case SPE_MINOR_HEALING:
    case JAR_OF_MEDICINAL_SALVE:
    case JAR_OF_EXTRA_HEALING_SALVE:
    case JAR_OF_GREATER_HEALING_SALVE:
    case JAR_OF_PRODIGIOUS_HEALING_SALVE:
    case JAR_OF_BASILISK_BLOOD:
    case GRAIL_OF_HEALING:
    case SPE_HEALING:
    case SPE_EXTRA_HEALING:
    case SPE_GREATER_HEALING:
    case SPE_PRODIGIOUS_HEALING:
    case SPE_FULL_HEALING:
    case SPE_REPLENISH_UNDEATH:
    case SPE_GREATER_UNDEATH_REPLENISHMENT:
    case SPE_DRAIN_LEVEL:
    case WAN_OPENING:
    case SPE_KNOCK:
        (void) bhitm(u.usteed, obj, &youmonst);
        steedhit = TRUE;
        break;

    default:
        steedhit = FALSE;
        break;
    }
    return steedhit;
}

/*
 * cancel a monster (possibly the hero).  inventory is cancelled only
 * if the monster is zapping itself directly, since otherwise the
 * effect is too strong.  currently non-hero monsters do not zap
 * themselves with cancellation.
 */
boolean
cancel_monst(mdef, obj, youattack, allow_cancel_kill, self_cancel, duration)
register struct monst *mdef;
register struct obj *obj;
boolean youattack, allow_cancel_kill, self_cancel;
int duration;
{
    boolean youdefend = (mdef == &youmonst);
    static const char writing_vanishes[] =
        "Some writing vanishes from %s head!";
    static const char your[] = "your"; /* should be extern */

    if (youdefend ? (!youattack && Antimagic_or_resistance)
                  : check_magic_resistance_and_inflict_damage(mdef, obj, youattack ? &youmonst : (struct monst*)0, FALSE, 0, 0, NOTELL))
        return FALSE; /* resisted cancellation */

    if (self_cancel || (youattack && !youdefend && is_tame(mdef)) || (!youattack && !youdefend && !is_peaceful(mdef)))
    {
        /* 1st cancel inventory */
        struct obj* otmp;
        for (otmp = (youdefend ? invent : mdef->minvent); otmp;
             otmp = otmp->nobj)
            cancel_item(otmp, FALSE);
        if (youdefend) {
            context.botl = 1; /* potential AC change */
            find_ac();
            find_mc();
            update_inventory();
        }
    }

    /* now handle special cases */
    if (youdefend) 
    {
        play_sfx_sound(SFX_ACQUIRE_CANCELLATION);
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "your magic is not flowing properly.");

        /* Remove all buffs */
        boolean was_flying = !!Flying;
        for (int i = 1; i < MAX_PROPS; i++)
        {
            if ((u.uprops[i].intrinsic & TIMEOUT) && (property_definitions[i].pflags & PROPFLAGS_BUFF_CANCELLABLE))
            {
                u.uprops[i].intrinsic &= ~TIMEOUT;
                property_expiry_message(i, was_flying);
            }
        }


        /* Add cancellation debuff */
        incr_itimeout(&HCancelled, duration);
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);

        if (Upolyd) { /* includes lycanthrope in creature form */
            /*
             * Return to normal form unless Unchanging.
             * Hero in clay golem form dies if Unchanging.
             * Does not cure lycanthropy or stop timed random polymorph.
             */
            if (u.umonnum == PM_CLAY_GOLEM) {
                if (!Blind)
                    pline(writing_vanishes, your);
                else /* note: "dark" rather than "heavy" is intentional... */
                    You_feel("%s headed.", Hallucination ? "dark" : "light");
                u.mh = 0; /* fatal; death handled by rehumanize() */
            }
            if (Unchanging && u.mh > 0)
                Your("amulet grows hot for a moment, then cools.");
            else
                rehumanize();
        }
    } 
    else 
    {
        boolean viseffect = increase_mon_property_b(mdef, CANCELLED, duration, TRUE);
        break_charm(mdef, TRUE);
        for (int i = 1; i < MAX_PROPS; i++)
        {
            if((mdef->mprops[i] & M_TIMEOUT) && (property_definitions[i].pflags & PROPFLAGS_BUFF_CANCELLABLE))
                set_mon_property_verbosely(mdef, i, 0);
        }

        if (viseffect && obj && obj->oclass == WAND_CLASS)
            makeknown(obj->otyp);

        /* force shapeshifter into its base form */
        if (M_AP_TYPE(mdef) != M_AP_NOTHING)
            seemimic(mdef);
        /* [not 'else if'; chameleon might have been hiding as a mimic] */
        if (mdef->cham >= LOW_PM) {
            /* note: newcham() uncancels shapechangers (resets m->mcancelled
               to 0), but only for shapechangers whose m->cham is already
               to 0), but only for shapechangers whose m->cham is already
               NON_PM and we just verified that it's LOW_PM or higher */
            newcham(mdef, &mons[mdef->cham], mdef->cham_subtype, FALSE, FALSE);
            mdef->cham = NON_PM; /* cancelled shapeshifter can't shift */
            mdef->cham_subtype = 0;
        }
        if (is_were(mdef->data) && !is_human(mdef->data))
            were_change(mdef);

        if (mdef->data == &mons[PM_CLAY_GOLEM]) {
            if (canseemon(mdef))
                pline(writing_vanishes, s_suffix(mon_nam(mdef)));
            /* !allow_cancel_kill is for Magicbane, where clay golem
               will be killed somewhere back up the call/return chain... */
            if (allow_cancel_kill) {
                if (youattack)
                    killed(mdef);
                else
                    monkilled(mdef, "", AD_SPEL, 0);
            }
        }
    }
    return TRUE;
}

boolean
add_temporary_property(mdef, obj, youattack, allow_cancel_kill, self_cancel, duration)
register struct monst* mdef;
register struct obj* obj;
boolean youattack, allow_cancel_kill, self_cancel;
int duration;
{
    boolean youdefend = (mdef == &youmonst);

    if (!obj)
        return FALSE;

    if (allow_cancel_kill || self_cancel)
    {
        /* Do nothing, these seem obsolete */
    }

    if (youdefend)
    {
        if (objects[obj->otyp].oc_spell_flags & S1_SPELL_BYPASSES_MAGIC_RESISTANCE)
            ; //OK;
        else if (!youattack && Antimagic_or_resistance)
            return FALSE; /* resisted cancellation */
            
        pline("A dim shimmer surrounds you.");
    }
    else
    {
        if (objects[obj->otyp].oc_spell_flags & S1_SPELL_BYPASSES_MAGIC_RESISTANCE)
            ; //OK;
        else if (check_magic_resistance_and_inflict_damage(mdef, obj, youattack ? &youmonst : (struct monst*)0, FALSE, 0, 0, TELL))
            return FALSE;

        pline("A dim shimmer surrounds %s.", mon_nam(mdef));
    }

    int prop = 0;
    switch (obj->otyp)
    {
    case SPE_LOWER_MAGIC_RESISTANCE:
        prop = THREE_FOURTHS_MAGIC_RESISTANCE;
        break;
    case SPE_DIMINISH_MAGIC_RESISTANCE:
        prop = HALVED_MAGIC_RESISTANCE;
        break;
    case SPE_ABOLISH_MAGIC_RESISTANCE:
        prop = ONE_FOURTH_MAGIC_RESISTANCE;
        break;
    case SPE_NEGATE_MAGIC_RESISTANCE:
        prop = NO_MAGIC_RESISTANCE;
        break;
    case SPE_FORBID_SUMMONING:
        prop = SUMMON_FORBIDDEN;
        break;
    default:
        break;
    }

    if (prop > 0)
    {
        increase_mon_property(mdef, prop, duration);
    }
        
    return TRUE;
}

/* you've zapped an immediate type wand up or down */
STATIC_OVL boolean
zap_updown(obj)
struct obj *obj; /* wand or spell */
{
    boolean striking = FALSE, disclose = FALSE;
    int x, y, xx, yy, ptmp;
    struct obj *otmp;
    struct engr *e;
    struct trap *ttmp;
    char buf[BUFSZ];
    context.bhitcount = 0;
    uchar hit_only_one = 1;
    if (objects[obj->otyp].oc_dir == IMMEDIATE_MULTIPLE_TARGETS)
        hit_only_one = 0;
    else if (objects[obj->otyp].oc_dir == IMMEDIATE_ONE_TO_THREE_TARGETS)
        hit_only_one = 2; /* 1- 3 targets based on BUC status */
    else if (objects[obj->otyp].oc_dir == IMMEDIATE_ONE_TO_SEVEN_TARGETS)
        hit_only_one = 3; /* 1- 7 targets based on BUC status */
    else if (objects[obj->otyp].oc_dir == IMMEDIATE_TWO_TO_SIX_TARGETS)
        hit_only_one = 4; /* 2- 6 targets based on BUC status */

    /* some wands have special effects other than normal bhitpile */
    /* drawbridge might change <u.ux,u.uy> */
    x = xx = u.ux;     /* <x,y> is zap location */
    y = yy = u.uy;     /* <xx,yy> is drawbridge (portcullis) position */
    ttmp = t_at(x, y); /* trap if there is one */

    switch (obj->otyp) {
    case WAN_PROBING:
    case SPE_PROBE:
        ptmp = 0;
        if (u.dz < 0) {
            You("probe towards the %s.", ceiling(x, y));
        } else {
            ptmp += bhitpile(obj, &youmonst, bhito, x, y, u.dz, hit_only_one, TRUE);
            You("probe beneath the %s.", surface(x, y));
            ptmp += display_binventory(x, y, TRUE);
        }
        if (!ptmp)
            Your("probe reveals nothing.");
        return TRUE; /* we've done our own bhitpile */
    case WAN_OPENING:
    case SPE_KNOCK:
        /* up or down, but at closed portcullis only */
        if (is_db_wall(x, y) && find_drawbridge(&xx, &yy)) {
            open_drawbridge(xx, yy, TRUE);
            disclose = TRUE;
        } else if (u.dz > 0 && (x == xdnstair && y == ydnstair)
                   /* can't use the stairs down to quest level 2 until
                      leader "unlocks" them; give feedback if you try */
                   && on_level(&u.uz, &qstart_level) && !ok_to_quest()) {
            pline_The("stairs seem to ripple momentarily.");
            disclose = TRUE;
        }
        /* down will release you from bear trap or web */
        if (u.dz > 0 && u.utrap) {
            (void) openholdingtrap(&youmonst, &disclose);
            /* down will trigger trapdoor, hole, or [spiked-] pit */
        } else if (u.dz > 0 && !u.utrap) {
            (void) openfallingtrap(&youmonst, FALSE, &disclose);
        }
        break;
    case WAN_STRIKING:
    case SPE_FORCE_BOLT:
    case SPE_FORCE_STRIKE:
        striking = TRUE;
        /*FALLTHRU*/
    case WAN_LOCKING:
    case SPE_WIZARD_LOCK:
        /* down at open bridge or up or down at open portcullis */
        if (((levl[x][y].typ == DRAWBRIDGE_DOWN)
                 ? (u.dz > 0)
                 : (is_drawbridge_wall(x, y) >= 0 && !is_db_wall(x, y)))
            && find_drawbridge(&xx, &yy)) 
        {
            if (!striking)
                maybe_close_drawbridge(xx, yy, TRUE);
            else
                destroy_drawbridge(xx, yy, FALSE);
            disclose = TRUE;
        } 
        else if (striking && u.dz < 0 && rn2(3) && !Is_airlevel(&u.uz)
                   && !Is_waterlevel(&u.uz) && !Underwater
                   && !Is_qstart(&u.uz)) {
            int dmg;
            /* similar to zap_dig() */
            pline("A rock is dislodged from the %s and falls on your %s.",
                  ceiling(x, y), body_part(HEAD));
            dmg = rnd((uarmh && is_metallic(uarmh)) ? 2 : 6);
            losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "falling rock", KILLED_BY_AN);
            if ((otmp = mksobj_at(ROCK, x, y, FALSE, FALSE)) != 0) {
                (void) xname(otmp); /* set dknown, maybe bknown */
                stackobj(otmp);
            }
            newsym(x, y);
        } else if (u.dz > 0 && ttmp) {
            if (!striking && closeholdingtrap(&youmonst, &disclose)) {
                ; /* now stuck in web or bear trap */
            } else if (striking && ttmp->ttyp == TRAPDOOR) {
                /* striking transforms trapdoor into hole */
                if (Blind && !ttmp->tseen) {
                    pline("%s beneath you shatters.", Something);
                } else if (!ttmp->tseen) { /* => !Blind */
                    pline("There's a trapdoor beneath you; it shatters.");
                } else {
                    pline("The trapdoor beneath you shatters.");
                    disclose = TRUE;
                }
                ttmp->ttyp = HOLE;
                ttmp->tsubtyp = 0;
                ttmp->tflags = 0;
                ttmp->activation_count = 0;
                ttmp->tseen = 1;
                newsym(x, y);
                /* might fall down hole */
                dotrap(ttmp, 0);
            } else if (!striking && ttmp->ttyp == HOLE) {
                /* locking transforms hole into trapdoor */
                ttmp->ttyp = TRAPDOOR;
                ttmp->tsubtyp = 0;
                ttmp->tflags = 0;
                ttmp->activation_count = 0;
                if (Blind || !ttmp->tseen) {
                    pline("Some %s swirls beneath you.",
                          is_ice(x, y) ? "frost" : "dust");
                } else {
                    ttmp->tseen = 1;
                    newsym(x, y);
                    pline("A trapdoor appears beneath you.");
                    disclose = TRUE;
                }
                /* hadn't fallen down hole; won't fall now */
            }
        }
        break;
    case SPE_STONE_TO_FLESH:
        if (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz) || Underwater
            || (Is_qstart(&u.uz) && u.dz < 0)) {
            pline1(nothing_happens);
        } else if (u.dz < 0) { /* we should do more... */
            pline("Blood drips on your %s.", body_part(FACE));
        } else if (u.dz > 0 && !OBJ_AT(u.ux, u.uy)) {
            /*
            Print this message only if there wasn't an engraving
            affected here.  If water or ice, act like waterlevel case.
            */
            e = engr_at(u.ux, u.uy);
            if (!(e && e->engr_type == ENGRAVE)) {
                if (is_pool(u.ux, u.uy) || is_ice(u.ux, u.uy))
                    pline1(nothing_happens);
                else
                    pline("Blood %ss %s your %s.",
                          is_lava(u.ux, u.uy) ? "boil" : "pool",
                          Levitation ? "beneath" : "at",
                          makeplural(body_part(FOOT)));
            }
        }
        break;
    default:
        break;
    }

    if (u.dz > 0) {
        /* zapping downward */
        context.bhitcount = 0;
        (void)bhitpile(obj, &youmonst, bhito, x, y, u.dz, hit_only_one, (objects[obj->otyp].oc_spell_flags& S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT));

        /* subset of engraving effects; none sets `disclose' */
        if ((e = engr_at(x, y)) != 0 && e->engr_type != ENGR_HEADSTONE && e->engr_type != ENGR_SIGNPOST) {
            switch (obj->otyp) {
            case WAN_POLYMORPH:
            case SPE_POLYMORPH:
                del_engr(e);
                make_engr_at(x, y, random_engraving(buf), moves, (xchar) 0, ENGR_FLAGS_NONE);
                break;
            case WAN_CANCELLATION:
            case SPE_CANCELLATION:
            case WAN_DISJUNCTION:
            case SPE_DISJUNCTION:
            case WAN_MAKE_INVISIBLE:
                del_engr(e);
                break;
            case WAN_TELEPORTATION:
            case SPE_TELEPORT_MONSTER:
                if(!level.flags.noteleport)
                    rloc_engr(e);
                break;
            case SPE_STONE_TO_FLESH:
                if (e->engr_type == ENGRAVE) {
                    /* only affects things in stone */
                    pline_The(Hallucination
                                  ? "floor runs like butter!"
                                  : "edges on the floor get smoother.");
                    wipe_engr_at(x, y, d(2, 4), TRUE);
                }
                break;
            case WAN_STRIKING:
            case SPE_FORCE_BOLT:
            case SPE_FORCE_STRIKE:
                wipe_engr_at(x, y, d(2, 4), TRUE);
                break;
            default:
                break;
            }
        }
    } else if (u.dz < 0) {
        /* zapping upward */

        /* game flavor: if you're hiding under "something"
         * a zap upward should hit that "something".
         */
        if (u.uundetected && hides_under(youmonst.data)) {
            int hitit = 0;
            otmp = level.objects[u.ux][u.uy];

            if (otmp)
                hitit = bhito(otmp, obj, &youmonst);
            if (hitit) {
                (void) hideunder(&youmonst);
                disclose = TRUE;
            }
        }
    }

    return disclose;
}

/* used by do_break_wand() was well as by weffects() */
void
zapsetup()
{
    obj_zapped = FALSE;
}

void
zapwrapup()
{
    /* if do_osshock() set obj_zapped while polying, give a message now */
    if (obj_zapped)
        You_feel("shuddering vibrations.");
    obj_zapped = FALSE;
}

/* called for various wand and spell effects - M. Stephenson */
void
weffects(obj)
struct obj *obj;
{
    if (!obj)
        return;

    int otyp = obj->otyp;
    boolean disclose = FALSE;
    //boolean was_unkn = !objects[otyp].oc_name_known;

    exercise(A_WIS, TRUE);
    if (u.usteed && (objects[otyp].oc_dir != NODIR) && !u.dx && !u.dy
        && (u.dz > 0) && zap_steed(obj)) 
    {
        disclose = TRUE;
    } 
    else if (objects[otyp].oc_dir == IMMEDIATE || objects[otyp].oc_dir == IMMEDIATE_MULTIPLE_TARGETS
        || objects[otyp].oc_dir == IMMEDIATE_ONE_TO_THREE_TARGETS || objects[otyp].oc_dir == IMMEDIATE_ONE_TO_SEVEN_TARGETS || objects[otyp].oc_dir == IMMEDIATE_TWO_TO_SIX_TARGETS
        || objects[otyp].oc_dir == TOUCH)
    {
        zapsetup(); /* reset obj_zapped */
        if (u.uswallow) 
        {
            (void) bhitm(u.ustuck, obj, &youmonst);
            /* [how about `bhitpile(u.ustuck->minvent)' effect?] */
        }
        else if (u.dz) 
        {
            disclose = zap_updown(obj);
        } 
        else
        {
            int range = 1, radius = 0;

            if (objects[otyp].oc_dir == TOUCH)
                range = 1;
            else if (objects[otyp].oc_spell_range > 0)
                range = (int)objects[otyp].oc_spell_range;
            else
                range = rn1(8, 6);

            if (objects[otyp].oc_spell_radius > 0)
                radius = (int)objects[otyp].oc_spell_radius;

            uchar hit_only_one = 1;
            if (objects[otyp].oc_dir == IMMEDIATE_MULTIPLE_TARGETS)
                hit_only_one = 0;
            else if (objects[otyp].oc_dir == IMMEDIATE_ONE_TO_THREE_TARGETS)
                hit_only_one = 2; /* 1- 3 targets based on BUC status */
            else if (objects[otyp].oc_dir == IMMEDIATE_ONE_TO_SEVEN_TARGETS)
                hit_only_one = 3; /* 1- 7 targets based on BUC status */
            else if (objects[otyp].oc_dir == IMMEDIATE_TWO_TO_SIX_TARGETS)
                hit_only_one = 4; /* 2- 6 targets based on BUC status */

            (void) bhit(u.dx, u.dy, range, radius, ZAPPED_WAND, bhitm, bhito, &obj, &youmonst, hit_only_one, !!(objects[otyp].oc_spell_flags & S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT));
        }
        zapwrapup(); /* give feedback for obj_zapped */

    } 
    else if (objects[otyp].oc_dir == NODIR) 
    {
        zapnodir(obj);

    } 
    else if (objects[otyp].oc_dir == RAY)
    {
        /* neither immediate nor directionless */
        int osubtype = objects[otyp].oc_dir_subtype;

        switch (osubtype)
        {
        case RAY_DIGGING:
        case RAY_WND_DIGGING:
            zap_dig(obj);
            break;
        case RAY_EVAPORATION:
        case RAY_WND_EVAPORATION:
            zap_evaporation(obj);
            break;
        case RAY_WND_MAGIC_MISSILE:
        case RAY_WND_FIRE:
        case RAY_WND_COLD:
        case RAY_WND_SLEEP:
        case RAY_WND_LIGHTNING:
        case RAY_WND_POISON_GAS:
        case RAY_WND_ACID:
        case RAY_WND_DEATH:
        case RAY_WND_DISINTEGRATION:
        case RAY_WND_PETRIFICATION:
        case RAY_MAGIC_MISSILE:
        case RAY_FIRE:
        case RAY_COLD:
        case RAY_LIGHTNING:
        case RAY_SLEEP:
        case RAY_POISON_GAS:
        case RAY_ACID:
        case RAY_DEATH:
        case RAY_DISINTEGRATION:
        case RAY_PETRIFICATION:
            buzz(osubtype, obj, &youmonst, 0, 0, 0, u.ux, u.uy, u.dx, u.dy);
            break;
        default:
            impossible("weffects: unexpected spell or wand");
            break;
        }

        /* Give skill points for wand use */
        if (objects[otyp].oc_class == WAND_CLASS)
        {
            use_skill(P_WAND, 2);
        }

        /*
        if (otyp == WAN_DIGGING || otyp == SPE_DIG)
            zap_dig();
        else if (otyp == SPE_FINGER_OF_DEATH)
            buzz(ZT_SPELL(ZT_DEATH), u.ulevel / 2 + 1, u.ux, u.uy, u.dx, u.dy);
        else if (otyp == WAN_DEATH)
            buzz(ZT_WAND(ZT_DEATH), 1, u.ux, u.uy, u.dx, u.dy);
        else if (otyp >= SPE_MAGIC_MISSILE && otyp <= SPE_DISINTEGRATE)
            buzz(otyp - SPE_MAGIC_MISSILE + 10, u.ulevel / 2 + 1, u.ux, u.uy,
                 u.dx, u.dy);
        else if (otyp >= WAN_MAGIC_MISSILE && otyp <= WAN_LIGHTNING)
            buzz(otyp - WAN_MAGIC_MISSILE,
                 (otyp == WAN_MAGIC_MISSILE) ? 2 : 6, u.ux, u.uy, u.dx, u.dy);
        else
            impossible("weffects: unexpected spell or wand");
            */

        disclose = TRUE;
    }
    if (disclose) {
        learnwand(obj);
//        if (was_unkn)
//            more_experienced(0, 10);
    }
    return;
}

/* Note: this is needed, because objects cannot have self-references */
int 
get_displayed_object_type_from_subdir_type(subdir)
int subdir;
{
    int displayedobjtype = STRANGE_OBJECT;
    switch (subdir)
    {
    case IMMEDIATE_ITEM_ARROW:
        displayedobjtype = ENCHANTED_ARROW;
        break;
    default:
        break;
    }

    return displayedobjtype;
}

/*
 * Generate the to hit bonus for a spell.  Based on the hero's skill in
 * spell class and dexterity.
 */
STATIC_OVL int
m_spell_hit_skill_bonus(mtmp, otyp)
struct monst* mtmp;
int otyp;
{
    if (!mtmp)
        return 0;

    int hit_bon = 0;
    int skill_level = P_ISRESTRICTED;

    if(mtmp == &youmonst)
        skill_level = P_SKILL_LEVEL(spell_skilltype(otyp));
    else
    {
        if(is_prince(mtmp->data))
            skill_level = P_GRAND_MASTER;
        else if (is_lord(mtmp->data))
            skill_level = P_EXPERT;
        else
            skill_level = P_BASIC;
    }

    switch (skill_level) {
    case P_ISRESTRICTED:
    case P_UNSKILLED:
        hit_bon = 0;
        break;
    case P_BASIC:
        hit_bon = 3;
        break;
    case P_SKILLED:
        hit_bon = 6;
        break;
    case P_EXPERT:
        hit_bon = 9;
        break;
    case P_MASTER:
        hit_bon = 12;
        break;
    case P_GRAND_MASTER:
        hit_bon = 15;
        break;
    }

    return hit_bon;
}

STATIC_OVL int
m_spell_hit_dex_bonus(mtmp, otyp)
struct monst* mtmp;
int otyp;
{
    if (!mtmp)
        return 0;

    if (otyp == STRANGE_OBJECT)
    {
        /* Do nothing for the time being */
    }

    int hit_bon = 0;
    int dex = M_ACURR(mtmp, A_DEX);

    if (dex < 4)
        hit_bon -= 3;
    else if (dex < 6)
        hit_bon -= 2;
    else if (dex < 8)
        hit_bon -= 1;
    else if (dex < 14)
        /* Will change when print stuff below removed */
        hit_bon -= 0;
    else
        /* Even increment for dextrous heroes (see weapon.c u_strdex_to_hit_bonus) */
        hit_bon += dex - 14;

    return hit_bon;
}

STATIC_OVL int
m_wand_hit_skill_bonus(mtmp, otyp)
struct monst* mtmp;
int otyp;
{
    if (!mtmp || objects[otyp].oc_class != WAND_CLASS)
        return 0;

    int skill_level = P_ISRESTRICTED;

    if (mtmp == &youmonst)
        skill_level = P_SKILL_LEVEL(P_WAND);
    else
    {
        if (is_prince(mtmp->data))
            skill_level = P_GRAND_MASTER;
        else if (is_lord(mtmp->data))
            skill_level = P_EXPERT;
        else
            skill_level = P_BASIC;
    }

    return wand_skill_hit_bonus(skill_level);
}

const char *
exclam(force)
int force;
{
    /* force == 0 occurs e.g. with sleep ray */
    /* note that large force is usual with wands so that !! would
            require information about hand/weapon/wand */
    return (const char *) ((force < 0) ? "?" : (force <= 4) ? "." : "!");
}

void
hit(str, mtmp, force, damage, adjective)
const char* str;
struct monst* mtmp;
const char* force; /* usually either "." or "!" */
const char* adjective; /* usually either "" or " critically" */
int damage;
{
    hit_with_hit_tile(str, mtmp, force, damage, adjective, HIT_GENERAL, FALSE);
}

void
hit_with_hit_tile(str, mtmp, force, damage, adjective, hit_tile, show_hit_tile_always)
const char *str;
struct monst *mtmp;
const char *force; /* usually either "." or "!" */
const char* adjective; /* usually either "" or " critically" */
int damage;
enum hit_tile_types hit_tile;
boolean show_hit_tile_always;
{
    if ((!cansee(bhitpos.x, bhitpos.y) && !canspotmon(mtmp)
         && !(u.uswallow && mtmp == u.ustuck)) || !flags.verbose)
        if (damage > 0)
        {
            int multicolors[5] = { NO_COLOR, NO_COLOR, NO_COLOR, CLR_ORANGE, NO_COLOR };
            pline_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolors, "%s %s it%s for %d damage%s", The(str), vtense(str, "hit"), adjective, damage, force);
        }
        else
            pline("%s %s it%s%s", The(str), vtense(str, "hit"), adjective, force);
    else
        if (damage > 0)
        {
            int multicolors[6] = { NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, CLR_ORANGE, NO_COLOR };
            pline_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolors, "%s %s %s%s for %d damage%s", The(str), vtense(str, "hit"), mon_nam(mtmp), adjective, damage, force);
        }
        else
            pline("%s %s %s%s%s", The(str), vtense(str, "hit"), mon_nam(mtmp), adjective, force);

    if(damage > 0 || show_hit_tile_always)
        display_m_being_hit(mtmp, hit_tile, damage, 0UL, TRUE);

}

void
miss(str, mtmp)
register const char *str;
register struct monst *mtmp;
{
    pline(
        "%s %s %s.", The(str), vtense(str, "miss"),
        ((cansee(bhitpos.x, bhitpos.y) || canspotmon(mtmp)) && flags.verbose)
            ? mon_nam(mtmp)
            : "it");
}

STATIC_OVL void
skiprange(range, skipstart, skipend)
int range, *skipstart, *skipend;
{
    int tr = (range / 4);
    int tmp = range - ((tr > 0) ? rnd(tr) : 0);

    *skipstart = tmp;
    *skipend = tmp - ((tmp / 4) * rnd(3));
    if (*skipend >= tmp)
        *skipend = tmp - 1;
}

/*
 *  Called for the following distance effects:
 *      when a weapon is thrown (weapon == THROWN_WEAPON)
 *      when an object is kicked (KICKED_WEAPON)
 *      when an IMMEDIATE wand is zapped (ZAPPED_WAND)
 *      when a light beam is flashed (FLASHED_LIGHT)
 *      when a mirror is applied (INVIS_BEAM)
 *  A thrown/kicked object falls down at end of its range or when a monster
 *  is hit.  The variable 'bhitpos' is set to the final position of the weapon
 *  thrown/zapped.  The ray of a wand may affect (by calling a provided
 *  function) several objects and monsters on its path.  The return value
 *  is the monster hit (weapon != ZAPPED_WAND), or a null monster pointer.
 *
 * Thrown and kicked objects (THROWN_WEAPON or KICKED_WEAPON) may be
 * destroyed and *pobj set to NULL to indicate this.
 *
 *  Check !u.uswallow before calling bhit().
 *  This function reveals the absence of a remembered invisible monster in
 *  necessary cases (throwing or kicking weapons).  The presence of a real
 *  one is revealed for a weapon, but if not a weapon is left up to fhitm().
 */
struct monst *
bhit(ddx, ddy, range, radius, weapon, fhitm, fhito, pobj, origmonst, hit_only_one, stop_at_first_hit_object)
register int ddx, ddy, range, radius;          /* direction, range, and effect radius */
enum bhit_call_types weapon;           /* defined in hack.h */
int FDECL((*fhitm), (MONST_P, OBJ_P, MONST_P)), /* fns called when mon/obj hit */
    FDECL((*fhito), (OBJ_P, OBJ_P, MONST_P));
struct obj **pobj; /* object tossed/used, set to NULL
                    * if object is destroyed */
struct monst* origmonst;
uchar hit_only_one;
boolean stop_at_first_hit_object;
{
    struct monst *mtmp;
    struct obj *obj = *pobj;
    uchar typ;
    boolean shopdoor = FALSE, point_blank = TRUE;
    boolean in_skip = FALSE, allow_skip = FALSE;
    boolean tethered_weapon = FALSE;
    int skiprange_start = 0, skiprange_end = 0, skipcount = 0;
    context.bhitcount = 0;
    int bucstatus = !obj || obj->cursed ? -1 : obj->blessed ? 1 : 0;
    int bhitlimit = hit_only_one == 1 ? 1 : 
        hit_only_one == 2 ? (bucstatus == -1 ? 1 : bucstatus == 0 ? 2 : 3) :
        hit_only_one == 3 ? (bucstatus == -1 ? 1 : bucstatus == 0 ? 4 : 7) :
        hit_only_one == 4 ? (bucstatus == -1 ? 2 : bucstatus == 0 ? 4 : 6) :
        0;

    if (weapon == ZAPPED_WAND || weapon == FLASHED_LIGHT || weapon == INVIS_BEAM)
        if(obj)
            play_immediate_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_CREATE, u.ux, u.uy);

    if (weapon == KICKED_WEAPON || weapon == GOLF_SWING)
    {
        /* object starts one square in front of player */
        bhitpos.x = u.ux + ddx;
        bhitpos.y = u.uy + ddy;
        range--;
    }
    else
    {
        bhitpos.x = u.ux;
        bhitpos.y = u.uy;
    }

    if (weapon == THROWN_WEAPON && obj && is_rock(obj)) {
        skiprange(range, &skiprange_start, &skiprange_end);
        allow_skip = !rn2(3);
    }

    int displayedobjtype = STRANGE_OBJECT;
    struct obj dispobj = zeroobj;
    if (weapon == ZAPPED_WAND && obj)
    {
        if (objects[obj->otyp].oc_dir_subtype > 0)
        {
            displayedobjtype = get_displayed_object_type_from_subdir_type(objects[obj->otyp].oc_dir_subtype);
            dispobj.otyp = displayedobjtype;
        }
        else if (objects[obj->otyp].oc_dir_subtype < 0)
        {
            displayedobjtype = objects[obj->otyp].oc_dir_subtype;
        }
    }

    boolean zapped_wand_obj_displayed = FALSE;
    boolean zapped_wand_beam = FALSE;

    if (weapon == FLASHED_LIGHT) 
    {
        tmp_at(DISP_BEAM, zapdir_to_glyph(ddx, ddy, ZAP_SPECIAL_FLASHED_LIGHT));
    } 
    else if (weapon == THROWN_TETHERED_WEAPON && obj) 
    {
        tethered_weapon = TRUE;
        weapon = THROWN_WEAPON;     /* simplify if's that follow below */
        tmp_at(DISP_TETHER, obj_to_missile_glyph(obj, get_missile_index(ddx, ddy), rn2_on_display_rng));
    }
    else if (weapon != ZAPPED_WAND && weapon != INVIS_BEAM)
    {
        tmp_at_with_missile_flags(DISP_FLASH, obj_to_missile_glyph(obj, get_missile_index(ddx, ddy), rn2_on_display_rng), get_missile_flags(obj, FALSE), obj ? obj->material : MAT_NONE, obj ? obj->special_quality : 0);
    }
    else if (weapon == ZAPPED_WAND && displayedobjtype != STRANGE_OBJECT)
    {
        zapped_wand_obj_displayed = TRUE;
        if (displayedobjtype > STRANGE_OBJECT)
        {
            tmp_at(DISP_FLASH, obj_to_missile_glyph(&dispobj, get_missile_index(ddx, ddy), rn2_on_display_rng));
        }
        else if (displayedobjtype == IMMEDIATE_FLASHED_LIGHT)
        {
            tmp_at(DISP_BEAM, zapdir_to_glyph(ddx, ddy, ZAP_SPECIAL_FLASHED_LIGHT));
            zapped_wand_beam = TRUE;
        }
        else if (displayedobjtype <= IMMEDIATE_MAGIC_MISSILE_BEAM)
        {
            tmp_at(DISP_BEAM, zapdir_to_glyph(ddx, ddy, -displayedobjtype - 11));
            zapped_wand_beam = TRUE;
        }
        else if (displayedobjtype < STRANGE_OBJECT)
        {
            tmp_at(DISP_FLASH, zapdir_to_glyph(ddx, ddy, -displayedobjtype - 1));
        }
    }

    if(obj)
        start_ambient_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, bhitpos.x, bhitpos.y);

    boolean beam_cleared_off = FALSE;
    boolean drawbridge_hit = FALSE;
    boolean tree_hit = FALSE;

    while (range-- > 0)
    {
        int x, y;

        if(isok(bhitpos.x, bhitpos.y))
            show_missile_info(bhitpos.x, bhitpos.y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0UL, 0, 0, 0); /* Clear missile info out in the previous location */

        bhitpos.x += ddx;
        bhitpos.y += ddy;
        x = bhitpos.x;
        y = bhitpos.y;

        if (obj)
            update_ambient_ray_sound_to_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, bhitpos.x, bhitpos.y);

        if (!isok(x, y))
        {
            bhitpos.x -= ddx;
            bhitpos.y -= ddy;
            break;
        }

        if (is_pick(obj) && inside_shop(x, y)
            && (mtmp = shkcatch(obj, x, y)) != 0) 
        {
            tmp_at(DISP_END, 0);
            if (obj)
                stop_ambient_ray_sound(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset);
            return mtmp;
        }

        typ = levl[bhitpos.x][bhitpos.y].typ;

        /* iron bars will block anything big enough */
        if ((weapon == THROWN_WEAPON || weapon == KICKED_WEAPON || weapon == GOLF_SWING)
            && typ == IRONBARS
            && hits_bars(pobj, x - ddx, y - ddy, bhitpos.x, bhitpos.y,
                         point_blank ? 0 : !rn2(5), 1)) 
        {
            /* caveat: obj might now be null... */
            obj = *pobj;
            bhitpos.x -= ddx;
            bhitpos.y -= ddy;
            break;
        }

        if (weapon == ZAPPED_WAND && !drawbridge_hit && find_drawbridge(&x, &y))
        {
            boolean learn_it = FALSE;

            switch (obj->otyp) 
            {
            case WAN_OPENING:
            case SPE_KNOCK:
                if (is_db_wall(bhitpos.x, bhitpos.y))
                {
                    if (cansee(x, y) || cansee(bhitpos.x, bhitpos.y))
                        learn_it = TRUE;
                    open_drawbridge(x, y, TRUE);
                    drawbridge_hit = TRUE;
                }
                break;
            case WAN_LOCKING:
            case SPE_WIZARD_LOCK:
                if ((cansee(x, y) || cansee(bhitpos.x, bhitpos.y))
                    && levl[x][y].typ == DRAWBRIDGE_DOWN)
                    learn_it = TRUE;
                maybe_close_drawbridge(x, y, TRUE);
                drawbridge_hit = TRUE;
                break;
            case WAN_STRIKING:
            case SPE_FORCE_BOLT:
            case SPE_FORCE_STRIKE:
                if (typ == DBWALL)
                {
                    destroy_drawbridge(x, y, FALSE);
                    drawbridge_hit = TRUE;
                }
                learn_it = TRUE;
                break;
            }

            if (learn_it)
                learnwand(obj);

        }

        if (weapon == ZAPPED_WAND && !tree_hit && IS_TREE(typ))
        {
            boolean learn_it = FALSE;
            switch (obj->otyp)
            {
            case WAN_STRIKING:
            case SPE_FORCE_BOLT:
            case SPE_FORCE_STRIKE:
                zap_try_destroy_tree(x, y);
                tree_hit = TRUE;
                learn_it = TRUE;
                break;
            default:
                break;
            }
            if (learn_it)
                learnwand(obj);
        }

        mtmp = m_at(bhitpos.x, bhitpos.y);

        /*
         * skipping rocks
         *
         * skiprange_start is only set if this is a thrown rock
         */
        if (skiprange_start && (range == skiprange_start) && allow_skip)
        {
            if (is_pool(bhitpos.x, bhitpos.y) && !mtmp) 
            {
                in_skip = TRUE;
                if (!Blind)
                    pline("%s %s%s.", Yname2(obj), otense(obj, "skip"),
                          skipcount ? " again" : "");
                else
                    You_hear("%s skip.", yname(obj));
                skipcount++;
            } else if (skiprange_start > skiprange_end + 1)
            {
                --skiprange_start;
            }
        }

        if (in_skip)
        {
            if (range <= skiprange_end)
            {
                in_skip = FALSE;
                if (range > 3) /* another bounce? */
                    skiprange(range, &skiprange_start, &skiprange_end);
            }
            else if (mtmp && M_IN_WATER(mtmp->data)) 
            {
                if ((!Blind && canseemon(mtmp)) || sensemon(mtmp))
                    pline("%s %s over %s.", Yname2(obj), otense(obj, "pass"),
                          mon_nam(mtmp));
            }
        }

        if (mtmp && !(in_skip && M_IN_WATER(mtmp->data)))
        {
            notonhead = (bhitpos.x != mtmp->mx || bhitpos.y != mtmp->my);

            if (obj && (weapon == ZAPPED_WAND || weapon == FLASHED_LIGHT || weapon == INVIS_BEAM))
                play_immediate_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_HIT_MONSTER, bhitpos.x, bhitpos.y);

            if (weapon == FLASHED_LIGHT) 
            {
                /* FLASHED_LIGHT hitting invisible monster should
                   pass through instead of stop so we call
                   flash_hits_mon() directly rather than returning
                   mtmp back to caller.  That allows the flash to
                   keep on going.  Note that we use mtmp->minvis
                   not canspotmon() because it makes no difference
                   whether the hero can see the monster or not. */
                if (is_invisible(mtmp))
                {
                    obj->ox = u.ux, obj->oy = u.uy;
                    (void) flash_hits_mon(mtmp, obj);
                }
                else 
                {
                    tmp_at(DISP_END, 0);
                    if (obj)
                    {
                        play_immediate_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_DESTROY, bhitpos.x, bhitpos.y);
                        stop_ambient_ray_sound(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset);
                    }
                    return mtmp; /* caller will call flash_hits_mon */
                }
            } 
            else if (weapon == INVIS_BEAM) 
            {
                /* Like FLASHED_LIGHT, INVIS_BEAM should continue
                   through invisible targets; unlike it, we aren't
                   prepared for multiple hits so just get first one
                   that's either visible or could see its invisible
                   self.  [No tmp_at() cleanup is needed here.] */
                if (!is_invisible(mtmp) || has_see_invisible(mtmp))
                {
                    if (obj)
                    {
                        play_immediate_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_DESTROY, bhitpos.x, bhitpos.y);
                            stop_ambient_ray_sound(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset);
                    }
                    return mtmp;
                }
            } 
            else if (weapon != ZAPPED_WAND)
            {

                /* THROWN_WEAPON, KICKED_WEAPON */
                if (!tethered_weapon)
                {
                    tmp_at(DISP_END, 0);
                }
                if (cansee(bhitpos.x, bhitpos.y) && !canspotmon(mtmp))
                    map_invisible(bhitpos.x, bhitpos.y);

                if (obj)
                    stop_ambient_ray_sound(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset);
                return mtmp;
            }
            else 
            {
                boolean bhitadded = FALSE;
                /* ZAPPED_WAND */
                if (weapon == ZAPPED_WAND && zapped_wand_obj_displayed && !zapped_wand_beam)
                {
                    bhitadded = TRUE;
                    context.bhitcount++;
                    if (hit_only_one && context.bhitcount >= bhitlimit)
                    {
                        tmp_at(DISP_END, 0);
                        if (obj)
                            play_immediate_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_DESTROY, bhitpos.x, bhitpos.y);
                        beam_cleared_off = TRUE;
                    }
                    else
                        tmp_at(bhitpos.x, bhitpos.y);
                }
                
                int had_effect = (*fhitm)(mtmp, obj, origmonst);
                //int more_effect_num = 0;

                if (had_effect && obj)
                    play_immediate_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_HIT_MONSTER, bhitpos.x, bhitpos.y);

                if (had_effect && !bhitadded)
                    context.bhitcount++;

                /* Make radius if it is specified */
                if (radius > 0)
                {
                    for (int rx = -radius; rx <= radius; rx++)
                    {
                        for (int ry = -radius; ry <= radius; ry++)
                        {
                            int px = bhitpos.x + rx;
                            int py = bhitpos.y + ry;

                            if (isok(px, py) && dist2(px, py, bhitpos.x, bhitpos.y) <= (radius + 1) * radius)
                            {
                                struct monst* mtmp2 = m_at(px, py);
                                if (mtmp2 && mtmp2 != mtmp && mtmp2 != &youmonst)
                                {
                                    //int had_more_effect = 0;
                                    /*had_more_effect = */(void)(*fhitm)(mtmp2, obj, origmonst);
                                    //more_effect_num += had_more_effect;
                                }
                            }
                        }
                    }
                }
                if (had_effect)
                {
                    if (hit_only_one && context.bhitcount >= bhitlimit)
                        break;
                    else
                        range -= 3;
                }
            }
        } 
        else 
        {
            if (weapon == ZAPPED_WAND && (obj->otyp == WAN_PROBING || obj->otyp == SPE_PROBE)
                && glyph_is_invisible(levl[bhitpos.x][bhitpos.y].hero_memory_layers.glyph))
            {
                unmap_object(bhitpos.x, bhitpos.y);
                newsym(x, y);
            }
        }
        if (fhito)
        {
            if (bhitpile(obj, origmonst, fhito, bhitpos.x, bhitpos.y, 0, hit_only_one, stop_at_first_hit_object))
            {
                if(weapon == ZAPPED_WAND || weapon == FLASHED_LIGHT || weapon == INVIS_BEAM)
                    if(obj)
                        play_immediate_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_HIT_OBJECT, bhitpos.x, bhitpos.y);

                if (stop_at_first_hit_object)
                    break;
                else
                    range--;
            }
        } 
        else 
        {
            if ((weapon == KICKED_WEAPON || weapon == GOLF_SWING)
                && ((obj->oclass == COIN_CLASS
                     && OBJ_AT(bhitpos.x, bhitpos.y))
                    || ship_object(obj, bhitpos.x, bhitpos.y,
                                   costly_spot(bhitpos.x, bhitpos.y))))
            {
                tmp_at(DISP_END, 0);
                if (obj)
                    stop_ambient_ray_sound(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset);
                return (struct monst *) 0;
            }
        }
        if (weapon == ZAPPED_WAND && (IS_DOOR(typ) || typ == SDOOR))
        {
            switch (obj->otyp) 
            {
            case WAN_OPENING:
            case WAN_LOCKING:
            case WAN_STRIKING:
            case SPE_KNOCK:
            case SPE_WIZARD_LOCK:
            case SPE_FORCE_BOLT:
            case SPE_FORCE_STRIKE:
                if (doorlock(obj, bhitpos.x, bhitpos.y))
                {
                    if (cansee(bhitpos.x, bhitpos.y)
                        || (obj->otyp == WAN_STRIKING && !Deaf))
                        learnwand(obj);
                    if ((levl[bhitpos.x][bhitpos.y].doormask & D_MASK) == D_BROKEN
                        && *in_rooms(bhitpos.x, bhitpos.y, SHOPBASE))
                    {
                        shopdoor = TRUE;
                        add_damage(bhitpos.x, bhitpos.y, SHOP_DOOR_COST);
                    }
                }
                break;
            }
        }
        if (!ZAP_POS(typ) || closed_door(bhitpos.x, bhitpos.y))
        {
            bhitpos.x -= ddx;
            bhitpos.y -= ddy;
            break;
        }
        if ((weapon != ZAPPED_WAND || (weapon == ZAPPED_WAND && displayedobjtype != STRANGE_OBJECT)) && weapon != INVIS_BEAM)
        {
            /* 'I' present but no monster: erase */
            /* do this before the tmp_at() */
            if (glyph_is_invisible(levl[bhitpos.x][bhitpos.y].hero_memory_layers.glyph)
                && cansee(x, y)) 
            {
                unmap_object(bhitpos.x, bhitpos.y);
                newsym(x, y);
            }
            tmp_at(bhitpos.x, bhitpos.y);
            if (obj && ((is_poisonable(obj) && obj->opoisoned) || obj->material != objects[obj->otyp].oc_material || obj->special_quality != 0 || obj->elemental_enchantment || obj->exceptionality || obj->mythic_prefix || obj->mythic_suffix || obj->oeroded || obj->oeroded2 || tethered_weapon || get_obj_height(obj) > 0 || obj->lamplit))
            {                
                show_missile_info(bhitpos.x, bhitpos.y, obj->opoisoned, obj->material, obj->special_quality, obj->elemental_enchantment, obj->exceptionality, obj->mythic_prefix, obj->mythic_suffix, obj->oeroded, obj->oeroded2, get_missile_flags(obj, tethered_weapon), get_obj_height(obj), 0, 0);
                if (tethered_weapon)
                    show_leash_info(bhitpos.x, bhitpos.y, 0, 0, u.ux, u.uy);
                flush_screen(1);
            }

            adjusted_delay_output();
            /* kicked objects fall in pools */
            if ((weapon == KICKED_WEAPON || weapon == GOLF_SWING)
                && (is_pool(bhitpos.x, bhitpos.y)
                    || is_lava(bhitpos.x, bhitpos.y)))
                break;

            if (IS_SINK(typ) && weapon != FLASHED_LIGHT)
                break; /* physical objects fall onto sink */

        }

        /* limit range of ball so hero won't make an invalid move */
        if (weapon == THROWN_WEAPON && range > 0
            && obj->otyp == HEAVY_IRON_BALL)
        {
            struct obj *bobj;
            struct trap *t;

            if ((bobj = sobj_at(BOULDER, x, y)) != 0)
            {
                if (cansee(x, y))
                    pline("%s hits %s.", The(distant_name(obj, xname)),
                          an(xname(bobj)));
                range = 0;
            } 
            else if (obj == uball)
            {
                if (!test_move(x - ddx, y - ddy, ddx, ddy, TEST_MOVE))
                {
                    /* nb: it didn't hit anything directly */
                    if (cansee(x, y))
                        pline("%s jerks to an abrupt halt.",
                              The(distant_name(obj, xname))); /* lame */
                    range = 0;
                } 
                else if (Sokoban && (t = t_at(x, y)) != 0
                           && (is_pit(t->ttyp) || is_hole(t->ttyp))) 
                {
                    /* hero falls into the trap, so ball stops */
                    range = 0;
                }
            }
        }

        /* thrown/kicked missile has moved away from its starting spot */
        point_blank = FALSE; /* affects passing through iron bars */
    }

    if ((
        weapon != ZAPPED_WAND
        || (weapon == ZAPPED_WAND && zapped_wand_obj_displayed && !beam_cleared_off)
        )
        && weapon != INVIS_BEAM && !tethered_weapon)
    {
        tmp_at(DISP_END, 0);
        if (weapon == ZAPPED_WAND || weapon == FLASHED_LIGHT || weapon == INVIS_BEAM)
            if(obj)
               play_immediate_ray_sound_at_location(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_DESTROY, u.ux, u.uy);
    }
    if (shopdoor)
        pay_for_damage("destroy", FALSE);

    if (obj)
        stop_ambient_ray_sound(object_soundsets[objects[obj->otyp].oc_soundset].ray_soundset);

    return (struct monst *) 0;
}

/* process thrown boomerang, which travels a curving path...
 * A multi-shot volley ought to have all missiles in flight at once,
 * but we're called separately for each one.  We terminate the volley
 * early on a failed catch since continuing to throw after being hit
 * is too obviously silly.
 */
struct monst *
boomhit(obj, dx, dy)
struct obj *obj;
int dx, dy;
{
    register int i, ct;
    int boom; /* showsym[] index  */
    struct monst *mtmp;
    boolean counterclockwise = TRUE; /* right-handed throw */

    /* counterclockwise traversal patterns:
     *  ..........................54.................................
     *  ..................43.....6..3....765.........................
     *  ..........32.....5..2...7...2...8...4....87..................
     *  .........4..1....6..1...8..1....9...3...9..6.....98..........
     *  ..21@....5...@...7..@....9@......@12....@...5...@..7.....@9..
     *  .3...9....6..9....89.....................1..4...1..6....1..8.
     *  .4...8.....78.............................23....2..5...2...7.
     *  ..567............................................34....3..6..
     *  ........................................................45...
     * (invert rows for corresponding clockwise patterns)
     */

    bhitpos.x = u.ux;
    bhitpos.y = u.uy;
    int boomstart = counterclockwise ? S_boom_ml - S_boom_tl : S_boom_mr - S_boom_tl;
    boom = S_boom_tl + boomstart;
    for (i = 0; i < 8; i++)
        if (xdir[i] == dx && ydir[i] == dy)
            break;
    tmp_at(DISP_FLASH, cmap_to_glyph(boom));
    for (ct = 0; ct < 10; ct++) {
        i = (i + 8) % 8;                          /* 0..7 (8 -> 0, -1 -> 7) */
        boom = S_boom_tl + (counterclockwise ? ((8 + boomstart - i) % 8) : ((boomstart + i) % 8)); /* toggle */
        tmp_at(DISP_CHANGE, cmap_to_glyph(boom)); /* change glyph */
        dx = xdir[i];
        dy = ydir[i];
        bhitpos.x += dx;
        bhitpos.y += dy;
        if ((mtmp = m_at(bhitpos.x, bhitpos.y)) != 0) {
            m_respond(mtmp);
            tmp_at(DISP_END, 0);
            return mtmp;
        }
        if (!ZAP_POS(levl[bhitpos.x][bhitpos.y].typ)
            || closed_door(bhitpos.x, bhitpos.y)) {
            bhitpos.x -= dx;
            bhitpos.y -= dy;
            break;
        }
        if (bhitpos.x == u.ux && bhitpos.y == u.uy) { /* ct == 9 */
            if (Fumbling || rn2(20) >= ACURR(A_DEX)) {
                /* we hit ourselves */
                int dmg = weapon_dmg_value(obj, &youmonst, (struct monst*)0, 1);
                int extradmg = weapon_extra_dmg_value(obj, &youmonst, (struct monst*)0, dmg);
                (void) thitu(10 + obj->enchantment, dmg + extradmg, &obj,
                             "boomerang", &youmonst, "thrown");
                endmultishot(TRUE);
                break;
            } else { /* we catch it */
                tmp_at(DISP_END, 0);
                You("skillfully catch the boomerang.");
                return &youmonst;
            }
        }
        tmp_at(bhitpos.x, bhitpos.y);
        adjusted_delay_output();
        if (IS_SINK(levl[bhitpos.x][bhitpos.y].typ))
        {
            play_sfx_sound_at_location(SFX_SINK_KLUNK, bhitpos.x, bhitpos.y);
            if (!Deaf)
                pline("Klonk!");
            break; /* boomerang falls on sink */
        }
        /* ct==0, initial position, we want next delta to be same;
           ct==5, opposite position, repeat delta undoes first one */
        if (ct % 5 != 0)
            i += (counterclockwise ? -1 : 1);
    }
    tmp_at(DISP_END, 0); /* do not leave last symbol */
    return (struct monst *) 0;
}

/* used by buzz(); also used by munslime(muse.c); returns damage applied
   to mon; note: caller is responsible for killing mon if damage is fatal */
double
zhitm(mon, type, origobj, origmonst, dmgdice, dicesize, dmgplus, ootmp, out_flags_ptr)
register struct monst *mon;
register int type;
struct obj* origobj;
struct monst* origmonst;
int dmgdice, dicesize, dmgplus;
struct obj **ootmp; /* to return worn armor for caller to disintegrate */
uchar* out_flags_ptr;
{
    double damage = 0;
    int dmg = 0;
    register int abstype = abs(type) % NUM_ZAP;
    boolean sho_shieldeff = FALSE;
    boolean sho_hit_eff = TRUE;
    boolean sleep_eff = FALSE;
    //boolean spellcaster = is_hero_spell(type); /* maybe get a bonus! */
    int duration = 0;
    //boolean magic_resistance_success = check_magic_resistance_and_inflict_damage(mon, origobj, type < ZT_SPELL(0) ? 12 : u.ulevel, 0, 0, NOTELL);

    *ootmp = (struct obj *) 0;
    struct obj* otmp2;

    //Base damage here, set to zero, if not needed
    if (origobj)
    {
        dmg = get_spell_damage(origobj->otyp, origmonst, mon);
    }
    else
        dmg = d(dmgdice, dicesize) + dmgplus;

    if(origobj)
        duration = d(objects[origobj->otyp].oc_spell_dur_dice, objects[origobj->otyp].oc_spell_dur_diesize) + objects[origobj->otyp].oc_spell_dur_plus; //Same for smal and big

    damage = adjust_damage(dmg, origmonst, mon, abstype + 1, (abs(type) >= 20 && abs(type) <=29) ? ADFLAGS_NONE : ADFLAGS_SPELL_DAMAGE);

    if (out_flags_ptr)
        *out_flags_ptr = 0;

    //boolean allow_critical_strike = FALSE;
    switch (abstype)
    {
    case ZT_MAGIC_MISSILE:
        if (is_mon_immune_to_magic_missile(mon))
        {
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
            damage = 0;
            break;
        }
        //allow_critical_strike = TRUE;
        /*
        tmp = d(nd, 6);
        if (spellcaster)
            tmp = spell_damage_bonus(tmp);
        */
        break;
    case ZT_FIRE:
        if (is_mon_immune_to_fire(mon))
        {
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
            damage = 0;
            break;
        }
        //allow_critical_strike = TRUE;
        /*
        tmp = d(nd, 6);
        if (is_mon_immune_to_cold(mon))
            tmp += 7;
        if (spellcaster)
            tmp = spell_damage_bonus(tmp);
        */
        if (burnarmor(mon)) 
        {
            if (!rn2(3))
                (void) destroy_mitem(mon, POTION_CLASS, AD_FIRE);
            if (!rn2(3))
                (void) destroy_mitem(mon, SCROLL_CLASS, AD_FIRE);
            if (!rn2(5))
                (void) destroy_mitem(mon, SPBOOK_CLASS, AD_FIRE);
            destroy_mitem(mon, FOOD_CLASS, AD_FIRE); /* carried slime */
        }
        break;
    case ZT_COLD:
        if (is_mon_immune_to_cold(mon)) 
        {
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
            damage = 0;
            break;
        }
        //allow_critical_strike = TRUE;
        /*
        tmp = d(nd, 6);
        if (is_mon_immune_to_fire(mon))
            tmp += d(nd, 3);
        if (spellcaster)
            tmp = spell_damage_bonus(tmp);
        */
        if (!rn2(3))
            (void) destroy_mitem(mon, POTION_CLASS, AD_COLD);
        break;
    case ZT_SLEEP:
        damage = 0;
        //allow_critical_strike = TRUE;
        /* moved after critical strike */
        break;
    case ZT_DISINTEGRATION:  /* disintegration */
        damage = 0;
        if (resists_disint(mon)) // || (magic_resistance_success && !(abs(type) >= 20 && abs(type) <= 39)))
        {
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
        }
        else if (mon->worn_item_flags & W_ARMS)
        {
            /* destroy shield; victim survives */
            *ootmp = which_armor(mon, W_ARMS);
        } else if (mon->worn_item_flags & W_ARM) {
            /* destroy body armor, also cloak if present */
            *ootmp = which_armor(mon, W_ARM);
            if ((otmp2 = which_armor(mon, W_ARMC)) != 0)
                m_useup(mon, otmp2);
        }
        else 
        {
            /* no body armor, victim dies; destroy cloak
                and shirt now in case target gets life-saved */
            if (out_flags_ptr)
                *out_flags_ptr |= ZHITM_FLAGS_DISINTEGRATION;
            if ((otmp2 = which_armor(mon, W_ARMC)) != 0)
                m_useup(mon, otmp2);
            if ((otmp2 = which_armor(mon, W_ARMU)) != 0)
                m_useup(mon, otmp2);
        }
        type = -1; /* no saving throw wanted */
        break;     /* not ordinary damage */
    case ZT_DEATH:                              /* death */
        if (mon->data == &mons[PM_DEATH]) 
        {
            mon->mbasehpmax += mon->mbasehpmax / 2;
            if (mon->mbasehpmax > DEATH_MAX_HP_FROM_DEATH_RAY)
                mon->mbasehpmax = DEATH_MAX_HP_FROM_DEATH_RAY;
            update_mon_maxhp(mon);
            mon->mhp = mon->mhpmax;
            damage = 0;
            break;
        }
        if (resists_death(mon))
        {
            /* similar to player */
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
            damage = 0;
            break;
        }
        type = -1; /* so they don't get saving throws */
        damage = (double)mon->mhp + 1;
        if (out_flags_ptr)
            *out_flags_ptr |= (ZHITM_FLAGS_DEATH | ZHITM_FLAGS_HIDE_DAMAGE);
        break;
    case ZT_PETRIFICATION: 
        if (resists_ston(mon))
        {
            /* similar to player */
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
            damage = 0;
            break;
        }
        else
        {
            if (out_flags_ptr)
                *out_flags_ptr |= ZHITM_FLAGS_PETRIFICATION;
            damage = 0;  //PETRIFICATION_DUMMY_DAMAGE;
        }
        type = -1; /* so they don't get saving throws */
        break;
    case ZT_LIGHTNING:
        if (is_mon_immune_to_elec(mon)) 
        {
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
            damage = 0;
            /* can still blind the monster */
        }
        //allow_critical_strike = TRUE;
        /*
        tmp = d(nd, 6);
        if (spellcaster)
            tmp = spell_damage_bonus(tmp);
            
        */
        if (!resists_blnd(mon)
            && !(type > 0 && u.uswallow && mon == u.ustuck)) 
        {
            register unsigned rnd_tmp = rnd(50);
            nonadditive_increase_mon_property(mon, BLINDED, rnd_tmp);
        }
        if (!rn2(3))
            (void) destroy_mitem(mon, WAND_CLASS, AD_ELEC);
        /* not actually possible yet */
        if (!rn2(3))
            (void) destroy_mitem(mon, RING_CLASS, AD_ELEC);
        break;
    case ZT_POISON_GAS:
        if (resists_poison(mon)) 
        {
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
            damage = 0;
            break;
        }
        //allow_critical_strike = TRUE;
        //tmp = d(nd, 6);
        break;
    case ZT_ACID:
        if (is_mon_immune_to_acid(mon)) 
        {
            sho_shieldeff = TRUE;
            sho_hit_eff = FALSE;
            damage = 0;
            break;
        }
        //allow_critical_strike = TRUE;
        //tmp = d(nd, 6);
        if (!rn2(6))
            acid_damage(MON_WEP(mon));
        if (!rn2(6))
            erode_armor(mon, ERODE_CORRODE);
        break;
    }
    if (sho_shieldeff)
        m_shieldeff(mon);
    
#if 0
    if (origobj && objects[origobj->otyp].oc_class == WAND_CLASS && allow_critical_strike)
    {
        int skill_crit_chance = get_skill_critical_strike_chance(P_WAND, FALSE, FALSE, 0, TRUE);
        if (skill_crit_chance > 0 && rn2(100) < skill_crit_chance)
        {
            if (out_flags_ptr)
                *out_flags_ptr |= ZHITM_FLAGS_CRITICAL_STRIKE;
            if (damage >= 0.0)
                damage *= 2.0;
            if (duration >= 0)
                duration *= 2;
        }
    }
#endif

    if (abstype == ZT_SLEEP)
    {
        int save_adj = 0;
        if (out_flags_ptr && (*out_flags_ptr & ZHITM_FLAGS_CRITICAL_STRIKE))
        {
            save_adj = -4;
        }
        sleep_eff = sleep_monst(mon, origobj, origmonst, !origobj ? rn1(5, 8) : duration, save_adj, NOTELL); // Duration 0 = permanent sleep
        sho_hit_eff = sleep_eff;
        if (out_flags_ptr && sleep_eff)
            *out_flags_ptr |= ZHITM_FLAGS_SLEEP;
    }

    if (out_flags_ptr && (*out_flags_ptr & ZHITM_FLAGS_PETRIFICATION)) /* caller starts delayed petrification */
        return damage;

    if (damage < 0)
        damage = 0; /* don't allow negative damage */

    debugpline3("zapped monster hp = %d (= %d - %d)", mon->mhp - (int)damage, mon->mhp, (int)damage);
    if(damage > 0)
        deduct_monster_hp(mon, damage);

    if (out_flags_ptr && !sho_hit_eff)
        *out_flags_ptr |= ZHITM_FLAGS_DO_NOT_SHOW_HIT_TILE;

    return damage;
}

STATIC_OVL void
zhitu(type, origobj, origmonst, dmgdice, dicesize, dmgplus, fltxt)
int type;
struct obj* origobj;
struct monst* origmonst;
int dmgdice, dicesize, dmgplus;
const char *fltxt;
{
    int dam = 0, abstyp = abs(type);
    enum hit_tile_types hit_tile = HIT_GENERAL;
    char killername[BUFSIZ] = "";

    //Base damage here, set to zero, if not needed
    if (origobj)
    {
        dam = get_spell_damage(origobj->otyp, origmonst, &youmonst);
    }
    else
        dam = d(dmgdice, dicesize) + dmgplus;

    double damage = adjust_damage(dam, origmonst, &youmonst, (abstyp % 10) + 1, !(abstyp >= 20 && abstyp <= 39) ? ADFLAGS_SPELL_DAMAGE : ADFLAGS_NONE);
    char hisbuf[BUFSZ] = "";

    if (origmonst && !origobj && is_buzztype_breath_weapon(type) && 
        ((abstyp % 10) == ZT_DISINTEGRATION || (abstyp % 10) == ZT_PETRIFICATION)
        )
    {
        /* Special case: Omit fltxt to avoid repetition: disintegrated/petrified by a black dragon's / gorgon's breath weapon */
        if (origmonst == &youmonst)
            Sprintf(hisbuf, "%s own", uhis());
        else
            Sprintf(hisbuf, "%s's", mon_monster_name(origmonst));

        Sprintf(killername, "%s %s", hisbuf, "breath weapon");
    }
    else if (fltxt)
    {
        if (origmonst)
        {
            if (origmonst == &youmonst)
                Sprintf(hisbuf, "%s own", uhis());
            else
                Sprintf(hisbuf, "%s's", an(mon_monster_name(origmonst)));

            if (origobj)
            {
                Sprintf(killername, "%s from %s %s", fltxt, hisbuf, killer_xname_flags(origobj, KXNFLAGS_NO_ARTICLE | KXNFLAGS_SPELL));
            }
            else
            {
                if (is_buzztype_breath_weapon(type))
                    Sprintf(killername, "%s from %s %s", fltxt, hisbuf, "breath weapon");
                else if(is_buzztype_eyestalk(type))
                    Sprintf(killername, "%s from %s %s", fltxt, hisbuf, "eyestalk");
                else
                    Sprintf(killername, "%s %s", hisbuf, fltxt);
            }
        }
        else
        {
            Strcpy(killername, fltxt);
        }
    }
    else
    {
        if (origmonst)
        {
            Strcpy(killername, mon_monster_name(origmonst));
        }
        else
            Strcpy(killername, "");
    }

    switch (abstyp % 10) 
    {
    case ZT_MAGIC_MISSILE:
        if (Magic_missile_immunity || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_REFLECTS);
            u_shieldeff(); // shieldeff(sx, sy);
            damage = 0;
            pline_The_ex(ATR_NONE, CLR_MSG_SUCCESS, "missiles bounce off!");
        } 
        else 
        {
            //dam = d(nd, 6);
            exercise(A_STR, FALSE);
        }
        break;
    case ZT_FIRE:
        hit_tile = HIT_ON_FIRE;
        if (Fire_immunity || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            u_shieldeff(); // shieldeff(sx, sy);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "don't feel hot!");
            ugolemeffects(AD_FIRE, damage);
            damage = 0;
        }
        else 
        {
            //dam = d(nd, 6);
        }
        burn_away_slime();
        if (burnarmor(&youmonst))
        { /* "body hit" */
            if (!rn2(3))
                destroy_item(POTION_CLASS, AD_FIRE);
            if (!rn2(3))
                destroy_item(SCROLL_CLASS, AD_FIRE);
            if (!rn2(5))
                destroy_item(SPBOOK_CLASS, AD_FIRE);
            destroy_item(FOOD_CLASS, AD_FIRE);
            item_destruction_hint(AD_FIRE, TRUE);
        }
        break;
    case ZT_COLD:
        hit_tile = HIT_FROZEN;
        if (Cold_immunity || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            u_shieldeff(); // shieldeff(sx, sy);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "don't feel cold.");
            ugolemeffects(AD_COLD, damage);
            damage = 0;
        } 
        else 
        {
            //dam = d(nd, 6);
        }
        if (!rn2(3))
        {
            destroy_item(POTION_CLASS, AD_COLD);
            item_destruction_hint(AD_COLD, TRUE);
        }
        break;
    case ZT_SLEEP:
        damage = 0;
        if (Sleep_resistance)
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            u_shieldeff();
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "don't feel sleepy.");
        }
        else 
        {
            display_u_being_hit(HIT_SLEEP, 0, 0UL);
            fall_asleep(-rn1(5, 8), TRUE); /* sleep ray */
            newsym_with_flags(u.ux, u.uy, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS_AND_FLAGS);
        }
        break;
    case ZT_DISINTEGRATION:
        damage = 0;
        if (Disint_resistance || is_incorporeal(youmonst.data) || Invulnerable) // || (magic_resistance_success && !(abstyp >= 20 && abstyp <= 39))
        {                    // if (abstyp == ZT_BREATH(ZT_DISINTEGRATION)) {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "are not disintegrated.");
            break;
        }
        else if (uarms)
        {
            /* destroy shield; other possessions are safe */
            (void) destroy_arm(uarms);
            break;
        } 
        else if (uarm) 
        {
            /* destroy suit; if present, cloak and robe go too */
            if (uarmc)
                (void) destroy_arm(uarmc);
            if (uarmo)
                (void)destroy_arm(uarmo);
            (void) destroy_arm(uarm);
            break;
        }
        /* no shield or suit, you're dead; wipe out cloak
            and/or shirt in case of life-saving or bones */
        if (uarmc)
            (void) destroy_arm(uarmc);
        if (uarmo)
            (void)destroy_arm(uarmo);
        if (uarmu)
            (void) destroy_arm(uarmu);
        killer.hint_idx = HINT_KILLED_DISINTEGRATION_RAY;
        killer.format = KILLED_BY_AN;
        Strcpy(killer.name, killername);
        /* when killed by disintegration attack, don't leave corpse */
        u.ugrave_arise = (abstyp % 10 == ZT_DISINTEGRATION) ? -3 : NON_PM;
        display_u_being_hit(HIT_DISINTEGRATED, 0, 0UL);
        done(DISINTEGRATION);
        return; /* lifesaved */
    case ZT_DEATH:
        damage = 0;
        if (resists_death(&youmonst) || Death_resistance || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            u_shieldeff(); // (sx, sy);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "seem unaffected.");
            break;
        }
#if 0
        else if ((Antimagic || check_magic_resistance_and_inflict_damage(&youmonst, origobj, 12, 0, 0, NOTELL)) 
            && !(abstyp >= 20 && abstyp <= 39)) /* Antimagic does not work breath weapons and eyestalks, just spells and wands */
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            shieldeff(sx, sy);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "aren't affected.");
            break;
        }
#endif
        killer.format = KILLED_BY_AN;
        Strcpy(killer.name, killername);
        killer.hint_idx = HINT_KILLED_DEATH_RAY;
        display_u_being_hit(HIT_DEATH, 0, 0UL);
        done(DIED);
        return; /* lifesaved */
    case ZT_PETRIFICATION:
        damage = 0;
        if (Stoned || Stone_resistance)
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            u_shieldeff(); // shieldeff(sx, sy);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "aren't affected.");
        }
        else
        {
            display_u_being_hit(HIT_PETRIFIED, 0, 0UL);
            if (Stoned)
            {
                if ((Stoned & TIMEOUT) > 1)
                {
                    Stoned = Stoned - 1;
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "feel a bit more stiff.");
                }
                else
                {
                    play_sfx_sound(SFX_GENERAL_UNAFFECTED);
                    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "aren't affected.");
                }
            }
            else if (poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM))
            {
                //Nothing more
            }
            else
            {
                int kformat = KILLED_BY_AN;
                char kname[BUFSZ];
                Strcpy(kname, killername);
                make_stoned(5L, (char*)0, kformat, kname, HINT_KILLED_PETRIFICATION_RAY);
            }
        }
        return;
    case ZT_LIGHTNING:
        hit_tile = HIT_ELECTROCUTED;
        if (Shock_immunity || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            u_shieldeff(); // shieldeff(sx, sy);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "aren't affected.");
            ugolemeffects(AD_ELEC, damage);
            damage = 0;
        } 
        else 
        {
            //dam = d(nd, 6);
            exercise(A_CON, FALSE);
        }
        if (!rn2(3))
            destroy_item(WAND_CLASS, AD_ELEC);
        if (!rn2(3))
            destroy_item(RING_CLASS, AD_ELEC);
        item_destruction_hint(AD_ELEC, TRUE);
        break;
    case ZT_POISON_GAS:
        damage = 0;
        poisoned("blast", A_DEX, "poisoned blast", 15, FALSE, dmgdice ? dmgdice : 4);
        break;
    case ZT_ACID:
        hit_tile = HIT_SPLASHED_ACID;
        if (Acid_immunity || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            pline_The_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s doesn't hurt.", hliquid("acid"));
            damage = 0;
        }
        else 
        {
            pline_The("%s burns!", hliquid("acid"));
            //dam = d(nd, 6);
            exercise(A_STR, FALSE);
        }
        /* using two weapons at once makes both of them more vulnerable */
        if (!rn2(u.twoweap ? 3 : 6))
            acid_damage(uwep);
        if (u.twoweap && uarms && !rn2(3))
            acid_damage(uarms);
        if (!rn2(6))
            erode_armor(&youmonst, ERODE_CORRODE);
        break;
    }

    /* Half_spell_damage protection yields half-damage for wands & spells,
       including hero's own ricochets; breath attacks do full damage */
    //if (dam && Half_spell_damage && !(abstyp >= 20 && abstyp <= 29))
    //    dam = (dam + 1) / 2;
    if (damage > 0)
    {
        int hpbefore = Upolyd ? u.mh : u.uhp;
        losehp(damage, killername, KILLED_BY_AN);
        int hpafter = Upolyd ? u.mh : u.uhp;
        int damagedealt = hpbefore - hpafter;
        if (damagedealt > 0)
        {
            You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_red1, "sustain %d damage!", damagedealt);
            display_u_being_hit(hit_tile, damagedealt, 0UL);
        }
    }
    return;
}

/*
 * burn objects (such as scrolls and spellbooks) on floor
 * at position x,y; return the number of objects burned
 */
int
burn_floor_objects(x, y, give_feedback, u_caused)
int x, y;
boolean give_feedback; /* caller needs to decide about visibility checks */
boolean u_caused;
{
    struct obj *obj, *obj2;
    long i, scrquan, delquan;
    char buf1[BUFSZ], buf2[BUFSZ];
    int cnt = 0;

    for (obj = level.objects[x][y]; obj; obj = obj2) 
    {
        obj2 = obj->nexthere;
        if (obj->oclass == SCROLL_CLASS || obj->oclass == SPBOOK_CLASS
            || (obj->oclass == FOOD_CLASS
                && obj->otyp == GLOB_OF_GREEN_SLIME))
        {
            if (oresist_fire(obj) || obj_resists(obj, 2, 100))
                continue;
            scrquan = obj->quan; /* number present */
            delquan = 0L;        /* number to destroy */
            for (i = scrquan; i > 0L; i--)
                if (!rn2(3))
                    delquan++;

            if (delquan) 
            {
                /* save name before potential delobj() */
                if (give_feedback) 
                {
                    obj->quan = 1L;
                    Strcpy(buf1, (x == u.ux && y == u.uy)
                                     ? xname(obj)
                                     : distant_name(obj, xname));
                    obj->quan = 2L;
                    Strcpy(buf2, (x == u.ux && y == u.uy)
                                     ? xname(obj)
                                     : distant_name(obj, xname));
                    obj->quan = scrquan;
                }
                /* useupf(), which charges, only if hero caused damage */
                if (u_caused)
                    useupf_with_flags(obj, delquan, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
                else if (delquan < scrquan)
                    obj->quan -= delquan;
                else
                    delobj_with_flags(obj, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
                cnt += delquan;
                if (give_feedback)
                {
                    if (delquan > 1L)
                        pline("%ld %s burn.", delquan, buf2);
                    else
                        pline("%s burns.", An(buf1));
                }
            }
        }
    }
    return cnt;
}

/* will zap/spell/breath attack score a hit against armor class `ac'? */
STATIC_OVL int
zap_hit(ac, type, origobj, origmonst)
int ac;
int type;
struct obj* origobj;
struct monst* origmonst;
{
    int chance = rn2(20);
    int dex_bonus = origobj ? m_spell_hit_dex_bonus(origmonst, origobj->otyp) : 0;
    int spell_bonus = origobj && origobj->oclass == SPBOOK_CLASS ? m_spell_hit_skill_bonus(origmonst, origobj->otyp) : 0;
    int wand_bonus = origobj && origobj->oclass == WAND_CLASS ? m_wand_hit_skill_bonus(origmonst, origobj->otyp) : 0;
    int accuracy_bonus = type >=0 ? u.uhitinc : 0;

    /* small chance for naked target to avoid being hit */
    if (!chance)
        return 4 + rnd(10) < ac + spell_bonus + wand_bonus + accuracy_bonus;

    /* very high armor protection does not achieve invulnerability */
    ac = AC_VALUE(ac);

    return (7 - chance < ac + dex_bonus + spell_bonus + wand_bonus + accuracy_bonus);
}

boolean
check_rider_disintegration(mon, fltxt)
struct monst* mon;
const char* fltxt;
{
    /* Rider non-disintegration */
    if (mon && is_rider(mon->data))
    {
        if (canseemon(mon))
        {
            if(fltxt && strcmp(fltxt, ""))
                hit(fltxt, mon, ".", -1, "");

            play_sfx_sound_at_location(SFX_DISINTEGRATE, mon->mx, mon->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s disintegrates.", Monnam(mon));
            pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s body reintegrates before your %s!",
                s_suffix(Monnam(mon)),
                (eyecount(youmonst.data) == 1)
                ? body_part(EYE)
                : makeplural(body_part(EYE)));
            pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s resurrects!", Monnam(mon));
        }
        mon->mhp = mon->mhpmax;
        return TRUE;
    }
    return FALSE;
}


boolean
check_rider_death_absorption(mon, fltxt)
struct monst* mon;
const char* fltxt;
{
    /* Death gets stronger */
    if (mon && mon->data == &mons[PM_DEATH])
    {
        if (canseemon(mon)) 
        {
            if (fltxt && strcmp(fltxt, ""))
                hit(fltxt, mon, ".", -1, "");

            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s absorbs the deadly magics!", Monnam(mon));
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "It seems even stronger than before.");
        }
        mon->mbasehpmax += mon->mbasehpmax / 2;
        if (mon->mbasehpmax > DEATH_MAX_HP_FROM_DEATH_RAY)
            mon->mbasehpmax = DEATH_MAX_HP_FROM_DEATH_RAY;
        update_mon_maxhp(mon);
        mon->mhp = mon->mhpmax;
        return TRUE;
    }
    return FALSE;
}


void
maybe_disintegrate_mon(mon, type, fltxt)
struct monst* mon;
int type; /* hero vs other */ /* 100 == disintegrate just items and caller takes care of the killing and messaging*/
const char* fltxt;
{
    if (!mon || DEADMONSTER(mon))
        return;

    /* Rider non-disintegration */
    if (check_rider_disintegration(mon, fltxt))
    {
        /* No further action */
    }
    else if (resists_disint(mon))
    {
        /* should never get here if the resistance and magic resistance are properly checked earlier */
        if (canspotmon(mon))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
            m_shieldeff(mon);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is unaffected!", Monnam(mon));
        }
    }
    else
    {
        disintegrate_mon(mon, type, fltxt);
    }

}

void //STATIC_OVL 
disintegrate_mon(mon, type, fltxt)
struct monst *mon;
int type; /* hero vs other */ /* 100 == disintegrate just items and caller takes care of the killing and messaging */
const char *fltxt;
{
    if (!mon)
        return;

    struct obj *otmp, *otmp2, *m_amulet = mlifesaver(mon);

    play_sfx_sound_at_location(SFX_DISINTEGRATE, mon->mx, mon->my);

    if (canseemon(mon)) 
    {
        if (!m_amulet)
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is disintegrated!", Monnam(mon));
            display_m_being_hit(mon, HIT_DISINTEGRATED, 0, 0UL, FALSE);
        }
        else
            hit(fltxt, mon, "!", -1, "");
    }
    
    for (otmp = mon->minvent; otmp; otmp = otmp2) 
    {
        otmp2 = otmp->nobj;
        /* note: worn amulet of life saving must be preserved in order to operate */
        if (!oresist_disintegration(otmp) && otmp != m_amulet) 
        {
            if (otmp->owornmask) 
            {
                /* in case monster's life gets saved */
                mon->worn_item_flags &= ~otmp->owornmask;
                if (otmp->owornmask & W_WEP)
                    setmnotwielded(mon, otmp);
                /* also dismounts hero if this object is steed's saddle */
                otmp->owornmask = 0L;
                update_all_mon_statistics(mon, TRUE);
                if (mon == u.usteed && otmp->otyp == SADDLE)
                    dismount_steed(DISMOUNT_FELL);
            }
            obj_extract_self(otmp);
            obfree(otmp, (struct obj *) 0);
        }
    }

    if (type < 0)
        monkilled(mon, (char*)0, abs(type) + 1, XKILL_NOCORPSE | XKILL_DISINTEGRATED);
    else
        xkilled(mon, XKILL_NOMSG | XKILL_NOCORPSE | XKILL_DISINTEGRATED);
}

void
buzz(type, origobj, origmonst, dmgdice, dicesize, dmgplus, sx, sy, dx, dy)
int type;
struct obj* origobj; //Originating item or spell, null if breath weapon
struct monst* origmonst; //Originating monst, null if god, trap, etc.
int dmgdice, dicesize, dmgplus;         //Damage ndd+p, used only for breath weapons
xchar sx, sy;
int dx, dy;
{
    dobuzz(type, origobj, origmonst, dmgdice, dicesize, dmgplus, sx, sy, dx, dy, TRUE);
}

boolean
is_buzztype_breath_weapon(type)
int type;
{
    if (abs(type) >= 20 && abs(type) < 30)
        return TRUE;

    return FALSE;
}

boolean
is_buzztype_eyestalk(type)
int type;
{
    if (abs(type) >= 30 && abs(type) < 40)
        return TRUE;

    return FALSE;
}

/*
 * type ==   0 to   9 : you shooting a wand
 * type ==  10 to  19 : you casting a spell
 * type ==  20 to  29 : you breathing as a monster
 * type ==  30 to  39 : you firing an eyestalk as a monster
 * type == -10 to -19 : monster casting spell
 * type == -20 to -29 : monster breathing at you
 * type == -30 to -39 : monster firing an eyestalk
 * type == -40 to -49 : monster shooting a wand
 * called with dx = dy = 0 with vertical bolts
 */
void
dobuzz(type, origobj, origmonst, dmgdice, dicesize, dmgplus, sx, sy, dx, dy, say)
register int type;
struct obj* origobj; //Originating item or spell, null if breath weapon
struct monst* origmonst; //Originating monst, null if god, trap, etc.
int dmgdice, dicesize, dmgplus;         //Damage ndd+p, used only for breath weapons
register xchar sx, sy;
register int dx, dy;
boolean say; /* Announce out of sight hit/miss events if true */
{
    int i, range, abstype = abs(type) % NUM_ZAP;
    register xchar lsx = 0, lsy = 0;
    struct monst *mon;
    coord save_bhitpos;
    boolean shopdamage = FALSE;
    const char *fltxt;
    struct obj *otmp;
    uchar zhitm_out_flags = 0;
    int zaptype = 0;
    struct obj origobj_copy = origobj ? *origobj : zeroobj; /* Informatin copied here in the case origobj gets destroyed during buzz */

    zaptype = abstype + (abs(type) >= 20 && abs(type) < 30 ? NUM_ZAP : 0); /* Breathing has a different glyph */

    //Define if explosion effect
    boolean isexplosioneffect = FALSE;
    if (origobj && objects[origobj->otyp].oc_aflags & S1_SPELL_EXPLOSION_EFFECT) // (type == ZT_SPELL(ZT_FIRE));
        isexplosioneffect = TRUE;
    int soundset_id = zaptype;
    fltxt = origobj && object_uses_spellbook_wand_flags_and_properties(origobj) && (OBJ_CONTENT_NAME(origobj->otyp) || origobj->oclass == SPBOOK_CLASS) ?
        (OBJ_CONTENT_NAME(origobj->otyp) && strcmp(OBJ_CONTENT_NAME(origobj->otyp), "") ? OBJ_CONTENT_NAME(origobj->otyp) : OBJ_NAME(objects[origobj->otyp])) 
        : flash_types[(type <= -40) ? abstype : abs(type)];

    play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_CREATE, sx, sy);
    
    if (u.uswallow)
    {
        double damage = 0;

        if (type < 0)
            return;
        damage = zhitm(u.ustuck, type, origobj ? &origobj_copy : 0, origmonst, dmgdice, dicesize, dmgplus, &otmp, &zhitm_out_flags);

        if (!u.ustuck)
            u.uswallow = 0;
        else
            pline("%s rips into %s%s", The(fltxt), mon_nam(u.ustuck),
                  exclam((int)damage));

        /* Using disintegration from the inside only makes a hole... */
        if (zhitm_out_flags & ZHITM_FLAGS_DISINTEGRATION)
        {
            u.ustuck->mhp = 0;
        }
        else if (zhitm_out_flags & ZHITM_FLAGS_PETRIFICATION)
        {
            start_delayed_petrification(u.ustuck, TRUE);
            damage = 0;
        }
        if (DEADMONSTER(u.ustuck))
            killed(u.ustuck);
        return;
    }
    if (type < 0)
        newsym(u.ux, u.uy);
    range = (!origobj || objects[origobj_copy.otyp].oc_spell_range <= 0) ? rn1(7, 7) : (int)objects[origobj_copy.otyp].oc_spell_range;
    if (dx == 0 && dy == 0)
        range = 1;
    save_bhitpos = bhitpos;

    /* Start the zap */
    int framenum = 1;
    int anim_intervals = 0;
    context.zap_aggregate_intervals_to_wait_until_end = 0UL;
    context.zap_aggregate_intervals_to_wait_until_action = 0UL;
    for (i = 0; i < MAX_PLAYED_ZAP_ANIMATIONS; i++)
    {
        toggle_animation_timer(ANIMATION_TIMER_ZAP, i, FALSE, 0, 0, 0, 0UL);
        context.zap_animation_counter_on[i] = FALSE;
        //context.zap_animation_counter[i] = 0L;
        context.zap_animation_x[i] = 0;
        context.zap_animation_y[i] = 0;
    }
    enum animation_types anim = zap_type_definitions[zaptype].animation;
    boolean playing_anim = (iflags.using_gui_tiles && anim > 0 && animations[anim].play_type == ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY);
    if (playing_anim)
    {
        framenum = animations[anim].number_of_frames + (animations[anim].main_tile_use_style != ANIMATION_MAIN_TILE_IGNORE ? 1 : 0);
        anim_intervals = framenum * animations[anim].intervals_between_frames;
    }
    int zap_tile_count = 0;
    boolean first_tile_found = FALSE;
    //int prev_anim_counter_idx = -1;

    start_ambient_ray_sound_at_location(soundset_id, sx, sy);
    tmp_at(DISP_BEAM, zapdir_to_glyph(dx, dy, zaptype)); //abstype => zaptype

    while (range-- > 0)
    {
        lsx = sx;
        sx += dx;
        lsy = sy;
        sy += dy;

        if (!isok(sx, sy) || levl[sx][sy].typ == STONE)
            goto make_bounce;

        mon = m_at(sx, sy);
        if (cansee(sx, sy))
        {
            /* reveal/unreveal invisible monsters before tmp_at() */
            if (mon && !canspotmon(mon))
                map_invisible(sx, sy);
            else if (!mon)
                (void) unmap_invisible(sx, sy);

            if (ZAP_POS(levl[sx][sy].typ)
                || (isok(lsx, lsy) && cansee(lsx, lsy)))
            {
                if (playing_anim)
                {
                    int used_count = min(zap_tile_count, MAX_PLAYED_ZAP_ANIMATIONS);
                    int idx = 0;
                    boolean use_old = FALSE;
                    for (idx = 0; idx < used_count; idx++)
                    {
                        if (context.zap_animation_counter_on[idx] == TRUE && context.zap_animation_x[idx] == sx && context.zap_animation_y[idx] == sy)
                        {
                            use_old = TRUE;
                            break;
                        }
                    }

                    context.zap_animation_x[idx] = sx;
                    context.zap_animation_y[idx] = sy;
                    //context.zap_animation_counter[idx] = 0L;
                    context.zap_animation_counter_on[idx] = TRUE;
                    context.zap_aggregate_intervals_to_wait_until_action = 0UL;
                    context.zap_aggregate_intervals_to_wait_until_end = anim_intervals;
                    toggle_animation_timer(ANIMATION_TIMER_ZAP, idx, TRUE, sx, sy, 0, 0UL);

                    if (animations[anim].action_execution_frame > 0)
                    {
                        long intervals_to_execution = (long)(animations[anim].action_execution_frame * animations[anim].intervals_between_frames);
#if 0
                        if (prev_anim_counter_idx > -1 && context.zap_animation_counter_on[prev_anim_counter_idx])
                        {
                            long diff = context.zap_animation_counter[prev_anim_counter_idx] - intervals_to_execution -1; // -1;
                            if (abs((int)diff) <= 3) /* Extra check that something else is not going on */
                            {
                                context.zap_animation_counter[prev_anim_counter_idx] -= diff;
                            }
                        }
#endif
                        context.zap_aggregate_intervals_to_wait_until_action = (unsigned long)intervals_to_execution;
                    }

                    //prev_anim_counter_idx = idx;

                    if(!use_old)
                        zap_tile_count++;
                }
                remove_glyph_buffer_layer_flags(sx, sy, LFLAGS_ZAP_TRAILING_EDGE, 0UL);
                if (!first_tile_found)
                {
                    add_glyph_buffer_layer_flags(sx, sy, LFLAGS_ZAP_TRAILING_EDGE, 0UL);
                    first_tile_found = TRUE;
                }
                remove_glyph_buffer_layer_flags(lsx, lsy, LFLAGS_ZAP_LEADING_EDGE, 0UL);
                add_glyph_buffer_layer_flags(sx, sy, LFLAGS_ZAP_LEADING_EDGE, 0UL);
                tmp_at(sx, sy);
                force_redraw_at(sx, sy);
                if (animations[anim].sound_play_frame > 0)
                {
                    delay_output_intervals((int)animations[anim].sound_play_frame * animations[anim].intervals_between_frames);
                }
                update_ambient_ray_sound_to_location(soundset_id, sx, sy);
            }
        }
        else
        {
            if (playing_anim)
            {
                /* Wait as though the zap would be going forward */
                if (animations[anim].action_execution_frame > 0)
                {
                    long intervals_to_execution = (long)(animations[anim].action_execution_frame * animations[anim].intervals_between_frames);
#if 0
                    if (prev_anim_counter_idx > -1 && context.zap_animation_counter_on[prev_anim_counter_idx])
                    {
                        long diff = context.zap_animation_counter[prev_anim_counter_idx] - intervals_to_execution - 1;
                        if (abs((int)diff) <= 3) /* Extra check that something else is not going on */
                        {
                            context.zap_animation_counter[prev_anim_counter_idx] -= diff;
                        }
                    }
#endif
                    context.zap_aggregate_intervals_to_wait_until_action = (unsigned long)intervals_to_execution;
                }
                if (animations[anim].sound_play_frame > 0)
                {
                    delay_output_intervals((int)animations[anim].sound_play_frame * animations[anim].intervals_between_frames);
                }
            }
            update_ambient_ray_sound_to_location(soundset_id, sx, sy);
        }

        /* hit() and miss() need bhitpos to match the target */
        bhitpos.x = sx, bhitpos.y = sy;
        /* Fireballs only damage when they explode */
        if (!isexplosioneffect) //type != ZT_SPELL(ZT_FIRE)) {
        {
            context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_ZAP_GLYPH;
            range += zap_over_floor(sx, sy, type, &shopdamage, 0);
            context.global_newsym_flags = 0UL;
            /* zap with fire -> melt ice -> drown monster, so monster
               found and cached above might not be here any more */
            mon = m_at(sx, sy);
        }

        if (mon)
        {
            /* Ray hits a monster */
            
            if (isexplosioneffect) //type == ZT_SPELL(ZT_FIRE))
                break;
            
            if (type >= 0)
                mon->mstrategy &= ~STRAT_WAITMASK;

        buzzmonst:
            
            notonhead = (mon->mx != bhitpos.x || mon->my != bhitpos.y);
            context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_ZAP_GLYPH;
            if (zap_hit(find_mac(mon), type, origobj ? &origobj_copy : 0, origmonst))
            {
                if (mon_reflects(mon, (char *) 0))
                {
                    /* Ray is reflected */
                    play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_BOUNCE, mon->mx, mon->my);
                    tmp_at(DISP_CHANGE, zapbounce_to_glyph(dx, dy, -dx, -dy, zaptype));
                    tmp_at(sx, sy);
                    if (cansee(mon->mx, mon->my))
                    {
                        hit(fltxt, mon, exclam(0), -1, "");
                        play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mon->mx, mon->my);
                        m_shieldeff(mon);
                        (void) mon_reflects(mon,
                                            "But it reflects from %s %s!");
                    }
                    dx = -dx;
                    dy = -dy;
                    tmp_at(DISP_CHANGE, zapdir_to_glyph(dx, dy, zaptype));
                }
                else 
                {
                    /* Ray is not reflected */
                    //boolean mon_could_move = mon_can_move(mon);
                    
                    /* Ray does damage and actually reduces mon's hit points */
                    play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_HIT_MONSTER, mon->mx, mon->my);
                    double damage = zhitm(mon, type, origobj ? &origobj_copy : 0, origmonst, dmgdice, dicesize, dmgplus, &otmp, &zhitm_out_flags);
                    boolean show_hit_tile = TRUE;
                    if (zhitm_out_flags & ZHITM_FLAGS_DO_NOT_SHOW_HIT_TILE)
                    {
                        show_hit_tile = FALSE;
                    }

                    /* Normal hit text */
                    if (say || canseemon(mon))
                    {
                        boolean hide_damage = !!(zhitm_out_flags & ZHITM_FLAGS_HIDE_DAMAGE);
                        boolean is_crit = !!(zhitm_out_flags & ZHITM_FLAGS_CRITICAL_STRIKE);
                        enum hit_tile_types htile = get_hit_tile_by_adtyp(abstype + 1);
                        if (htile == HIT_GENERAL && is_crit)
                            htile = HIT_CRITICAL;

                        hit_with_hit_tile(fltxt, mon, exclam((int)ceil(damage)), hide_damage ? 0 : (int)ceil(damage), is_crit ? " critically" : "", htile, show_hit_tile);
                    }

                    /* Rider non-disintegration */
                    if (abstype == ZT_DISINTEGRATION && check_rider_disintegration(mon, fltxt))
                    {
                        break; /* Out of while loop */
                    }

                    /* Death grows stronger */
                    if (abstype == ZT_DEATH && mon->mnum == PM_DEATH)
                    {
                        if (canseemon(mon)) 
                        {
                            hit(fltxt, mon, ".", -1, "");
                            pline("%s absorbs the deadly %s!", Monnam(mon),
                                type == ZT_BREATH(ZT_DEATH) ? "blast"
                                : "ray");
                            pline("It seems even stronger than before.");
                        }
                    }

                    /* Disintegrate */
                    if (zhitm_out_flags & ZHITM_FLAGS_DISINTEGRATION)
                    { /* disintegration */
                        disintegrate_mon(mon, type, fltxt);
                    }
                    else if (DEADMONSTER(mon)) 
                    {
                        /* Already dead monsters */
                        if (type < 0)
                        {
                            /* mon has just been killed by another monster */
                            monkilled(mon, fltxt, abstype + 1, 0);
                        } 
                        else 
                        {
                            int xkflags = XKILL_GIVEMSG; /* killed(mon); */

                            /* killed by hero; we know 'type' isn't negative;
                               if it's fire, highly flammable monsters leave
                               no corpse; don't bother reporting that they
                               "burn completely" -- unnecessary verbosity */
                            if ((abstype == ZT_FIRE)
                                /* paper golem or straw golem */
                                && completelyburns(mon->data))
                                xkflags |= XKILL_NOCORPSE;
                            xkilled(mon, xkflags);
                        }
                    }
                    else
                    {
                        /* Disintegration ray hit a piece of armor, otmp */
                        if (otmp)
                        {
                            /* some armor was destroyed; no damage done */
                            play_sfx_sound_at_location(SFX_DISINTEGRATE, mon->mx, mon->my);
                            if (canseemon(mon))
                                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s is disintegrated!",
                                      s_suffix(Monnam(mon)),
                                      distant_name(otmp, xname));
                            m_useup(mon, otmp);
                        }
                        if (zhitm_out_flags & ZHITM_FLAGS_SLEEP) // (mon_could_move && !mon_can_move(mon)) /* ZT_SLEEP */
                            slept_monst(mon);
                        if (zhitm_out_flags & ZHITM_FLAGS_PETRIFICATION)
                            start_delayed_petrification(mon, TRUE);
                    }
                    tmp_at(sx, sy);
                    force_redraw_at(sx, sy);
                    flush_screen(1);
                }
                range -= 2;
            } 
            else 
            {
                if (say || canseemon(mon))
                    miss(fltxt, mon);
            }
            context.global_newsym_flags = 0UL;
        } 
        else if (sx == u.ux && sy == u.uy && range >= 0) 
        {
            /* Ray hits you or your steed */
            nomul(0);
            context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_ZAP_GLYPH;
            if (u.usteed && !rn2(3) && !mon_reflects(u.usteed, (char *) 0))
            {
                mon = u.usteed;
                context.global_newsym_flags = 0UL;
                goto buzzmonst;
            } 
            else if (zap_hit((int) u.uac, -1, (struct obj*)0, origmonst)) /* No accuracy or skill bonus for hitting yourself */
            {
                range -= 2;
                pline("%s hits you!", The(fltxt));
                if (Reflecting) 
                {
                    play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_BOUNCE, sx, sy);
                    tmp_at(DISP_CHANGE, zapbounce_to_glyph(dx, dy, -dx, -dy, zaptype));
                    tmp_at(sx, sy);
                    play_sfx_sound(SFX_GENERAL_REFLECTS);
                    if (!Blind)
                    {
                        (void) ureflects("But %s reflects from your %s!",
                                         "it");
                    } else
                        pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "For some reason you are not affected.");
                    dx = -dx;
                    dy = -dy;
                    u_shieldeff(); // shieldeff(sx, sy);
                    tmp_at(DISP_CHANGE, zapdir_to_glyph(dx, dy, zaptype));
                }
                else 
                {
                    play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_HIT_MONSTER, sx, sy);
                    zhitu(type, origobj ? &origobj_copy : 0, origmonst, dmgdice, dicesize, dmgplus, fltxt);
                }
            } 
            else if (!Blind) 
            {
                play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_WHIZZES_BY_YOU, sx, sy);
                pline("%s whizzes by you!", The(fltxt));
            }
            else if (abstype == ZT_LIGHTNING)
            {
                play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_WHIZZES_BY_YOU, sx, sy);
                Your("%s tingles.", body_part(ARM));
            }

            /* Special effects */
            if (abstype == ZT_LIGHTNING)
            {
                if (origobj)
                {
                    long dam = (long)get_spell_damage(origobj_copy.otyp, origmonst, &youmonst);
                    (void)flashburn(dam);
                }
                else
                    (void)flashburn((long)d(dmgdice, dicesize) + dmgplus);
            }

            stop_occupation();
            nomul(0);
            context.global_newsym_flags = 0UL;
        }

        if (!ZAP_POS(levl[sx][sy].typ)
            || (closed_door(sx, sy) && range >= 0)) 
        {
            uchar rmn;
            boolean fireball = FALSE;
            int bchance = 0;
            int bounce = 0;

        make_bounce:
            bchance = (!isok(sx, sy) || levl[sx][sy].typ == STONE) ? 10
                : (In_mines(&u.uz) && IS_WALL(levl[sx][sy].typ)) ? 20
                : 75;

            bounce = 0;

            fireball = isexplosioneffect;

            if ((--range > 0 && isok(lsx, lsy) && cansee(lsx, lsy))
                || fireball)
            {
                if (Is_airlevel(&u.uz))
                { /* nothing to bounce off of */
                    pline_The("%s vanishes into the aether!", fltxt);
//                    if (fireball) //No need to do anything here
//                        type = ZT_WAND(ZT_FIRE); /* skip pending fireball */
                    break;
                }
                else if (fireball) 
                {
                    sx = lsx;
                    sy = lsy;
                    break; /* fireballs explode before the obstacle */
                }
                else
                {
                    play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_BOUNCE, lsx, lsy);
                    pline_The("%s bounces!", fltxt);
                }
            }

            if (!dx || !dy || !rn2(bchance))
            {
                dx = -dx;
                dy = -dy;
                tmp_at(DISP_CHANGE, zapbounce_to_glyph(-dx, -dy, dx, dy, zaptype));
                tmp_at(sx, sy);
                tmp_at(DISP_CHANGE, zapdir_to_glyph(dx, dy, zaptype));
            } 
            else 
            {
                int orig_dx = dx, orig_dy = dy;
                if (isok(sx, lsy) && ZAP_POS(rmn = levl[sx][lsy].typ)
                    && !closed_door(sx, lsy)
                    && (IS_ROOM(rmn) || (isok(sx + dx, lsy)
                                         && ZAP_POS(levl[sx + dx][lsy].typ))))
                    bounce = 1;
                if (isok(lsx, sy) && ZAP_POS(rmn = levl[lsx][sy].typ)
                    && !closed_door(lsx, sy)
                    && (IS_ROOM(rmn) || (isok(lsx, sy + dy)
                                         && ZAP_POS(levl[lsx][sy + dy].typ))))
                    if (!bounce || rn2(2))
                        bounce = 2;

                switch (bounce)
                {
                case 0:
                    dx = -dx;
                    /*FALLTHRU*/
                case 1:
                    dy = -dy;
                    break;
                case 2:
                    dx = -dx;
                    break;
                }

                if (bounce >= 0)
                {
                    tmp_at(DISP_CHANGE, zapbounce_to_glyph(orig_dx, orig_dy, dx, dy, zaptype));
                    tmp_at(sx, sy);
                }
                tmp_at(DISP_CHANGE, zapdir_to_glyph(dx, dy, zaptype));  //abstype changed to zaptype
            }
        }

        if (iflags.using_gui_tiles)
        {
            if (playing_anim)
            {
                delay_output_intervals((int)context.zap_aggregate_intervals_to_wait_until_action);
                context.zap_aggregate_intervals_to_wait_until_action = 0UL;
            }
            else
                adjusted_delay_output();
        }
        else
        {
            if(isok(sx, sy) && cansee(sx, sy))
                adjusted_delay_output(); /* wait a little */
        }

    }
    if (!isexplosioneffect && context.zap_aggregate_intervals_to_wait_until_end > 0)
    {
        delay_output_intervals((int)context.zap_aggregate_intervals_to_wait_until_end);
    }
    context.zap_aggregate_intervals_to_wait_until_end = 0UL;

    tmp_at(DISP_END, 0);
    stop_ambient_ray_sound(soundset_id);
    play_immediate_ray_sound_at_location(soundset_id, RAY_SOUND_TYPE_DESTROY, lsx, lsy);
    for (i = 0; i < MAX_PLAYED_ZAP_ANIMATIONS; i++)
    {
        toggle_animation_timer(ANIMATION_TIMER_ZAP, i, FALSE, 0, 0, 0, 0UL);
        context.zap_animation_counter_on[i] = FALSE;
        //context.zap_animation_counter[i] = 0L;
        context.zap_animation_x[i] = 0;
        context.zap_animation_y[i] = 0;
    }

    if (isexplosioneffect) //type == ZT_SPELL(ZT_FIRE))
    {
        int expltype = 0;
        switch (abstype)
        {
        case ZT_COLD:
            expltype = EXPL_FROSTY;
            break;
        case ZT_FIRE:
            expltype = EXPL_FIERY;
            break;
        case ZT_POISON_GAS:
        case ZT_ACID:
            expltype = EXPL_NOXIOUS;
            break;
        case ZT_MAGIC_MISSILE:
        case ZT_SLEEP:
        case ZT_DEATH:
        case ZT_PETRIFICATION:
        case ZT_DISINTEGRATION:
        default:
            expltype = EXPL_MAGICAL;
            break;
        }

        if(origobj && origobj->otyp == SPE_METEOR_SWARM)
            expltype = EXPL_METEOR_SWARM;

        int otyp = 0;
        int oclass = 0;
        if (origobj)
        {
            otyp = origobj_copy.otyp;
            oclass = origobj_copy.oclass;
        }

        if (origobj)
            explode(sx, sy, type, origmonst, objects[origobj_copy.otyp].oc_wsdice, objects[origobj_copy.otyp].oc_wsdam, objects[origobj_copy.otyp].oc_wsdmgplus, otyp, oclass, expltype);
        else
            explode(sx, sy, type, origmonst, dmgdice, dicesize, dmgplus, otyp, oclass, expltype);
    }
    if (shopdamage)
        pay_for_damage(abstype == ZT_FIRE
                          ? "burn away"
                          : abstype == ZT_COLD
                             ? "shatter"
                             /* "damage" indicates wall rather than door */
                             : abstype == ZT_ACID
                                ? "damage"
                                : abstype == ZT_DISINTEGRATION
                                   ? "disintegrate"
                                  : abstype == ZT_PETRIFICATION
                                     ? "shatter"
                                    : abstype == ZT_DEATH
                                        ? "destroy" : "destroy",
                                                   FALSE);
    bhitpos = save_bhitpos;
}

void
melt_ice(x, y, msg)
xchar x, y;
const char *msg;
{
    struct rm *lev = &levl[x][y];
    struct obj *otmp;
    struct monst *mtmp;

    if (!msg)
        msg = "The ice crackles and melts.";
    if (lev->typ == DRAWBRIDGE_UP || lev->typ == DRAWBRIDGE_DOWN)
    {
        lev->drawbridgemask &= ~DB_ICE; /* revert to DB_MOAT */
    } 
    else 
    { /* lev->typ == ICE */
#ifdef STUPID
        if (lev->icedpool == ICED_POOL)
            lev->typ = POOL;
        else
            lev->typ = MOAT;
#else
        lev->typ = (lev->icedpool == ICED_POOL ? POOL : MOAT);
#endif
        lev->icedpool = 0;
    }
    spot_stop_timers(x, y, MELT_ICE_AWAY); /* no more ice to melt away */
    obj_ice_effects(x, y, FALSE);
    unearth_objs(&youmonst, x, y, FALSE, FALSE);
    if (Underwater)
        vision_recalc(1);
    newsym(x, y);
    if (cansee(x, y))
        Norep("%s", msg);
    if ((otmp = sobj_at(BOULDER, x, y)) != 0) 
    {
        if (cansee(x, y))
            pline("%s settles...", An(xname(otmp)));

        do 
        {
            obj_extract_self(otmp); /* boulder isn't being pushed */
            if (!boulder_hits_pool(otmp, x, y, FALSE))
                impossible("melt_ice: no pool?");
            /* try again if there's another boulder and pool didn't fill */
        } while (is_pool(x, y) && (otmp = sobj_at(BOULDER, x, y)) != 0);
        newsym(x, y);
    }

    if (x == u.ux && y == u.uy)
        spoteffects(TRUE); /* possibly drown, notice objects */
    else if (is_pool(x, y) && (mtmp = m_at(x, y)) != 0)
        (void) minliquid(mtmp);
}

#define MIN_ICE_TIME 50
#define MAX_ICE_TIME 2000
/*
 * Usually start a melt_ice timer; sometimes the ice will become
 * permanent instead.
 */
void
start_melt_ice_timeout(x, y, min_time)
xchar x, y;
long min_time; /* <x,y>'s old melt timeout (deleted by time we get here) */
{
    int when;
    long where;

    when = (int) min_time;
    if (when < MIN_ICE_TIME - 1)
        when = MIN_ICE_TIME - 1;

    /* random timeout; surrounding ice locations ought to be a factor... */
    while (++when <= MAX_ICE_TIME)
        if (!rn2((MAX_ICE_TIME - when) + MIN_ICE_TIME))
            break;

    /* if we're within MAX_ICE_TIME, install a melt timer;
       otherwise, omit it to leave this ice permanent */
    if (when <= MAX_ICE_TIME) 
    {
        where = ((long) x << 16) | (long) y;
        (void) start_timer((long) when, TIMER_LEVEL, MELT_ICE_AWAY,
                           long_to_any(where));
    }
}
#undef MIN_ICE_TIME
#undef MAX_ICE_TIME

/*
 * Called when ice has melted completely away.
 */
void
melt_ice_away(arg, timeout)
anything *arg;
long timeout UNUSED;
{
    xchar x, y;
    long where = arg->a_long;
    boolean save_mon_moving = context.mon_moving; /* will be False */

    /* melt_ice -> minliquid -> mondead|xkilled shouldn't credit/blame hero */
    context.mon_moving = TRUE; /* hero isn't causing this ice to melt */
    y = (xchar) (where & 0xFFFF);
    x = (xchar) ((where >> 16) & 0xFFFF);
    /* melt_ice does newsym when appropriate */
    melt_ice(x, y, "Some ice melts away.");
    context.mon_moving = save_mon_moving;
}

/* Burn floor scrolls, evaporate pools, etc... in a single square.
 * Used both for normal bolts of fire, cold, etc... and for fireballs.
 * Sets shopdamage to TRUE if a shop door is destroyed, and returns the
 * amount by which range is reduced (the latter is just ignored by fireballs)
 */
int
zap_over_floor(x, y, type, shopdamage, exploding_wand_typ)
xchar x, y;
int type;
boolean *shopdamage;
short exploding_wand_typ;
{
    const char *zapverb;
    struct monst *mon;
    struct trap *t;
    struct rm *lev = &levl[x][y];
    boolean see_it = cansee(x, y), yourzap;
    int rangemod = 0, abstype = abs(type) % 10;

    switch (abstype)
    {
    case ZT_FIRE:
        t = t_at(x, y);
        if (t && t->ttyp == WEB) 
        {
            /* a burning web is too flimsy to notice if you can't see it */
            if (see_it)
                Norep("A web bursts into flames!");
            (void) delfloortrap(t);
            if (see_it)
                newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_MISSILE_GLYPH | NEWSYM_FLAGS_KEEP_OLD_ZAP_GLYPH | NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS);
        }

        if (is_ice(x, y)) 
        {
            play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_BURNT);
            melt_ice(x, y, (char *) 0);
        } 
        else if (is_pool(x, y))
        {
            const char *msgtxt = "You hear hissing gas.";

            if (lev->typ != POOL)
            { /* MOAT or DRAWBRIDGE_UP */
                play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_LITTLE_BURNT);
                if (see_it)
                    msgtxt = "Some water evaporates.";
            } 
            else
            {
                rangemod -= 3;
                play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_BURNT);
                create_basic_floor_location(x, y, lev->floortyp ? lev->floortyp : GROUND, lev->floorsubtyp ? lev->floorsubtyp : 0, 0, FALSE);
                /* Wand of fire leaves pits */
                t = maketrap(x, y, PIT, NON_PM, MKTRAP_NO_FLAGS);
                if (t)
                    t->tseen = 1;
                if (see_it)
                    msgtxt = "The water evaporates.";
            }
            Norep("%s", msgtxt);
            if (lev->typ == ROOM)
                newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
        }
        else if (IS_FOUNTAIN(lev->typ))
        {
            play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_BURNT);
            if (see_it)
                pline("Steam billows from the fountain.");
            rangemod -= 1;
            dryup(x, y, type > 0);
        }
        break; /* ZT_FIRE */

    case ZT_COLD:
        if (is_pool(x, y) || is_lava(x, y)) 
        {
            boolean lava = is_lava(x, y),
                    moat = is_moat(x, y);

            if (lev->typ == WATER) 
            {
                /* For now, don't let WATER freeze. */
                if (see_it)
                    pline_The("%s freezes for a moment.", hliquid("water"));
                else
                    You_hear("a soft crackling.");
                rangemod -= 1000; /* stop */
            } 
            else 
            {
                char buf[BUFSZ];
                play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_FROZEN);
                Strcpy(buf, waterbody_name(x, y)); /* for MOAT */
                rangemod -= 3;
                if (lev->typ == DRAWBRIDGE_UP) 
                {
                    lev->drawbridgemask &= ~DB_UNDER; /* clear lava */
                    lev->drawbridgemask |= (lava ? DB_GROUND : DB_ICE);
                } 
                else 
                {
                    lev->icedpool = lava ? 0
                                         : (lev->typ == POOL) ? ICED_POOL
                                                              : ICED_MOAT;
                    lev->typ = lava ? GROUND : ICE;
                    lev->subtyp = 0;
                }

                bury_objs(x, y);

                if (see_it) 
                {
                    if (lava)
                        Norep("The %s cools and solidifies.", hliquid("lava"));
                    else if (moat)
                        Norep("The %s is bridged with ice!", buf);
                    else
                        Norep("The %s freezes.", hliquid("water"));
                    newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
                } else if (!lava)
                    You_hear("a crackling sound.");

                if (x == u.ux && y == u.uy) 
                {
                    if (u.uinwater) 
                    { /* not just `if (Underwater)' */
                        /* leave the no longer existent water */
                        u.uinwater = 0;
                        u.uundetected = 0;
                        docrt();
                        vision_full_recalc = 1;
                        play_environment_ambient_sounds();
                    }
                    else if (u.utrap && u.utraptype == TT_LAVA)
                    {
                        if (Passes_walls)
                        {
                            You("pass through the now-solid rock.");
                            reset_utrap(TRUE);
                        }
                        else 
                        {
                            set_utrap(rn1(50, 20), TT_INFLOOR);
                            You("are firmly stuck in the cooling rock.");
                        }
                    }
                } 
                else if ((mon = m_at(x, y)) != 0) 
                {
                    /* probably ought to do some hefty damage to any
                       non-ice creature caught in freezing water;
                       at a minimum, eels are forced out of hiding */
                    if (is_swimmer(mon->data) && mon->mundetected) 
                    {
                        mon->mundetected = 0;
                        newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
                    }
                }
                if (!lava)
                {
                    start_melt_ice_timeout(x, y, 0L);
                    obj_ice_effects(x, y, TRUE);
                }
            } /* ?WATER */

        }
        else if (is_ice(x, y))
        {
            long melt_time;

            /* Already ice here, so just firm it up. */
            /* Now ensure that only ice that is already timed is affected */
            if ((melt_time = spot_time_left(x, y, MELT_ICE_AWAY)) != 0L) 
            {
                spot_stop_timers(x, y, MELT_ICE_AWAY);
                start_melt_ice_timeout(x, y, melt_time);
            }
        }
        break; /* ZT_COLD */

    case ZT_POISON_GAS:
        (void) create_gas_cloud(x, y, REGION_POISON_GAS, 1, 1, 8, 5);
        break;

    case ZT_ACID:
        if (lev->typ == IRONBARS) 
        {
            if ((lev->wall_info & W_NONDIGGABLE) != 0) 
            {
                if (see_it)
                    Norep("The %s corrode somewhat but remain intact.",
                          defsyms[S_bars].explanation);
                /* but nothing actually happens... */
            }
            else
            {
                rangemod -= 3;
                if (see_it)
                    Norep("The %s melt.", defsyms[S_bars].explanation);
                if (*in_rooms(x, y, SHOPBASE)) 
                {
                    /* in case we ever have a shop bounded by bars */
                    create_current_floor_location(x, y, 0, NO_GLYPH, FALSE);
                    if (see_it)
                        newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
                    add_damage(x, y, (type >= 0) ? SHOP_BARS_COST : 0L);
                    if (type >= 0)
                        *shopdamage = TRUE;
                }
                else 
                {
                    transform_location_type_and_flags(x, y, DOOR, 0, D_NODOOR);
                    //lev->typ = DOOR, lev->doormask = D_NODOOR;
                    if (see_it)
                        newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
                }
            }
        }
        break; /* ZT_ACID */

    default:
        break;
    }

    /* set up zap text for possible door feedback; for exploding wand, we
       want "the blast" rather than "your blast" even if hero caused it */
    yourzap = (type >= 0 && !exploding_wand_typ);
    zapverb = "blast"; /* breath attack or wand explosion */
    if (!exploding_wand_typ)
    {
        if (abs(type) < ZT_SPELL(0))
            zapverb = "bolt"; /* wand zap */
        else if (abs(type) < ZT_BREATH(0))
            zapverb = "spell";
    }

    /* secret door gets revealed, converted into regular door */
    if (levl[x][y].typ == SDOOR) 
    {
        cvt_sdoor_to_door(x, y); /* .typ = DOOR */
        /* target spot will now pass closed_door() test below
           (except on rogue level) */
        newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
        if (see_it)
            pline("%s %s reveals a secret door.",
                  yourzap ? "Your" : "The", zapverb);
        else if (Is_really_rogue_level(&u.uz))
            draft_message(FALSE); /* "You feel a draft." (open doorway) */
    }

    /* regular door absorbs remaining zap range, possibly gets destroyed */
    if (closed_door(x, y)) 
    {
        int new_doormask = -1;
        int new_door_subtype = -1;
        boolean block_point = FALSE;
        boolean unblock_point = FALSE;
        enum sfx_sound_types sfx_sound = 0;
        enum location_sound_types break_sound = LOCATION_SOUND_TYPE_BREAK;
        boolean createsplinters = FALSE;
        const char *see_txt = 0, *sense_txt = 0, *hear_txt = 0;

        rangemod = -1000;
        switch (abstype) 
        {
        case ZT_FIRE:
            if (door_subtype_definitions[lev->subtyp].material == MAT_WOOD && !is_door_indestructible_at_ptr(lev))
            {
                new_doormask = D_NODOOR;
                see_txt = "The door is consumed in flames!";
                sense_txt = "smell smoke.";
                break_sound = LOCATION_SOUND_TYPE_BURNT;
            }
            break;
        case ZT_COLD:
            if (door_subtype_definitions[lev->subtyp].material == MAT_WOOD && !is_door_indestructible_at_ptr(lev))
            {
                new_doormask = D_NODOOR;
                see_txt = "The door freezes and shatters!";
                sense_txt = "feel cold.";
                break_sound = LOCATION_SOUND_TYPE_FROZEN;
            }
            break;
        case ZT_DISINTEGRATION:
            if (!is_door_indestructible_at_ptr(lev))
            {
                new_doormask = D_NODOOR;
                see_txt = "The door disintegrates!";
                hear_txt = "crashing wood.";
                sfx_sound = SFX_DISINTEGRATE;
            }
            break;
        case ZT_DEATH:
            if (door_subtype_definitions[lev->subtyp].material == MAT_WOOD && !is_door_indestructible_at_ptr(lev))
            {
                new_doormask = D_BROKEN;
                see_txt = "The door withers away!";
                sense_txt = "feel death lurking nearby.";
            }
            break;
        case ZT_PETRIFICATION:
            if (door_subtype_definitions[lev->subtyp].material == MAT_WOOD && !is_door_indestructible_at_ptr(lev))
            {
                //new_doormask = D_NODOOR;
                see_txt = "The door petrifies!";
                sense_txt = "hear stone cracking.";
                new_door_subtype = DOOR_SUBTYPE_STONE;
                sfx_sound = SFX_PETRIFY;
            }
            break;
        case ZT_LIGHTNING:
            if (door_subtype_definitions[lev->subtyp].material == MAT_WOOD && !is_door_indestructible_at_ptr(lev))
            {
                new_doormask = D_BROKEN;
                see_txt = "The door splinters!";
                hear_txt = "crackling.";
                createsplinters = TRUE;
                break_sound = LOCATION_SOUND_TYPE_ELECTROCUTED;
            }
            break;
        default:
//        def_case:
            if (exploding_wand_typ > 0) 
            {
                /* Magical explosion from misc exploding wand */
                if (exploding_wand_typ == WAN_STRIKING && !is_door_destroyed_by_striking_at_ptr(lev)) {
                    new_doormask = D_BROKEN;
                    see_txt = "The door crashes open!";
                    sense_txt = "feel a burst of cool air.";
                    break;
                }
            }
            if (see_it) 
            {
                /* "the door absorbs the blast" would be
                   inaccurate for an exploding wand since
                   other adjacent locations still get hit */
                if (exploding_wand_typ)
                    pline_The("door remains intact.");
                else
                    pline_The("door absorbs %s %s!", yourzap ? "your" : "the",
                              zapverb);
            }
            else
                You_feel("vibrations.");
            break;
        }

        if (new_doormask >= 0 || new_door_subtype >= 0)
        { /* door gets broken */
            if (sfx_sound > 0)
                play_sfx_sound_at_location(sfx_sound, x, y);
            
            if (sfx_sound == 0)
                play_simple_location_sound(x, y, break_sound);

            if (new_doormask >= 0  && *in_rooms(x, y, SHOPBASE))
            {
                if (type >= 0) 
                {
                    add_damage(x, y, SHOP_DOOR_COST);
                    *shopdamage = TRUE;
                } else /* caused by monster */
                    add_damage(x, y, 0L);
            }

            if (new_doormask >= 0)
            {
                lev->doormask &= ~D_MASK;
                lev->doormask |= new_doormask;
            }

            if (new_door_subtype >= 0)
            {
                if ((door_subtype_definitions[lev->subtyp].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND) == 0 && (door_subtype_definitions[new_door_subtype].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND) != 0)
                    block_point = TRUE;
                if ((door_subtype_definitions[lev->subtyp].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND) != 0 && (door_subtype_definitions[new_door_subtype].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND) == 0)
                    unblock_point = TRUE;

                lev->subtyp = new_door_subtype;
            }

            if(new_doormask >= 0 || unblock_point)
                unblock_vision_and_hearing_at_point(x, y); /* vision */
            
            if (block_point)
                block_vision_and_hearing_at_point(x, y); /* vision */

            if (see_it)
            {
                pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, see_txt);
                newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
            }
            else if (sense_txt) 
            {
                You_ex1(ATR_NONE, CLR_MSG_ATTENTION, sense_txt);
            } 
            else if (hear_txt)
                You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, hear_txt);

            if (createsplinters)
            {
                struct obj* otmp = mksobj_at(PIECE_OF_WOOD, x, y, FALSE, FALSE);
                otmp->quan = d(1, 2);
                otmp->owt = weight(otmp);
            }

            if (picking_at(x, y)) 
            {
                stop_occupation();
                reset_pick();
            }
        }
    }

    if (abstype == ZT_DISINTEGRATION)
    {
        int x2 = x;
        int y2 = y;
    
        if (levl[x][y].typ == TREE)
        {
            rangemod = -1000;
            struct mkroom* r = which_room(x, y);
            int ltype = 0, lsubtype = 0, lvartype = 0;
            if (levl[x][y].floortyp)
                ltype = levl[x][y].floortyp, lsubtype = levl[x][y].floorsubtyp;
            else
            {
                if (r && r->orig_rtype == GARDEN)
                    ltype = GRASS;
                else
                    ltype = ROOM;

                lsubtype = get_initial_location_subtype(ltype);
                lvartype = get_initial_location_vartype(ltype, lsubtype);
            }
            play_sfx_sound_at_location(SFX_DISINTEGRATE, x, y);
            create_simple_location(x, y, ltype, lsubtype, lvartype, 0, 0, 0, 0, 0, FALSE); /* The tree is not broken, since it is disintegrated */
            unblock_vision_and_hearing_at_point(x, y); /* vision */
            newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
            if (cansee(x, y))
            {
                play_special_effect_at(SPECIAL_EFFECT_DISINTEGRATED, 0, x, y, FALSE);
                play_sfx_sound_at_location(SFX_DISINTEGRATE, x, y);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "tree disintegrates!");
                special_effect_wait_until_action(0);
                special_effect_wait_until_end(0);
            }
        }
        else if (find_drawbridge(&x2, &y2) && levl[x2][y2].typ == DRAWBRIDGE_UP)
        {
            rangemod = -1000;
            destroy_drawbridge(x2, y2, TRUE);
        }
    }

    if (OBJ_AT(x, y) && abstype == ZT_FIRE)
        if (burn_floor_objects(x, y, FALSE, type > 0) && couldsee(x, y)) 
        {
            newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
            play_special_effect_at(SPECIAL_EFFECT_PUFF_OF_SMOKE, 0, x, y, FALSE);
            play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, x, y);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s of smoke.", !Blind ? "see a puff" : "smell a whiff");
            special_effect_wait_until_action(0);
            special_effect_wait_until_end(0);
        }

    if ((mon = m_at(x, y)) != 0) 
    {
        wakeup(mon, FALSE);
        if (type >= 0) 
        {
            setmangry(mon, TRUE);
            if (mon->ispriest && *in_rooms(mon->mx, mon->my, TEMPLE))
                ghod_hitsu(mon);
            if (mon->isshk && !*u.ushops)
                hot_pursuit(mon);
        }
    }
    return rangemod;
}

/* fractured by pick-axe or wand of striking */
void
fracture_rock(obj, verbose)
register struct obj *obj; /* no texts here! */
boolean verbose;
{
    xchar x, y;
    boolean by_you = !context.mon_moving;

    if (by_you && get_obj_location(obj, &x, &y, 0) && costly_spot(x, y))
    {
        struct monst *shkp = 0;
        char objroom = *in_rooms(x, y, SHOPBASE);

        if (billable(&shkp, obj, objroom, FALSE)) 
        {
            /* shop message says "you owe <shk> <$> for it!" so we need
               to precede that with a message explaining what "it" is */
            if(verbose)
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "fracture %s %s.", s_suffix(shkname(shkp)), xname(obj));
            breakobj(obj, x, y, TRUE, FALSE); /* charges for shop goods */
        }
    }

    if (by_you && obj->otyp == BOULDER)
        sokoban_guilt();

    if(verbose)
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BREAK);

    obj->otyp = ROCK;
    obj->material = objects[obj->otyp].oc_material;
    obj->oclass = GEM_CLASS;
    obj->quan = (long) rn1(60, 7);
    obj->owt = weight(obj);
    obj->dknown = obj->bknown = obj->rknown = obj->nknown = 0;
    obj->known = objects[obj->otyp].oc_uses_known ? 0 : 1;
    dealloc_oextra(obj);

    if (obj->where == OBJ_FLOOR) 
    {
        obj_extract_self(obj); /* move rocks back on top */
        obj_set_found(obj);
        place_object(obj, obj->ox, obj->oy);
        if (!does_block(obj->ox, obj->oy, &levl[obj->ox][obj->oy]))
            unblock_vision_and_hearing_at_point(obj->ox, obj->oy);
        if (cansee(obj->ox, obj->oy))
            newsym(obj->ox, obj->oy);
    }
}

/* handle statue hit by striking/force bolt/pick-axe */
boolean
pre_break_statue(obj)
register struct obj* obj;
{
    /* [obj is assumed to be on floor, so no get_obj_location() needed] */
    struct trap* trap = t_at(obj->ox, obj->oy);
    struct obj* item;
    boolean by_you = !context.mon_moving;

    if (trap && trap->ttyp == STATUE_TRAP
        && activate_statue_trap(trap, obj->ox, obj->oy, TRUE))
        return FALSE;
    /* drop any objects contained inside the statue */
    while ((item = obj->cobj) != 0) {
        obj_extract_self(item);
        obj_set_found(item);
        place_object(item, obj->ox, obj->oy);
    }
    if (by_you && Role_if(PM_ARCHAEOLOGIST) && obj->special_quality == SPEQUAL_STATUE_HISTORIC)
    {
        play_sfx_sound(SFX_GUILTY);
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "guilty about damaging such a historic statue.");
        adjalign(-1);
    }
    obj->special_quality = 0;
    return TRUE;
}

/* handle statue hit by striking/force bolt/pick-axe */
boolean
break_statue(obj)
register struct obj *obj;
{
    if (pre_break_statue(obj))
    {
        fracture_rock(obj, TRUE); /* Make it into a rubble */
        return TRUE;
    }
    else
        return FALSE;
}

/*
 * destroy_strings[dindx][0:singular,1:plural,2:killer_reason]
 *      [0] freezing potion
 *      [1] boiling potion other than oil
 *      [2] boiling potion of oil
 *      [3] burning scroll
 *      [4] burning spellbook
 *      [5] shocked ring
 *      [6] shocked wand
 * (books, rings, and wands don't stack so don't need plural form;
 *  crumbling ring doesn't do damage so doesn't need killer reason)
 */
const char *const destroy_strings[][3] = {
    /* also used in trap.c */
    { "freezes and shatters", "freeze and shatter", "shattered potion" },
    { "boils and explodes", "boil and explode", "boiling potion" },
    { "ignites and explodes", "ignite and explode", "exploding potion" },
    { "catches fire and burns", "catch fire and burn", "burning scroll" },
    { "catches fire and burns", "", "burning book" },
    { "turns to dust and vanishes", "", "" },
    { "breaks apart and explodes", "", "exploding wand" },
};

/* guts of destroy_item(), which ought to be called maybe_destroy_items();
   caller must decide whether obj is eligible */
void
destroy_one_item(obj, osym, dmgtyp, forcedestroy)
struct obj *obj;
int osym, dmgtyp;
boolean forcedestroy;
{
    long i, cnt, quan;
    int dmg, xresist, skip, dindx;
    const char *mult;
    boolean physical_damage;
    int obj_sound_type = OBJECT_SOUND_TYPE_BREAK;

    physical_damage = FALSE;
    xresist = skip = 0;
    /* lint suppression */
    dmg = dindx = 0;
    quan = 0L;

    switch (dmgtyp) 
    {
    case AD_COLD:
        if (osym == POTION_CLASS && obj->otyp != POT_OIL && !oresist_cold(obj) && !is_obj_protected_by_property(obj, &youmonst, dmgtyp))
        {
            obj_sound_type = OBJECT_SOUND_TYPE_FROZEN;
            quan = obj->quan;
            dindx = 0;
            dmg = rnd(4);
        } else
            skip++;
        break;
    case AD_FIRE:
        xresist = (Fire_immunity && obj->oclass != POTION_CLASS
                   && obj->otyp != GLOB_OF_GREEN_SLIME);
        if (obj->otyp == SPE_BOOK_OF_THE_DEAD) 
        {
            skip++;
            if (!Blind)
            {
                const char* hclr = hcolor_multi_buf2(NH_DARK_RED);
                multicolor_buffer[1] = multicolor_buffer[3] = multicolor_buffer[2];
                pline_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s glows a %s%s%s, but remains intact.",
                    The(xname(obj)), "strange ", hclr, " light");
            }
        }
        else if (oresist_fire(obj) || is_obj_protected_by_property(obj, &youmonst, dmgtyp))
            skip++;

        quan = obj->quan;
        obj_sound_type = OBJECT_SOUND_TYPE_BURNT;
        switch (osym) 
        {
        case POTION_CLASS:
            dindx = (obj->otyp != POT_OIL) ? 1 : 2;
            dmg = rnd(6);
            break;
        case SCROLL_CLASS:
            dindx = 3;
            dmg = 1;
            break;
        case SPBOOK_CLASS:
            dindx = 4;
            dmg = 1;
            break;
        case FOOD_CLASS:
            if (obj->otyp == GLOB_OF_GREEN_SLIME) 
            {
                dindx = 1; /* boil and explode */
                dmg = (obj->owt + 19) / 20;
            }
            else 
            {
                skip++;
            }
            break;
        default:
            skip++;
            break;
        }
        break;
    case AD_ELEC:
        xresist = (Shock_immunity && obj->oclass != RING_CLASS);
        obj_sound_type = OBJECT_SOUND_TYPE_ELECTROCUTED;
        quan = obj->quan;
        switch (osym) 
        {
        case RING_CLASS:
            if (oresist_elec(obj) || is_obj_protected_by_property(obj, &youmonst, dmgtyp))
            {
                skip++;
                break;
            }
            dindx = 5;
            dmg = 0;
            break;
        case WAND_CLASS:
            if (oresist_elec(obj) || is_obj_protected_by_property(obj, &youmonst, dmgtyp))
            {
                skip++;
                break;
            }
#if 0
            if (obj == current_wand) {  skip++;  break;  }
#endif
            dindx = 6;
            dmg = rnd(10);
            break;
        default:
            skip++;
            break;
        }
        break;
    default:
        skip++;
        break;
    }

    if (!skip) 
    {
        if (obj->in_use)
            --quan; /* one will be used up elsewhere */
        for (i = cnt = 0L; i < quan; i++)
            if (forcedestroy || !rn2(3))
                cnt++;

        if (!cnt)
            return;

        mult = (cnt == 1L)
                ? ((quan == 1L) ? "Your"                         /* 1 of 1 */
                                : "One of your")                 /* 1 of N */
                : ((cnt < quan) ? "Some of your"                 /* n of N */
                                : (quan == 2L) ? "Both of your"  /* 2 of 2 */
                                               : "All of your"); /* N of N */

        play_simple_object_sound(obj, obj_sound_type);
        char dcbuf[IBUFSZ] = "";
        Sprintf(dcbuf, "%s %s %s!", mult, xname(obj),
              destroy_strings[dindx][(cnt > 1L)]);
        pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, dcbuf);

        if (osym == POTION_CLASS && dmgtyp != AD_COLD) 
        {
            if (!has_innate_breathless(youmonst.data) || haseyes(youmonst.data))
                potionbreathe(obj, dcbuf);
        }

        if (obj->owornmask) 
        {
            if (obj->owornmask & W_RING) /* ring being worn */
                Ring_gone(obj);
            else
                setnotworn(obj);
        }

        if (obj == current_wand)
            current_wand = 0; /* destroyed */

        for (i = 0; i < cnt; i++)
            useup(obj);

        if (dmg) 
        {
            if (xresist || Invulnerable) 
            {
                You("aren't hurt!");
            } 
            else 
            {
                const char *how = destroy_strings[dindx][2];
                boolean one = (cnt == 1L);

                if (dmgtyp == AD_FIRE && osym == FOOD_CLASS)
                    how = "exploding glob of slime";
                if (physical_damage)
                    dmg = Maybe_Half_Phys(dmg);
                losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), one ? how : (const char *) makeplural(how),
                       one ? KILLED_BY_AN : KILLED_BY);
                exercise(A_STR, FALSE);
            }
        }
    }
}

/* target items of specified class for possible destruction */
void
destroy_item(osym, dmgtyp)
int osym, dmgtyp;
{
    register struct obj *obj;
    int i, deferral_indx = 0;
    /* 1+52+1: try to handle a full inventory; it doesn't matter if
      inventory actually has more, even if everything should be deferred */
    unsigned short deferrals[1 + 52 + 1]; /* +1: gold, overflow */

    (void) memset((genericptr_t) deferrals, 0, sizeof deferrals);
    /*
     * Sometimes destroying an item can change inventory aside from
     * the item itself (cited case was a potion of unholy water; when
     * boiled, potionbreathe() caused hero to transform into were-beast
     * form and that resulted in dropping or destroying some worn armor).
     *
     * Unlike other uses of the object bybass mechanism, destroy_item()
     * can be called multiple times for the same event.  So we have to
     * explicitly clear it before each use and hope no other section of
     * code expects it to retain previous value.
     *
     * Destruction of a ring of levitation or form change which pushes
     * off levitation boots could drop hero onto a fire trap that
     * could destroy other items and we'll get called recursively.  Or
     * onto a trap which transports hero elsewhere, which won't disrupt
     * traversal but could yield message sequencing issues.  So we
     * defer handling such things until after rest of inventory has
     * been processed.  If some other combination of items and events
     * triggers a recursive call, rest of inventory after the triggering
     * item will be skipped by the outer call since the inner one will
     * have set the bypass bits of the whole list.
     *
     * [Unfortunately, death while poly'd into flyer and subsequent
     * rehumanization could also drop hero onto a trap, and there's no
     * straightforward way to defer that.  Things could be improved by
     * redoing this to use two passes, first to collect a list or array
     * of o_id and quantity of what is targetted for destruction,
     * second pass to handle the destruction.]
     */
    bypass_objlist(invent, FALSE); /* clear bypass bit for invent */

    while ((obj = nxt_unbypassed_obj(invent)) != 0) 
    {
        if (obj->oclass != osym)
            continue; /* test only objs of type osym */
        if (obj->oartifact)
            continue; /* don't destroy artifacts */
        if (obj->in_use && obj->quan == 1L)
            continue; /* not available */

        /* if loss of this item might dump us onto a trap, hold off
           until later because potential recursive destroy_item() will
           result in setting bypass bits on whole chain--we would skip
           the rest as already processed once control returns here */
        if (deferral_indx < SIZE(deferrals)
            && ((obj->owornmask != 0L
                 && (objects[obj->otyp].oc_oprop == LEVITATION || objects[obj->otyp].oc_oprop2 == LEVITATION || objects[obj->otyp].oc_oprop3 == LEVITATION
                     || objects[obj->otyp].oc_oprop == FLYING || objects[obj->otyp].oc_oprop2 == FLYING || objects[obj->otyp].oc_oprop3 == FLYING))
                /* destroyed wands and potions of polymorph don't trigger
                   polymorph so don't need to be deferred */
                || (obj->otyp == POT_WATER && u.ulycn >= LOW_PM
                    && (Upolyd ? obj->blessed : obj->cursed)))) 
        {
            deferrals[deferral_indx++] = obj->o_id;
            continue;
        }
        /* obj is eligible; maybe destroy it */
        destroy_one_item(obj, osym, dmgtyp, FALSE);
    }
    /* if we saved some items for later (most likely just a worn ring
       of levitation) and they're still in inventory, handle them now */
    for (i = 0; i < deferral_indx; ++i) 
    {
        /* note: obj->nobj is only referenced when obj is skipped;
           having obj be dropped or destroyed won't affect traversal */
        for (obj = invent; obj; obj = obj->nobj)
            if (obj->o_id == deferrals[i]) 
            {
                destroy_one_item(obj, osym, dmgtyp, FALSE);
                break;
            }
    }
    return;
}

int
destroy_mitem(mtmp, osym, dmgtyp)
struct monst *mtmp;
int osym, dmgtyp;
{
    struct obj *obj;
    int skip, tmp = 0;
    long i, cnt, quan;
    int dindx;
    boolean vis;
    int obj_sound_type = OBJECT_SOUND_TYPE_BREAK;

    if (mtmp == &youmonst) 
    { /* this simplifies artifact_hit() */
        destroy_item(osym, dmgtyp);
        return 0; /* arbitrary; value doesn't matter to artifact_hit() */
    }

    vis = canseemon(mtmp);

    /* see destroy_item(); object destruction could disrupt inventory list */
    bypass_objlist(mtmp->minvent, FALSE); /* clear bypass bit for minvent */

    while ((obj = nxt_unbypassed_obj(mtmp->minvent)) != 0)
    {
        if (obj->oclass != osym)
            continue; /* test only objs of type osym */
        skip = 0;
        quan = 0L;
        dindx = 0;
        obj_sound_type = OBJECT_SOUND_TYPE_BREAK;

        switch (dmgtyp) 
        {
        case AD_COLD:
            obj_sound_type = OBJECT_SOUND_TYPE_FROZEN;
            if (oresist_cold(obj) || is_obj_protected_by_property(obj, mtmp, dmgtyp))
            {
                skip++;
                break;
            }
            if (osym == POTION_CLASS && obj->otyp != POT_OIL)
            {
                quan = obj->quan;
                dindx = 0;
                tmp++;
            } else
                skip++;
            break;
        case AD_FIRE:
            obj_sound_type = OBJECT_SOUND_TYPE_BURNT;
            if (obj->otyp == SPE_BOOK_OF_THE_DEAD)
            {
                skip++;
                if (vis)
                {
                    const char* hclr = hcolor_multi_buf2(NH_DARK_RED);
                    multicolor_buffer[1] = multicolor_buffer[3] = multicolor_buffer[2];
                    pline_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s glows a %s%s%s, but remains intact.",
                        The(distant_name(obj, xname)), "strange ", hclr, " light");
                }
            }
            if (oresist_fire(obj) || is_obj_protected_by_property(obj, mtmp, dmgtyp))
            {
                skip++;
                break;
            }
            quan = obj->quan;
            switch (osym) 
            {
            case POTION_CLASS:
                dindx = (obj->otyp != POT_OIL) ? 1 : 2;
                tmp++;
                break;
            case SCROLL_CLASS:
                dindx = 3;
                tmp++;
                break;
            case SPBOOK_CLASS:
                dindx = 4;
                tmp++;
                break;
            case FOOD_CLASS:
                if (obj->otyp == GLOB_OF_GREEN_SLIME) 
                {
                    dindx = 1; /* boil and explode */
                    tmp += (obj->owt + 19) / 20;
                } else 
                {
                    skip++;
                }
                break;
            default:
                skip++;
                break;
            }
            break;
        case AD_ELEC:
            obj_sound_type = OBJECT_SOUND_TYPE_ELECTROCUTED;
            quan = obj->quan;
            if(oresist_elec(obj) || is_obj_protected_by_property(obj, mtmp, dmgtyp))
            {
                skip++;
                break;
            }

            switch (osym)
            {
            case RING_CLASS:
                dindx = 5;
                break;
            case WAND_CLASS:
                dindx = 6;
                tmp++;
                break;
            default:
                skip++;
                break;
            }
            break;
        default:
            skip++;
            break;
        }
        if (!skip) 
        {
            for (i = cnt = 0L; i < quan; i++)
                if (!rn2(3))
                    cnt++;

            if (!cnt)
                continue;
            if (vis)
            {
                play_simple_object_sound(obj, obj_sound_type);
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s%s %s!",
                    (cnt == obj->quan) ? "" : (cnt > 1L) ? "Some of "
                    : "One of ",
                    (cnt == obj->quan) ? Yname2(obj) : yname(obj),
                    destroy_strings[dindx][(cnt > 1L)]);
            }

            for (i = 0; i < cnt; i++)
                m_useup(mtmp, obj);
        }
    }
    return tmp;
}

boolean
is_obj_protected_by_property(otmp, mtmp, adtyp)
struct obj* otmp; // Can be null
struct monst* mtmp;
int adtyp;
{
    if (!mtmp)
        return FALSE;

    ///* Worn items only, but including any wielded items (even if improperly like wielded rings and potions) */
    //if (otmp && (otmp->owornmask & ~(W_SWAP_WEAPON | W_QUIVER)) == 0L)
    //    return FALSE;
    
    boolean isyou = (mtmp == &youmonst);

    //Check that mtmp is carrying the item
    if (otmp)
    {
        if (isyou && !carried(otmp))
            return FALSE;
        else if (mcarried(otmp) && mtmp != otmp->ocarry)
            return FALSE;
    }

    switch (adtyp)
    {
    case AD_FIRE:
        if (isyou ? Fire_immunity : is_mon_immune_to_fire(mtmp))
            return TRUE;
        else if (isyou ? Improved_fire_resistance : mon_resists_fire_strongly(mtmp))
        {
            if (rn2(4))
                return TRUE;
        }
        else if (isyou ? Fire_resistance : mon_resists_fire_weakly(mtmp))
        {
            if (rn2(2))
                return TRUE;
        }
        break;
    case AD_COLD:
        if (isyou ? Cold_immunity : is_mon_immune_to_cold(mtmp))
            return TRUE;
        else if (isyou ? Improved_cold_resistance : mon_resists_cold_strongly(mtmp))
        {
            if (rn2(4))
                return TRUE;
        }
        else if (isyou ? Cold_resistance : mon_resists_cold_weakly(mtmp))
        {
            if (rn2(2))
                return TRUE;
        }
        break;
    case AD_ELEC:
        if (isyou ? Shock_immunity : is_mon_immune_to_elec(mtmp))
            return TRUE;
        else if (isyou ? Improved_shock_resistance : mon_resists_elec_strongly(mtmp))
        {
            if (rn2(4))
                return TRUE;
        }
        else if (isyou ? Shock_resistance : mon_resists_elec_weakly(mtmp))
        {
            if (rn2(2))
                return TRUE;
        }
        break;
    case AD_ACID:
        if (isyou ? Acid_immunity : is_mon_immune_to_acid(mtmp))
            return TRUE;
        else if (isyou ? Improved_acid_resistance : mon_resists_acid_strongly(mtmp))
        {
            if (rn2(4))
                return TRUE;
        }
        else if (isyou ? Acid_resistance : mon_resists_acid_weakly(mtmp))
        {
            if (rn2(2))
                return TRUE;
        }
        break;
    case AD_MAGM:
        if (isyou ? Magic_missile_immunity : is_mon_immune_to_magic_missile(mtmp))
            return TRUE;
        else if (isyou ? Improved_magic_missile_resistance : mon_resists_magic_missile_strongly(mtmp))
        {
            if (rn2(4))
                return TRUE;
        }
        else if (isyou ? Magic_missile_resistance : mon_resists_magic_missile_weakly(mtmp))
        {
            if (rn2(2))
                return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}


boolean
check_magic_resistance_and_inflict_damage(mtmp, otmp, origmonst, resisting_halves_damage, dmg, adtyp, tell)
struct monst *mtmp;
struct obj* otmp;
struct monst* origmonst;
boolean resisting_halves_damage;
int dmg, adtyp, tell;
{
    if (!mtmp)
        return FALSE;

    boolean resisted;
    //int alev, dlev;
    char oclass = ILLOBJ_CLASS;
    boolean is_you = (mtmp == &youmonst);

    if (otmp)
        oclass = otmp->oclass;

    /* fake players always pass resistance test against Conflict
       (this doesn't guarantee that they're never affected by it) */
    
    if (oclass == RING_CLASS && !dmg && !tell && is_mplayer(mtmp->data))
        return 1;

    if (otmp && (oclass == SPBOOK_CLASS || oclass == WAND_CLASS) 
        && (objects[otmp->otyp].oc_aflags & S1_SPELL_BYPASSES_MAGIC_RESISTANCE) 
        && (!(mtmp->data->geno & G_UNIQ) || (objects[otmp->otyp].oc_aflags & S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE)))
        resisted = FALSE;
    else
    {
        boolean nomr = is_you ? No_magic_resistance : has_no_magic_resistance(mtmp);
        boolean quartermr = is_you ? One_fourth_magic_resistance : has_one_fourth_magic_resistance(mtmp);
        boolean halfmr = is_you ? Half_magic_resistance : has_half_magic_resistance(mtmp);
        boolean threequartersmr = is_you ? Three_fourths_magic_resistance : has_three_fourths_magic_resistance(mtmp);

        int applicable_mr = mtmp->data->mr;

        if (resists_magic(mtmp) && applicable_mr < 100)
            applicable_mr = 100;

        if (is_you && Antimagic && applicable_mr < 100)
            applicable_mr = 100;

        if (nomr)
            applicable_mr = 0;
        else if(quartermr)
            applicable_mr = max(0, mtmp->data->mr - 75);
        else if (halfmr)
            applicable_mr = max(0, mtmp->data->mr - 50);
        else if (threequartersmr)
            applicable_mr = max(0, mtmp->data->mr - 25);

        if (applicable_mr <= 0)
            resisted = FALSE;
        else
            resisted = (rn2(100) < applicable_mr);  //(rn2(100 + alev - dlev) < applicable_mr);
    }

    /* No need to go any further */
    if (dmg == 0 && tell == NOTELL)
        return resisted;

    double damage = dmg == 0 ? 0 : adjust_damage(dmg, origmonst, mtmp, adtyp, ADFLAGS_SPELL_DAMAGE);

    if (resisted)
    {
        if(damage > 0)
            damage = resisting_halves_damage ? damage / 2 : 0;

        if (tell) 
        {
            if (is_you)
            {
                play_sfx_sound(SFX_GENERAL_RESISTS);
                u_shieldeff();
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "resist!");
            }
            else
            {
                play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mtmp->mx, mtmp->my);
                m_shieldeff(mtmp);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s resists!", Monnam(mtmp));
            }
        }
    }

    if (damage > 0) 
    {
        enum hit_tile_types hit_tile = get_hit_tile_by_adtyp(adtyp);
        int hp_before = 0, hp_after = 0;
        if (is_you)
        {
            boolean was_polyd = Upolyd;
            hp_before = Upolyd ? u.mh : u.uhp;
            losehp(damage, otmp ? cxname(otmp) : "damage source", KILLED_BY);
            hp_after = Upolyd ? u.mh : u.uhp;
            boolean polyd_same = (Upolyd && was_polyd) || (!Upolyd && !was_polyd);

            int damagedealt = hp_before - hp_after;
            if (tell && damagedealt > 0 && polyd_same && !(tell == TELL_LETHAL_STYLE && !resisted))
            {//Lethal damage not shown, resisted though yes
                You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_red1, "sustain %d damage!", damagedealt);
                display_u_being_hit(hit_tile, damagedealt, 0UL);
            }
        }
        else
        {
            hp_before = mtmp->mhp;
            deduct_monster_hp(mtmp, damage);
            hp_after = mtmp->mhp;

            int damagedealt = hp_before - hp_after;
            if (tell && damagedealt > 0 && !(tell == TELL_LETHAL_STYLE && !resisted))
            {//Lethal damage not shown, resisted though yes
                pline_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_orange2, "%s sustains %d damage!", Monnam(mtmp), damagedealt);
                display_m_being_hit(mtmp, hit_tile, damagedealt, 0UL, FALSE);
            }

            if (DEADMONSTER(mtmp))
            {
                if (m_using)
                    monkilled(mtmp, "", adtyp, 0);
                else
                    killed(mtmp);
            }
        }

    }
    return resisted;
}

boolean
inflict_spell_damage(mtmp, otmp, origmonst, dmg, adtyp, tell)
struct monst* mtmp;
struct obj* otmp;
struct monst* origmonst;
int dmg, adtyp, tell;
{
    boolean is_you = (mtmp == &youmonst);

    double damage = dmg == 0 ? 0 : adjust_damage(dmg, origmonst, mtmp, adtyp, ADFLAGS_SPELL_DAMAGE);

    if (damage > 0)
    {
        enum hit_tile_types hit_tile = get_hit_tile_by_adtyp(adtyp);
        int hp_before = 0, hp_after = 0;
        if (is_you)
        {
            boolean was_polyd = Upolyd;
            hp_before = Upolyd ? u.mh : u.uhp;
            losehp(damage, otmp ? cxname(otmp) : "damage source", KILLED_BY);
            hp_after = Upolyd ? u.mh : u.uhp;
            boolean polyd_same = (Upolyd && was_polyd) || (!Upolyd && !was_polyd);

            int damagedealt = hp_before - hp_after;
            if (tell == TELL && damagedealt > 0 && polyd_same)
            {//Lethal damage not shown
                You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_red1, "sustain %d damage!", damagedealt);
                display_u_being_hit(hit_tile, damagedealt, 0UL);
            }
        }
        else
        {
            hp_before = mtmp->mhp;
            deduct_monster_hp(mtmp, damage);
            hp_after = mtmp->mhp;

            int damagedealt = hp_before - hp_after;
            if (tell == TELL && damagedealt > 0)
            {//Lethal damage not shown
                pline_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_orange2, "%s sustains %d damage!", Monnam(mtmp), damagedealt);
                display_m_being_hit(mtmp, hit_tile, damagedealt, 0UL, FALSE);
            }

            if (DEADMONSTER(mtmp))
            {
                if (m_using)
                    monkilled(mtmp, "", adtyp, 0);
                else
                    killed(mtmp);
            }
        }
    }

    return 0;
}


#define MAXWISHTRY 5

STATIC_OVL void
wishcmdassist(triesleft)
int triesleft;
{
    static NEARDATA const char *
        wishinfo[] = {
  "Wish details:",
  "",
  "Enter the name of an object, such as \"potion of monster detection\",",
  "\"scroll labeled README\", \"elven mithril-coat\", or \"Grimtooth\"",
  "(without the quotes).",
  "",
  "For object types which come in stacks, you may specify a plural name",
  "such as \"potions of healing\", or specify a count, such as \"1000 gold",
  "pieces\", although that aspect of your wish might not be granted.",
  "",
  "You may also specify various prefix values which might be used to",
  "modify the item, such as \"uncursed\" or \"rustproof\" or \"+1\".",
  "Most modifiers shown when viewing your inventory can be specified.",
  "",
  "You may specify 'nothing' to explicitly decline this wish.",
  0,
    },
        preserve_wishless[] = "Doing so will preserve 'wishless' conduct.",
        retry_info[] =
                    "If you specify an unrecognized object name %s%s time%s,",
        retry_too[] = "a randomly chosen item will be granted.",
        suppress_cmdassist[] =
            "(Suppress this assistance with !cmdassist in your config file.)",
        *cardinals[] = { "zero",  "one",  "two", "three", "four", "five" },
        too_many[] = "too many";
    int i;
    winid win;
    char buf[BUFSZ];

    win = create_nhwindow(NHW_TEXT);
    if (!win)
        return;
    for (i = 0; i < SIZE(wishinfo) - 1; ++i)
        putstr(win, 0, wishinfo[i]);
    if (!u.uconduct.wishes)
        putstr(win, 0, preserve_wishless);
    putstr(win, 0, "");
    Sprintf(buf, retry_info,
            (triesleft >= 0 && triesleft < SIZE(cardinals))
               ? cardinals[triesleft]
               : too_many,
            (triesleft < MAXWISHTRY) ? " more" : "",
            plur(triesleft));
    putstr(win, 0, buf);
    putstr(win, 0, retry_too);
    putstr(win, 0, "");
    if (iflags.cmdassist)
        putstr(win, 0, suppress_cmdassist);
    display_nhwindow(win, FALSE);
    destroy_nhwindow(win);
}

void
makewish(is_wiz_wish, play_sound)
boolean is_wiz_wish, play_sound;
{
    char buf[BUFSZ] = DUMMY;
    char bufcpy[BUFSZ];
    char promptbuf[BUFSZ];
    struct obj *otmp, nothing;
    int tries = 0;
    long prev_artwish = u.uconduct.wisharti;

    promptbuf[0] = '\0';
    nothing = zeroobj; /* lint suppression; only its address matters */
    if (flags.verbose)
    {
        if (play_sound)
            play_sfx_sound(SFX_WISH_FOR_OBJECT);
        You("may wish for an object.");
    }
retry:
    Strcpy(promptbuf, "For what do you wish");
    if (iflags.cmdassist && tries > 0)
        Strcat(promptbuf, " (enter 'help' for assistance)");
    Strcat(promptbuf, "?");
    getlin_ex(GETLINE_WISHING, ATR_NONE, NO_COLOR, promptbuf, buf, (char*)0, (char*)0, (char*)0);
    (void) mungspaces(buf);
    if (buf[0] == '\033') {
        buf[0] = '\0';
    } else if (!strcmpi(buf, "help")) {
        wishcmdassist(MAXWISHTRY - tries);
        goto retry;
    }
    /*
     *  Note: if they wished for and got a non-object successfully,
     *  otmp == &zeroobj.  That includes gold, or an artifact that
     *  has been denied.  Wishing for "nothing" requires a separate
     *  value to remain distinct.
     */
    Strcpy(bufcpy, buf);
    boolean isremovedfromgame = FALSE;;
    otmp = readobjnam(buf, &nothing, is_wiz_wish, &isremovedfromgame);
    if (!otmp) {
        pline(isremovedfromgame ? "The requested object has been removed from the game." : "Nothing fitting that description exists in the game.");
        if (++tries < MAXWISHTRY)
            goto retry;
        pline1(thats_enough_tries);
        otmp = readobjnam((char *) 0, (struct obj *) 0, is_wiz_wish, (boolean*)0);
        if (!otmp)
            return; /* for safety; should never happen */
    } else if (otmp == &nothing) {
        /* explicitly wished for "nothing", presumably attempting
           to retain wishless conduct */
        return;
    }

    play_sfx_sound(SFX_WISH_FULFILLED);

    /* KMH, conduct */
    if (!u.uconduct.wishes++) 
    {
        livelog_printf(LL_CONDUCT | LL_WISH | (prev_artwish < u.uconduct.wisharti ? LL_ARTIFACT : 0),
            "made %s first wish - \"%s\"", uhis(), bufcpy);
    }
    else if (!prev_artwish && u.uconduct.wisharti) 
    {
        /* arti conduct handled in readobjnam() above */
        livelog_printf(LL_CONDUCT | LL_WISH | LL_ARTIFACT,
            "made %s first artifact wish - \"%s\"", uhis(), bufcpy);
    }
    else 
    {
        livelog_printf(LL_WISH | (prev_artwish < u.uconduct.wisharti ? LL_ARTIFACT : 0),
            "wished for \"%s\"", bufcpy);
    }

    if (otmp != &zeroobj)
    {
        const char
            *verb = ((Is_airlevel(&u.uz) || u.uinwater) ? "slip" : "drop"),
            *oops_msg = (u.uswallow
                         ? "Oops!  %s out of your reach!"
                         : (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)
                            || levl[u.ux][u.uy].typ < IRONBARS
                            || levl[u.ux][u.uy].typ >= ICE)
                            ? "Oops!  %s away from you!"
                            : "Oops!  %s to the floor!");

#ifdef WISH_TRACKER
        /* write it out to our universal wishtracker file */
        trackwish(bufcpy);
#endif
        /* The(aobjnam()) is safe since otmp is unidentified -dlc */
        (void) hold_another_object(otmp, oops_msg,
                                   The(aobjnam(otmp, verb)),
                                   (const char *) 0);
        u.uprayer_timeout += rn1(100, 50) / (Role_if(PM_PRIEST) ? 2 : 1); /* the gods take notice */
    }
}

void
summonblackblade(spell_otmp)
struct obj* spell_otmp;
{
    summonitem(spell_otmp, BLACK_BLADE_OF_DISINTEGRATION);
}

void
summonitem(spell_otmp, otyp)
struct obj* spell_otmp;
int otyp;
{
    if (!spell_otmp)
        return;

    struct obj* otmp;

    otmp = mksobj(otyp, FALSE, FALSE, FALSE);
    if (otmp && otmp != &zeroobj) 
    {
        otmp->age = d(objects[spell_otmp->otyp].oc_spell_dur_dice, objects[spell_otmp->otyp].oc_spell_dur_diesize) + objects[spell_otmp->otyp].oc_spell_dur_plus;
        otmp->nomerge = 1;
        begin_existence(otmp);
        
        const char
            * verb = ((Is_airlevel(&u.uz) || u.uinwater) ? "slip" : "drop"),
            * oops_msg = (u.uswallow
                ? "Oops!  %s out of your reach!"
                : (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)
                    || levl[u.ux][u.uy].typ < IRONBARS
                    || levl[u.ux][u.uy].typ >= ICE)
                ? "Oops!  %s away from you!"
                : "Oops!  %s to the floor!");

        /* The(aobjnam()) is safe since otmp is unidentified -dlc */
        (void)hold_another_object(otmp, oops_msg,
            The(aobjnam(otmp, verb)),
            (const char*)0);
    }
}

void
summonmagearmor(spell_otmp)
struct obj* spell_otmp;
{
    summonitem(spell_otmp, FORCE_FIELD_ARMOR);
}

struct monst*
summoncreature(spl_otyp, monst_id, message_fmt, mmflags, scflags) // capitalize, markassummoned, disregardstrength, disregardhealth, bloodlust, pacifist, faithful
int spl_otyp, monst_id;
const char* message_fmt; //input the summoning message with one %s, which is for the monster name
unsigned long mmflags;
unsigned long scflags;
{
    if (monst_id < LOW_PM)
        return (struct monst*)0;

    struct monst* mon = (struct monst*)0;
    boolean capitalize = !!(scflags & SUMMONCREATURE_FLAGS_CAPITALIZE); //capitalize the monster name for %s
    boolean markassummoned = !!(scflags & SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED); //mark as summoned
    boolean disregardstrength = !!(scflags & SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH);
    boolean disregardhealth = !!(scflags & SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
    boolean bloodlust = !!(scflags & SUMMONCREATURE_FLAGS_BLOODLUST);
    boolean pacifist = !!(scflags & SUMMONCREATURE_FLAGS_PACIFIST);
    boolean faithful = !!(scflags & SUMMONCREATURE_FLAGS_FAITHFUL);
    boolean protector = !!(scflags & SUMMONCREATURE_FLAGS_PROTECTOR);

    mon = makemon(&mons[monst_id], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END | mmflags);
    if (mon)
    {
        mon->issummoned = markassummoned;
        mon->disregards_enemy_strength = disregardstrength;
        mon->disregards_own_health = disregardhealth;
        mon->hasbloodlust = bloodlust;
        mon->ispacifist = pacifist;
        mon->isprotector = protector;

        if(!mon->isfaithful)
            mon->isfaithful = faithful;

        (void)tamedog(mon, (struct obj*) 0, TAMEDOG_FORCE_ALL, FALSE, 0, FALSE, FALSE);

        if(spl_otyp > STRANGE_OBJECT && ((objects[spl_otyp].oc_spell_dur_dice > 1 && objects[spl_otyp].oc_spell_dur_diesize > 1) || objects[spl_otyp].oc_spell_dur_plus))
            mon->summonduration = d(objects[spl_otyp].oc_spell_dur_dice, objects[spl_otyp].oc_spell_dur_diesize) + objects[spl_otyp].oc_spell_dur_plus;
        else
            mon->summonduration = 0;

        if(mon->summonduration > 0) //Otherwise, permanent
            begin_summontimer(mon);

        if(strcmp(message_fmt, "") != 0) //Strings do not match
            pline_ex(ATR_NONE, CLR_MSG_SPELL, message_fmt, capitalize ? Amonnam(mon) : a_monnam(mon)); //"%s appears in a puff of smoke!"
    }
    return mon;
}


void
summondemon(spl_otyp)
int spl_otyp;
{
    struct monst* mon = (struct monst*) 0;
    int monindex = 0;
    
    monindex = ndemon(A_NONE, TRUE, TRUE);
    
    if(monindex >= LOW_PM)
        mon = makemon(&mons[monindex], u.ux, u.uy, MM_NO_MONSTER_INVENTORY | MM_PLAY_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);

    if (mon)
    {
        mon->issummoned = TRUE;
        mon->disregards_enemy_strength = TRUE;
        mon->disregards_own_health = FALSE;
        mon->hasbloodlust = TRUE;
        (void)tamedog(mon, (struct obj*) 0, TAMEDOG_FORCE_NON_UNIQUE, FALSE, 0, FALSE, FALSE);
        if ((objects[spl_otyp].oc_spell_dur_dice > 0 && objects[spl_otyp].oc_spell_dur_diesize > 0) || objects[spl_otyp].oc_spell_dur_plus > 0)
        {
            mon->summonduration = d(objects[spl_otyp].oc_spell_dur_dice, objects[spl_otyp].oc_spell_dur_diesize) + objects[spl_otyp].oc_spell_dur_plus;
            begin_summontimer(mon);
        }
        //play_sfx_sound_at_location(SFX_SUMMON_DEMON, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s appears before you in a puff of smoke!", Amonnam(mon));
    }
    else
        pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, "However, nothing seems to happen.");

}

void
summondemogorgon(spl_otyp)
int spl_otyp;
{
    struct monst* mon = (struct monst*) 0;
    int monindex = PM_DEMOGORGON;

    if(!Blind)
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "A pitch black gate forms in the air before you...");
    else
        You_ex(ATR_NONE, CLR_MSG_SPELL, "start to smell unnatural stench of death and decay!");

    if (mvitals[monindex].mvflags & MV_GONE || mvitals[monindex].born > 0)
    {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "However, nobody answers your call.");
        return;
    }

    mon = makemon(&mons[monindex], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);

    if (mon)
    {
        //Demogorgon gets bored and goes back to the abyss
        mon->issummoned = TRUE;
        mon->disregards_enemy_strength = TRUE;
        mon->disregards_own_health = FALSE;
        mon->hasbloodlust = TRUE;
        if (u.ualign.type == A_CHAOTIC)
        {
            (void)tamedog(mon, (struct obj*)0, TAMEDOG_FORCE_ALL, FALSE, 0, FALSE, FALSE);
            //mon->mpeaceful = TRUE;
            //mon->isprotector = TRUE;
        }
        mon->summonduration = d(objects[spl_otyp].oc_spell_dur_dice, objects[spl_otyp].oc_spell_dur_diesize) + objects[spl_otyp].oc_spell_dur_plus;
        begin_summontimer(mon);
        if (!Blind)
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s steps through the portal!", Monnam(mon));
        else
            You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "a deeply evil presence near you!");
    }
    else
    {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "However, nothing else happens");
    }
    if (!Blind)
    {
        if (!Deaf)
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s hisses maliciously!", Monnam(mon));
    }
    else if(!Deaf)
        You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "malicious hissing!");
}

void
summonbahamut(spl_otyp)
int spl_otyp UNUSED;
{
    struct monst* mon = (struct monst*) 0;
    int monindex = PM_BAHAMUT;

    if (Deaf)
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "You feel vibrations in the air...");
    else
        You_ex(ATR_NONE, CLR_MSG_SPELL, "start to hear a distinctive heavenly melody from a distance!");

    if ((mvitals[monindex].mvflags & MV_GONE) || mvitals[monindex].born > 0)
    {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "However, the music stops suddenly.");
        return;
    }

    mon = makemon(&mons[monindex], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_LAWFUL_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);

    if (mon)
    {
        (void)tamedog(mon, (struct obj*)0, TAMEDOG_FORCE_ALL, FALSE, 0, FALSE, FALSE);

        //mon->issummoned = TRUE;
        mon->disregards_enemy_strength = TRUE;
        mon->disregards_own_health = FALSE;
        mon->hasbloodlust = FALSE;
        if (!Blind)
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s descends from the heavens!", Monnam(mon));
        else
            You_feel_ex(ATR_NONE, CLR_MSG_SPELL, "a %s presence near you!", is_peaceful(mon) ? "benevolent" : "threatening");

        if (!Blind)
        {
            if (is_peaceful(mon))
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s gazes at you and smiles.", Monnam(mon));
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s looks worringly angry.", Monnam(mon));
        }
        else if (!Deaf && is_peaceful(mon))
            You_hear_ex(ATR_NONE, CLR_MSG_POSITIVE, "deep voice greeting you.");
    }
    else
    {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "However, nothing else happens");
    }

}




void
armageddon()
{
    struct monst* mon;
    int killstyle = rn2(3); //0 = all monsters, but not pets or you, 1 = all monsters and pets, but not you, 2 = also you

    for (mon = fmon; mon; mon = mon->nmon)
    {
        if (is_tame(mon) && killstyle == 0)
            continue;

        if(!DEADMONSTER(mon))
        {
            mon->mhp = 0;
            xkilled(mon, canseemon(mon) ? XKILL_DROPDEAD : XKILL_NOMSG);
        }
    }

    if (killstyle == 2)
    {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Finally, the spell catches up on you... You die.");
        Strcpy(killer.name, "armageddon");
        killer.format = KILLED_BY_AN;
        done(DIED);
    }
}


void
timestop(duration)
int duration;
{
    pline_ex(ATR_NONE, CLR_MSG_SPELL, "The flow of time seems to slow down!");
    context.time_stopped = TRUE;
    begin_timestoptimer((long)duration);

}


int
mon_to_zombie(montype)
int montype;
{
    int zombietype = NON_PM;
    if (montype == PM_HUMAN || mons[montype].mflags2 & M2_HUMAN)
        zombietype = PM_HUMAN_ZOMBIE;
    else if (montype == PM_DWARF || mons[montype].mflags2 & M2_DWARF)
        zombietype = PM_DWARF_ZOMBIE;
    else if (montype == PM_ELF || mons[montype].mflags2 & M2_ELF)
        zombietype = PM_ELF_ZOMBIE;
    else if (montype == PM_GNOLL || mons[montype].mflags2 & M2_GNOLL)
        zombietype = PM_GNOLL_ZOMBIE;
    else if (montype == PM_GNOME || mons[montype].mflags2 & M2_GNOME)
        zombietype = PM_GNOME_ZOMBIE;
    else if (montype == PM_ORC || mons[montype].mflags2 & M2_ORC)
        zombietype = PM_ORC_ZOMBIE;
    else if (montype == PM_KOBOLD || mons[montype].mlet == S_KOBOLD)
        zombietype = PM_KOBOLD_ZOMBIE;
    else if (montype == PM_ETTIN)
        zombietype = PM_ETTIN_ZOMBIE;
    else if (montype == PM_GIANT || mons[montype].mflags2 & M2_GIANT)
        zombietype = PM_GIANT_ZOMBIE;

    return zombietype;
}

int
mon_to_mummy(montype)
int montype;
{
    int zombietype = NON_PM;
    if (montype == PM_HUMAN || mons[montype].mflags2 & M2_HUMAN)
        zombietype = PM_HUMAN_MUMMY;
    else if (montype == PM_DWARF || mons[montype].mflags2 & M2_DWARF)
        zombietype = PM_DWARF_MUMMY;
    else if (montype == PM_ELF || mons[montype].mflags2 & M2_ELF)
        zombietype = PM_ELF_MUMMY;
    else if (montype == PM_GNOLL || mons[montype].mflags2 & M2_GNOLL)
        zombietype = PM_GNOLL_MUMMY;
    else if (montype == PM_GNOME || mons[montype].mflags2 & M2_GNOME)
        zombietype = PM_GNOME_MUMMY;
    else if (montype == PM_ORC || mons[montype].mflags2 & M2_ORC)
        zombietype = PM_ORC_MUMMY;
    else if (montype == PM_KOBOLD || mons[montype].mlet == S_KOBOLD)
        zombietype = PM_KOBOLD_MUMMY;
    else if (montype == PM_ETTIN)
        zombietype = PM_ETTIN_MUMMY;
    else if (montype == PM_GIANT || mons[montype].mflags2 & M2_GIANT)
        zombietype = PM_GIANT_MUMMY;

    return zombietype;
}


STATIC_OVL int
get_summon_monster_type(otyp)
int otyp;
{
    switch(otyp)
    {
    case SPE_SUMMON_GRIZZLY_BEAR:
        return PM_GRIZZLY_BEAR;
    case SPE_SUMMON_OWLBEAR:
        return PM_OWLBEAR;
    case SPE_SUMMON_OWLBEAR_PATRIARCH:
        return PM_OWLBEAR_PATRIARCH;
    case SPE_SUMMON_COCKATRICE:
        return PM_COCKATRICE;
    case SPE_SUMMON_GIANT_COCKATRICE:
        return PM_GIANT_COCKATRICE;
    case SPE_SUMMON_GARGANTUAN_COCKATRICE:
        return PM_GARGANTUAN_COCKATRICE;
    case SPE_SUMMON_GIANT_SPIDER:
        return PM_GIANT_SPIDER;
    case SPE_SUMMON_PHASE_SPIDER:
        return PM_PHASE_SPIDER;
    case SPE_SUMMON_PURPLE_WORM:
        return PM_PURPLE_WORM;
    case SPE_SUMMON_GARGANTUAN_BEETLE:
        return PM_GARGANTUAN_BEETLE;
    case SPE_SUMMON_RAVEN:
        return PM_RAVEN;
    case SPE_SUMMON_WINTER_WOLF:
        return PM_WINTER_WOLF;
    case SPE_SUMMON_DIREWOLF:
        return PM_DIREWOLF;
    case SPE_SUMMON_GIANT_ANT:
        return PM_GIANT_ANT;
    case SPE_SUMMON_SOLDIER_ANT:
        return PM_SOLDIER_ANT;
    case SPE_SUMMON_BISON:
        return PM_BISON;
    case SPE_SUMMON_PEGASUS:
        return PM_PEGASUS;
    case SPE_SUMMON_ROC:
        return PM_ROC;
    default:
        break;
    }
    return NON_PM;

}

void
reset_zap(VOID_ARGS)
{
    obj_zapped = FALSE;
    poly_zapped = 0;
}

/*zap.c*/
