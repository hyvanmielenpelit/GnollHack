/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    mcastu.c    $NHDT-Date: 1436753517 2015/07/13 02:11:57 $  $NHDT-Branch: master $:$NHDT-Revision: 1.44 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

/* monster mage spells */
enum mcast_mage_spells {
    MGC_PSI_BOLT = 0,
    MGC_CURE_SELF,
    MGC_HASTE_SELF,
    MGC_STUN_YOU,
    MGC_DISAPPEAR,
    MGC_WEAKEN_YOU,
    MGC_DESTRY_ARMR,
    MGC_CURSE_ITEMS,
    MGC_AGGRAVATION,
    MGC_SUMMON_MONS,
    MGC_SUMMON_NASTY,
    MGC_CLONE_WIZ,
    MGC_DEATH_TOUCH,
    MAX_MAGE_SPELLS
};

/* monster cleric spells */
enum mcast_cleric_spells {
    CLC_OPEN_WOUNDS = 0,
    CLC_CURE_SELF,
    CLC_CONFUSE_YOU,
    CLC_PARALYZE,
    CLC_BLIND_YOU,
    CLC_INSECTS,
    CLC_SNAKES,
    CLC_CURSE_ITEMS,
    CLC_LIGHTNING,
    CLC_FIRE_PILLAR,
    CLC_GEYSER,
    CLC_DEATH_TOUCH,
    MAX_CLERIC_SPELLS
};

STATIC_DCL void FDECL(cursetxt, (struct monst *, BOOLEAN_P));
STATIC_DCL int FDECL(choose_magic_spell, (int));
STATIC_DCL int FDECL(choose_clerical_spell, (int));
STATIC_DCL double FDECL(m_cure_self, (struct monst *, double));
STATIC_DCL void FDECL(cast_wizard_spell, (struct monst *, double, int));
STATIC_DCL void FDECL(cast_cleric_spell, (struct monst *, double, int));
STATIC_DCL boolean FDECL(is_undirected_spell, (unsigned int, int));
STATIC_DCL boolean FDECL(is_ultimate_spell, (unsigned int, int));
STATIC_DCL boolean FDECL(is_intermediate_spell, (unsigned int, int));
STATIC_DCL boolean
FDECL(spell_would_be_useless, (struct monst *, unsigned int, int));

extern const char *const flash_types[]; /* from zap.c */

/* feedback when frustrated monster couldn't cast a spell */
STATIC_OVL
void
cursetxt(mtmp, undirected)
struct monst *mtmp;
boolean undirected;
{
    if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my)) {
        const char *point_msg; /* spellcasting monsters are impolite */

        if (undirected)
            point_msg = "all around, then curses";
        else if ((Invis && !has_see_invisible(mtmp)
                  && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                 || is_obj_mappear(&youmonst, STRANGE_OBJECT)
                 || u.uundetected)
            point_msg = "and curses in your general direction";
        else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            point_msg = "and curses at your displaced image";
        else
            point_msg = "at you, then curses";

        play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_CURSE);
        pline("%s points %s.", Monnam(mtmp), point_msg);
    } 
    else if ((!(moves % 4) || !rn2(4)))
    {
        if (!Deaf)
        {
            play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_MUMBLED_CURSE);
            Norep("You hear a mumbled curse.");
        }
    }
}

/* convert a level based random selection into a specific mage spell;
   inappropriate choices will be screened out by spell_would_be_useless() */
STATIC_OVL int
choose_magic_spell(spellval)
int spellval;
{
    /* for 3.4.3 and earlier, val greater than 22 selected the default spell
     */
    while (spellval > 14 && rn2(20))
        spellval = rn2(spellval);

    switch (spellval) {
#if 0
    case 24:
    case 23:
        if (Death_resistance || Hallucination) //Antimagic || 
            return MGC_PSI_BOLT;
        /*FALLTHRU*/
    case 22:
    case 21:
    case 20:
        return MGC_DEATH_TOUCH;
#endif
    case 18:
    case 17:
        return MGC_CLONE_WIZ;
    case 16:
    case 15:
        return MGC_SUMMON_NASTY;
    case 14:
        return MGC_AGGRAVATION;
    case 13:
        return MGC_DISAPPEAR;
    case 12:
    case 11:
        return MGC_CURSE_ITEMS;
    case 10:
    case 9:
        return MGC_SUMMON_MONS;
    case 8:
    case 7:
        return MGC_DESTRY_ARMR;
    case 6:
    case 5:
        return MGC_WEAKEN_YOU;
    case 4:
    case 3:
        return MGC_STUN_YOU;
    case 2:
        return MGC_HASTE_SELF;
    case 1:
        return MGC_CURE_SELF;
    case 0:
    default:
        return MGC_PSI_BOLT;
    }
}

/* convert a level based random selection into a specific cleric spell */
STATIC_OVL int
choose_clerical_spell(spellnum)
int spellnum;
{
    /* for 3.4.3 and earlier, num greater than 13 selected the default spell
     */
    while (spellnum > 18 && rn2(19))
        spellnum = rn2(spellnum);

    switch (spellnum) {
    case 18:
    case 17:
        if (Death_resistance || Hallucination)
            return CLC_OPEN_WOUNDS;
        /*FALLTHRU*/
    case 16:
        return CLC_DEATH_TOUCH;
    case 15:
    case 14:
        if (rn2(3))
            return CLC_OPEN_WOUNDS;
        /*FALLTHRU*/
    case 13:
        return CLC_GEYSER;
    case 12:
        return CLC_FIRE_PILLAR;
    case 11:
        return CLC_LIGHTNING;
    case 10:
        return CLC_SNAKES;
    case 9:
        return CLC_CURSE_ITEMS;
    case 8:
        return CLC_INSECTS;
    case 7:
    case 6:
        return CLC_BLIND_YOU;
    case 5:
    case 4:
        return CLC_PARALYZE;
    case 3:
    case 2:
        return CLC_CONFUSE_YOU;
    case 1:
        return CLC_CURE_SELF;
    case 0:
    default:
        return CLC_OPEN_WOUNDS;
    }
}

/* return values:
 * 1: successful spell
 * 0: unsuccessful spell
 */
int
castmu(mtmp, mattk, thinks_it_foundyou, foundyou)
register struct monst *mtmp;
register struct attack *mattk;
boolean thinks_it_foundyou;
boolean foundyou;
{
    double damage;
    int ret;
    int spellnum = 0;
    int  ml = 0;
    
    //Assumes that attack type is AT_MAGC when this function is called
    if(mattk->mlevel > 0)
        ml = (int)mattk->mlevel;
    else
        ml = (int)mtmp->m_lev;

    /* Three cases:
     * -- monster is attacking you.  Search for a useful spell.
     * -- monster thinks it's attacking you.  Search for a useful spell,
     *    without checking for undirected.  If the spell found is directed,
     *    it fails with cursetxt() and loss of mspec_used.
     * -- monster isn't trying to attack.  Select a spell once.  Don't keep
     *    searching; if that spell is not useful (or if it's directed),
     *    return and do something else.
     * Since most spells are directed, this means that a monster that isn't
     * attacking casts spells only a small portion of the time that an
     * attacking monster does.
     */
    if ((mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) && ml) 
    {
        int cnt = 40;

        do 
        {
            spellnum = rn2(ml);
            if (mattk->adtyp == AD_SPEL)
                spellnum = choose_magic_spell(spellnum);
            else
                spellnum = choose_clerical_spell(spellnum);
            /* not trying to attack?  don't allow directed spells */
            if (!thinks_it_foundyou) 
            {
                if (!is_undirected_spell(mattk->adtyp, spellnum)
                    || spell_would_be_useless(mtmp, mattk->adtyp, spellnum)) 
                {
                    if (foundyou)
                        impossible(
                       "spellcasting monster found you and doesn't know it?");
                    return 0;
                }
                break;
            }
        } 
        while (--cnt > 0
                 && spell_would_be_useless(mtmp, mattk->adtyp, spellnum));
        if (cnt == 0)
            return 0;
    }

    boolean nodirspell = is_undirected_spell(mattk->adtyp, spellnum);
    unsigned short* appr_spec_ptr = &mtmp->mspec_used;
    boolean is_ultimate = FALSE;
    boolean is_intermediate = FALSE;
    enum action_tile_types action = nodirspell ? ACTION_TILE_CAST_NODIR : ACTION_TILE_CAST_DIR;
    boolean next2u = (distmin(u.ux, u.uy, mtmp->mx, mtmp->my) <= 1);
    boolean show_action_tile = (next2u || !nodirspell);

    play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_CAST);
    if(show_action_tile)
        update_m_action(mtmp, action);
    play_sfx_sound_at_location(SFX_GENERIC_CAST_EFFECT, mtmp->mx, mtmp->my);
    if (show_action_tile)
        m_wait_until_action(mtmp, action);

    if (mattk->adtyp == AD_SPEL)
    {
        if ((is_ultimate = is_ultimate_spell(mattk->adtyp, spellnum)) != 0)
            appr_spec_ptr = &mtmp->mmageultimate_used;
        else if ((is_intermediate = is_intermediate_spell(mattk->adtyp, spellnum)) != 0)
            appr_spec_ptr = &mtmp->mmageintermediate_used;
        else
            appr_spec_ptr = &mtmp->mmagespell_used;
    }
    else if (mattk->adtyp == AD_CLRC)
    {
        if ((is_ultimate = is_ultimate_spell(mattk->adtyp, spellnum)) != 0)
            appr_spec_ptr = &mtmp->mclericultimate_used;
        else if ((is_intermediate = is_intermediate_spell(mattk->adtyp, spellnum)) != 0)
            appr_spec_ptr = &mtmp->mclericintermediate_used;
        else
            appr_spec_ptr = &mtmp->mclericspell_used;
    }

    /* monster unable to cast spells? */
    if (is_cancelled(mtmp) || is_silenced(mtmp) || *appr_spec_ptr > 0 || !ml)
    {
        cursetxt(mtmp, nodirspell);
        if (show_action_tile)
        {
            m_wait_until_end(mtmp, action);
            update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            m_wait_until_end(mtmp, action);
        }
        return (0);
    }

    *appr_spec_ptr = (is_ultimate ? d(3, 20) + 60 : is_intermediate ? d(1, 6) + 14 : rnd(2) + 1);

    /* monster can cast spells, but is casting a directed spell at the
       wrong place?  If so, give a message, and return.  Do this *after*
       penalizing mspec_used. */
    if (!foundyou && thinks_it_foundyou
        && !is_undirected_spell(mattk->adtyp, spellnum)) 
    {
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s casts a spell at %s!",
              canseemon(mtmp) ? Monnam(mtmp) : "Something",
              levl[mtmp->mux][mtmp->muy].typ == WATER ? "empty water"
                                                      : "thin air");

        if (show_action_tile)
        {
            m_wait_until_end(mtmp, action);
            update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            m_wait_until_end(mtmp, action);
        }
        return (0);
    }

    if (!is_peaceful(mtmp) && (!nodirspell || canspotmon(mtmp)))
        nomul(0);

    /*Spellnum + 1 is used as spell level 1...ml; chance of fail is 50% for ml and 0% for ml /2,
      interpolated linearly*/
    int failchance = 0;
    int sl = spellnum + 1;
    if (sl > ml / 2 && ml > 0) { // fail only if spell level is high enough
        failchance = (50 * (sl - ml / 2)) / (ml - ml / 2);
    }
    if (ml == 0)
        failchance = 100;

    if (rn2(100) < failchance) {//(rn2(ml * 10) < (is_confused(mtmp) ? 100 : 20)) { /* fumbled attack */
        if (canseemon(mtmp) && !Deaf)
        {
            pline_The("air crackles around %s.", mon_nam(mtmp));
            if (iflags.using_gui_sounds)
            {
                delay_output_milliseconds(200);
                play_sfx_sound_at_location(SFX_AIR_CRACKLES, mtmp->mx, mtmp->my);
            }
        }

        if (show_action_tile)
        {
            m_wait_until_end(mtmp, action);
            update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
            m_wait_until_end(mtmp, action);
        }
        return (0);
    }
    if (canspotmon(mtmp) || !is_undirected_spell(mattk->adtyp, spellnum))
    {
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s casts a spell%s!",
              canspotmon(mtmp) ? Monnam(mtmp) : "Something",
              is_undirected_spell(mattk->adtyp, spellnum)
                  ? ""
                  : (Invisib && !has_see_invisible(mtmp)
                     && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                        ? " at a spot near you"
                        : (Displaced
                           && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                              ? " at your displaced image"
                              : " at you");
    }

    /*
     * As these are spells, the damage is related to the level
     * of the monster casting the spell.
     */
    if (!foundyou)
    {
        damage = 0;
        if (mattk->adtyp != AD_SPEL && mattk->adtyp != AD_CLRC) 
        {
            impossible(
              "%s casting non-hand-to-hand version of hand-to-hand spell %d?",
                       Monnam(mtmp), mattk->adtyp);

            if (show_action_tile)
            {
                m_wait_until_end(mtmp, action);
                update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
                m_wait_until_end(mtmp, action);
            }
            return (0);
        }
    } 
    else
        damage = adjust_damage(max(0, d(mattk->damn > 0 ? mattk->damn : (int) ((ml / 4) + 1), mattk->damn > 0 ? mattk->damn : 6) + mattk->damp), mtmp, &youmonst, mattk->adtyp, ADFLAGS_SPELL_DAMAGE);

    ret = 1;

    switch (mattk->adtyp) {
    case AD_FIRE:
        pline_ex(ATR_NONE, HI_FIRE, "You're enveloped in flames.");
        if (iflags.using_gui_sounds)
        {
            delay_output_milliseconds(200);
            play_sfx_sound(SFX_MONSTER_ON_FIRE);
        }
        if (Fire_immunity || Invulnerable) {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "But you resist the effects.");
            damage = 0;
        }
        burn_away_slime();
        break;
    case AD_COLD:
        pline_ex(ATR_NONE, HI_ICE, "You're covered in frost.");
        if (iflags.using_gui_sounds)
        {
            delay_output_milliseconds(200);
            play_sfx_sound(SFX_MONSTER_COVERED_IN_FROST);
        }
        if (Cold_immunity || Invulnerable) {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "But you resist the effects.");
            damage = 0;
        }
        break;
    case AD_MAGM:
        You_ex(ATR_NONE, HI_ZAP, "are hit by a shower of missiles!");
        if (iflags.using_gui_sounds)
        {
            delay_output_milliseconds(200);
            play_sfx_sound(SFX_HAIL_OF_MAGIC_MISSILES_HITS);
        }
        if (Magic_missile_immunity || Invulnerable) {
            play_sfx_sound(SFX_GENERAL_REFLECTS);
            u_shieldeff();
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "missiles bounce off!");
            damage = 0;
        } else
            damage = adjust_damage(d((int) mtmp->m_lev / 2 + 1, 6), mtmp, &youmonst, mattk->adtyp, ADFLAGS_SPELL_DAMAGE);
        break;
    case AD_SPEL: /* wizard spell */
        cast_wizard_spell(mtmp, damage, spellnum);
        damage = 0; /* done by the spell casting functions */
        break;
    case AD_CLRC: /* clerical spell */
        cast_cleric_spell(mtmp, damage, spellnum);
        damage = 0; /* done by the spell casting functions */
        break;
    }
    if (damage > 0)
        mdamageu(mtmp, damage, TRUE);

    if (show_action_tile)
    {
        m_wait_until_end(mtmp, action);
        update_m_action_revert(mtmp, ACTION_TILE_NO_ACTION);
        m_wait_until_end(mtmp, action);
    }
    return (ret);
}

STATIC_OVL double
m_cure_self(mtmp, damage)
struct monst *mtmp;
double damage;
{
    if (mtmp->mhp < mtmp->mhpmax)
    {
        if (canseemon(mtmp))
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : NO_COLOR, "%s looks better.", Monnam(mtmp));
        /* note: player healing does 6d4; this used to do 1d8 */
        deduct_monster_hp(mtmp, -damage);
        //if ((mtmp->mhp += d(3, 6)) > mtmp->mhpmax)
        //    mtmp->mhp = mtmp->mhpmax;
        damage = 0;
    }
    return damage;
}

/* monster wizard and cleric spellcasting functions */
/*
   If damage is zero, then the monster is not casting at you.
   If the monster is intentionally not casting at you, we have previously
   called spell_would_be_useless() and spellnum should always be a valid
   undirected spell.
   If you modify either of these, be sure to change is_undirected_spell()
   and spell_would_be_useless().
 */
STATIC_OVL
void
cast_wizard_spell(mtmp, damage, spellnum)
struct monst *mtmp;
double damage;
int spellnum;
{
    //if (damage == 0 && !is_undirected_spell(AD_SPEL, spellnum)) {
    //    impossible("cast directed wizard spell (%d) with damage=0?", spellnum);
    //    return;
    //}
    enum hit_tile_types hit_tile = HIT_GENERAL;
    switch (spellnum) 
    {
    case MGC_DEATH_TOUCH:
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Oh no, %s's using the touch of death!", mhe(mtmp));
        //boolean magic_resistance_success = check_magic_resistance_and_inflict_damage(&youmonst, (struct obj*)0, mtmp->m_lev, 0, 0, NOTELL);

        if (is_not_living(youmonst.data) || is_demon(youmonst.data) || Death_resistance)
        { //Invulnerability does not protect against death attacks
            You("seem no deader than before.");
        }
        else if (!check_magic_cancellation_success(&youmonst, -4)) //(rn2(mtmp->m_lev) > 12) //!Antimagic && !magic_resistance_success && 
        {
            if (Hallucination) 
            {
                You_ex(ATR_NONE, CLR_MSG_WARNING, "have an out of body experience.");
            }
            else
            {
                killer.format = KILLED_BY_AN;
                Strcpy(killer.name, "touch of death");
                done(DIED);
            }
        } 
        else 
        {
            play_sfx_sound(SFX_LUCKILY_ITS_TOUCH_DID_NOT_WORK);
            //if (Antimagic || magic_resistance_success)
            //    u_shieldeff();
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Lucky for you, it didn't work!");
        }
        damage = 0;
        break;
    case MGC_CLONE_WIZ:
        if (mtmp->iswiz && context.no_of_wizards == 1)
        {
            play_voice_wizard_of_yendor_simple_line(mtmp, WIZARD_OF_YENDOR_LINE_DOUBLE_TROUBLE);
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "Double Trouble...");
            clonewiz();
            damage = 0;
        } else
            impossible("bad wizard cloning?");
        break;
    case MGC_SUMMON_MONS: 
    {
        int count;

        if (mtmp->iswiz)
            count = summon_nasties(mtmp); /* summon something nasty */
        else
            count = summon_level_appropriate_monsters(mtmp); /* summon something appropriate */

        if (count == 0)
            cursetxt(mtmp, TRUE);
        else if (mtmp->iswiz)
        {
            play_voice_wizard_of_yendor_simple_line(mtmp,
                count > 1 ? WIZARD_OF_YENDOR_LINE_DESTROY_THE_THIEF_MY_PETS :
                WIZARD_OF_YENDOR_LINE_DESTROY_THE_THIEF_MY_PET);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Destroy the thief, my pet%s!", plur(count));
            if(canseemon(mtmp))
                talkeff(mtmp->mx, mtmp->my);
        }
        else
        {
            const char *mappear =
                (count == 1) ? "A monster appears" : "Monsters appear";

            /* messages not quite right if plural monsters created but
               only a single monster is seen */
            if (Invisib && !has_see_invisible(mtmp)
                && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s around a spot near you!", mappear);
            else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s around your displaced image!", mappear);
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s from nowhere!", mappear);
        }
        damage = 0;
        break;
    }
    case MGC_SUMMON_NASTY: 
    {
        int count;

        count = summon_nasties(mtmp); /* summon something nasty */

        if (count == 0)
            cursetxt(mtmp, TRUE);
        else if (mtmp->iswiz)
        {
            play_voice_wizard_of_yendor_simple_line(mtmp,
                count > 1 ? WIZARD_OF_YENDOR_LINE_DESTROY_THE_THIEF_MY_PETS :
                WIZARD_OF_YENDOR_LINE_DESTROY_THE_THIEF_MY_PET);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Destroy the thief, my pet%s!", plur(count));
            if (canseemon(mtmp))
                talkeff(mtmp->mx, mtmp->my);
        }
        else
        {
            const char* mappear =
                (count == 1) ? "A monster appears" : "Monsters appear";

            /* messages not quite right if plural monsters created but
               only a single monster is seen */
            if (Invisib && !has_see_invisible(mtmp)
                && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s around a spot near you!", mappear);
            else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s around your displaced image!", mappear);
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s from nowhere!", mappear);
        }
        damage = 0;
        break;
    }
    case MGC_AGGRAVATION:
        play_sfx_sound(SFX_AGGRAVATE_MONSTER);
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "that monsters are aware of your presence.");
        aggravate();
        damage = 0;
        break;
    case MGC_CURSE_ITEMS:
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "as if you need some help.");
        rndcurse();
        damage = 0;
        break;
    case MGC_DESTRY_ARMR:
        if (Antimagic_or_resistance) 
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "A field of force surrounds you!");
        }
        else if (Armor_destruction_resistance) 
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            struct obj* protitem = what_gives(ARMOR_DESTRUCTION_RESISTANCE, FALSE);
            if (protitem)
            {
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s the destructive energies of the spell!", Yobjnam2(protitem, "neutralize"));
                makeknown(protitem->otyp);
            }
            else
                pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, "A mysterious force neutralizes the destructive energies of the spell!");
        }
        else if (!destroy_arm(some_armor(&youmonst))) 
        {
            play_sfx_sound(SFX_HANDS_ITCH);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin itches.");
        }
        damage = 0;
        break;
    case MGC_WEAKEN_YOU: /* drain strength */
        if (Antimagic_or_resistance) {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            You_feel_ex(ATR_NONE, CLR_MSG_SPELL, "momentarily weakened.");
        }
        else 
        {
            play_sfx_sound(SFX_LOSE_ABILITY);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "suddenly feel weaker!");
            int strloss = mtmp->m_lev - 6;
            if (Half_spell_damage)
                strloss = (strloss + 1) / 2;
            if (Invulnerable)
                strloss = 0;
            losestr(rnd(strloss));
            if (u.uhp < 1)
                done_in_by(mtmp, DIED);
        }
        damage = 0;
        break;
    case MGC_DISAPPEAR: /* makes self invisible */
        if (!is_invisible(mtmp) && !has_blocks_invisibility(mtmp))
        {
            if (canseemon(mtmp))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly %s!", Monnam(mtmp),
                      !See_invisible ? "disappears" : "becomes transparent");
            increase_mon_property(mtmp, INVISIBILITY, d(2, 10) + 80);
            if (cansee(mtmp->mx, mtmp->my) && !canspotmon(mtmp))
                map_invisible(mtmp->mx, mtmp->my);
            damage = 0;
        } else
            impossible("no reason for monster to cast disappear spell?");
        break;
    case MGC_STUN_YOU:
        if (Antimagic_or_resistance || Free_action || Stun_resistance || Invulnerable) {
            u_shieldeff();
            if (!Stunned)
            {
                play_sfx_sound(SFX_DISORIENTED_FOR_MOMENT);
                You_feel_ex(ATR_NONE, CLR_MSG_SPELL, "momentarily disoriented.");
            }
            make_stunned(1L, FALSE);
        } else {
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, Stunned ? "struggle to keep your balance." : "reel...");
            int stun_duration = d(ACURR(A_DEX) < 12 ? 6 : 4, 4);
            if (Half_spell_damage)
                stun_duration = (stun_duration + 1) / 2;
            if (!Stunned)
                play_sfx_sound(SFX_ACQUIRE_STUN);
            make_stunned((HStun & TIMEOUT) + (int64_t)stun_duration, FALSE);
        }
        damage = 0;
        break;
    case MGC_HASTE_SELF:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        play_sfx_sound_at_location(SFX_ACQUIRE_HASTE, mtmp->mx, mtmp->my);
        (void)increase_mon_property_verbosely(mtmp, VERY_FAST, 150 + rnd(50));
        damage = 0;
        special_effect_wait_until_end(0);
        break;
    case MGC_CURE_SELF:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        play_sfx_sound_at_location(SFX_HEALING, mtmp->mx, mtmp->my);
        damage = m_cure_self(mtmp, is_prince(mtmp->data) ? d(12, 6) : is_lord(mtmp->data) ? d(6, 6) : d(2, 6));
        special_effect_wait_until_end(0);
        damage = 0;
        break;
    case MGC_PSI_BOLT:
        /* prior to 3.4.0 Antimagic was setting the damage to 1--this
           made the spell virtually harmless to players with magic res. */
        if (Magic_missile_immunity || Antimagic_or_resistance || Invulnerable)
        {
            play_sfx_sound(SFX_GENERAL_RESISTS);
            u_shieldeff();
            damage = damage / 2;
        }
        if (damage <= 5)
        {
            play_player_ouch_sound(MONSTER_OUCH_SOUND_SLIGHT_HEADACHE);
            You_ex(ATR_NONE, CLR_MSG_WARNING, "get a slight %sache.", body_part(HEAD));
        }
        else if (damage <= 10)
        {
            play_player_ouch_sound(MONSTER_OUCH_SOUND_MODERATE_HEADACHE);
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "brain is on fire!");
        }
        else if (damage <= 20)
        {
            play_player_ouch_sound(MONSTER_OUCH_SOUND_PAINFUL_HEADACHE);
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s suddenly aches painfully!", body_part(HEAD));
        }
        else
        {
            play_player_ouch_sound(MONSTER_OUCH_SOUND_VERY_PAINFUL_HEADACHE);
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s suddenly aches very painfully!", body_part(HEAD));
        }
        break;
    default:
        impossible("mcastu: invalid magic spell (%d)", spellnum);
        damage = 0;
        break;
    }

    if (damage > 0)
        mdamageu_with_hit_tile(mtmp, damage, TRUE, hit_tile);
}

STATIC_OVL
void
cast_cleric_spell(mtmp, damage, spellnum)
struct monst *mtmp;
double damage;
int spellnum;
{
    if (!mtmp)
        return;

    //if (damage == 0 && !is_undirected_spell(AD_CLRC, spellnum)) {
    //    impossible("cast directed cleric spell (%d) with damage=0?", spellnum);
    //    return;
    //}

    enum hit_tile_types hit_tile = HIT_GENERAL;

    switch (spellnum)
    {
    case CLC_DEATH_TOUCH:
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Oh no, %s's using the touch of death!", mhe(mtmp));

        if (is_not_living(youmonst.data) || is_demon(youmonst.data) || Death_resistance)
        {
            You("seem no deader than before.");
        }
        else if (!check_magic_cancellation_success(&youmonst, -4))
        {
            if (Hallucination)
            {
                You_ex(ATR_NONE, CLR_MSG_WARNING, "have an out of body experience.");
            }
            else
            {
                killer.format = KILLED_BY_AN;
                Strcpy(killer.name, "touch of death");
                done(DIED);
            }
        }
        else
        {
            play_sfx_sound(SFX_LUCKILY_ITS_TOUCH_DID_NOT_WORK);
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Lucky for you, it didn't work!");
        }
        damage = 0;
        break;
    case CLC_GEYSER:
        /* this is physical damage (force not heat),
         * not magical damage or fire damage
         */
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A sudden geyser slams into you from nowhere!");
        damage = adjust_damage(d(8, 6), mtmp, &youmonst, AD_PHYS, ADFLAGS_SPELL_DAMAGE);
        break;
    case CLC_FIRE_PILLAR:
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A pillar of fire strikes all around you!");
        if (Fire_immunity) 
        {
            u_shieldeff();
            damage = 0;
        } else
            damage = adjust_damage(d(8, 6), mtmp, &youmonst, AD_FIRE, ADFLAGS_SPELL_DAMAGE);
        burn_away_slime();
        (void) burnarmor(&youmonst);
        destroy_item(SCROLL_CLASS, AD_FIRE);
        destroy_item(POTION_CLASS, AD_FIRE);
        destroy_item(SPBOOK_CLASS, AD_FIRE);
        (void) burn_floor_objects(u.ux, u.uy, TRUE, FALSE);
        item_destruction_hint(AD_FIRE, FALSE);
        break;
    case CLC_LIGHTNING: {
        boolean reflects;

        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A bolt of lightning strikes down at you from above!");
        reflects = ureflects("It bounces off your %s%s.", "");
        if (reflects || Shock_immunity) {
            play_sfx_sound(SFX_GENERAL_REFLECTS);
            u_shieldeff();
            damage = 0;
            if (reflects)
                break;
        } else
            damage = adjust_damage(d(8, 6), mtmp, &youmonst, AD_ELEC, ADFLAGS_SPELL_DAMAGE);
        destroy_item(WAND_CLASS, AD_ELEC);
        destroy_item(RING_CLASS, AD_ELEC);
        (void) flashburn((int64_t) rnd(100));
        item_destruction_hint(AD_ELEC, FALSE);
        break;
    }
    case CLC_CURSE_ITEMS:
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "as if you need some help.");
        rndcurse();
        damage = 0;
        break;
    case CLC_INSECTS: 
    case CLC_SNAKES:
    {
        /* Try for insects, and if there are none
           left, go for (sticks to) snakes.  -3. */
        char let = (spellnum == CLC_INSECTS ? S_ANT : S_SNAKE);
        struct permonst *pm = mkclass(let, 0);
        struct monst *mtmp2 = (struct monst *) 0;
        if (!pm)
            break;
        boolean success = FALSE, seecaster;
        int i, quan, oldseen, newseen;
        coord bypos;
        const char *fmt;

        oldseen = monster_census(TRUE);
        quan = 2 + rnd(4);

        int difficulty = 0;
        int summon_quan = 0;
        context.makemon_spef_idx = 0;
        for (i = 0; i < quan; i++)
        {
            if (i > 0 && difficulty >= mtmp->data->difficulty)
                break;

            if (!enexto(&bypos, mtmp->mux, mtmp->muy, mtmp->data))
                break;
            
            if ((pm = mkclass(let, 0)) != 0
                && (mtmp2 = makemon(pm, bypos.x, bypos.y, MM_ANGRY | MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL))) != 0)
            {
                context.makemon_spef_idx++;
                success = TRUE;
                mtmp2->msleeping = mtmp2->mpeaceful = mtmp2->mtame = 0;
                if (!mtmp2->mtame)
                    mtmp2->ispartymember = FALSE;
                set_mhostility(mtmp2);
                difficulty += mtmp2->data->difficulty;
                summon_quan++;
            }
        }
        makemon_animation_wait_until_end();

        newseen = monster_census(TRUE);

        /* not canspotmon(), which includes unseen things sensed via warning
         */
        seecaster = canseemon(mtmp) || tp_sensemon(mtmp) || Detect_monsters;

        fmt = 0;
        if (!seecaster) 
        {
            char *arg; /* [not const: upstart(N==1 ? an() : makeplural())] */
            const char *what = (let == S_SNAKE) ? "snake" : "insect";

            if (newseen <= oldseen || Unaware)
            {
                /* unseen caster fails or summons unseen critters,
                   or unconscious hero ("You dream that you hear...") */
                You_hear_ex(ATR_NONE, CLR_MSG_SPELL, "someone summoning %s.", makeplural(what));
            } 
            else 
            {
                /* unseen caster summoned seen critter(s) */
                arg = (newseen == oldseen + 1) ? an(what) : makeplural(what);
                if (!Deaf)
                    You_hear_ex(ATR_NONE, CLR_MSG_SPELL, "someone summoning something, and %s %s.", arg,
                             vtense(arg, "appear"));
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s.", upstart(arg), vtense(arg, "appear"));
            }

            /* seen caster, possibly producing unseen--or just one--critters;
               hero is told what the caster is doing and doesn't necessarily
               observe complete accuracy of that caster's results (in other
               words, no need to fuss with visibility or singularization;
               player is told what's happening even if hero is unconscious) */
        }
        else if (!success)
            fmt = "%s casts at a clump of sticks, but nothing happens.";
        else if (let == S_SNAKE)
            fmt = (summon_quan == 1 ? "%s transforms a stick of wood into a snake!" : "%s transforms a clump of sticks into snakes!");
        else if (Invisib && !has_see_invisible(mtmp)
                 && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            fmt = (summon_quan == 1 ? "%s summons an insect around a spot near you!" : "%s summons insects around a spot near you!");
        else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            fmt = (summon_quan == 1 ? "%s summons an insect around your displaced image!" : "%s summons insects around your displaced image!");
        else
            fmt = (summon_quan == 1 ? "%s summons an insect!" : "%s summons insects!");
        if (fmt)
            pline_ex(ATR_NONE, CLR_MSG_SPELL, fmt, Monnam(mtmp));

        damage = 0;
        break;
    }
    case CLC_BLIND_YOU:
        /* note: resists_blnd() doesn't apply here */
        if (!Blinded) 
        {
            int num_eyes = eyecount(youmonst.data);
            play_sfx_sound(SFX_SCALES_COVER_EYES);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Scales cover your %s!", (num_eyes == 1)
                                               ? body_part(EYE)
                                               : makeplural(body_part(EYE)));
            make_blinded(Half_spell_damage ? 100L : 200L, FALSE);
            if (!Blind)
                Your1(vision_clears);
            damage = 0;
        } else
            impossible("no reason for monster to cast blindness spell?");
        break;
    case CLC_PARALYZE:
        if (Antimagic_or_resistance || Free_action) {
            u_shieldeff();
            if (multi >= 0)
                You("stiffen briefly.");
            //nomul(-1);
            //multi_reason = "paralyzed by a monster";
        }
        else 
        {
            if (!Paralyzed)
            {
                play_sfx_sound(SFX_ACQUIRE_PARALYSIS);
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are frozen in place!");
            }
            int duration = d(3, 4); // 4 + (int)mtmp->m_lev;
            if (Half_spell_damage)
                duration = (duration + 1) / 2;
//            nomul(-duration);
//            multi_reason = "paralyzed by a monster";
            incr_itimeout(&HParalyzed, duration);

            /* No new paralysis for a while */
            set_itimeout(&HFree_action, 20);
            refresh_u_tile_gui_info(TRUE);
        }
        nomovemsg = 0;
        nomovemsg_attr = ATR_NONE;
        nomovemsg_color = NO_COLOR;
        damage = 0;
        break;
    case CLC_CONFUSE_YOU:
        if (Antimagic_or_resistance) {
            u_shieldeff();
            You_feel("momentarily dizzy.");
        } else {
            boolean oldprop = !!Confusion;

            int duration = (int) mtmp->m_lev;
            if (Half_spell_damage)
                duration = (duration + 1) / 2;
            if (!Confusion)
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            make_confused(itimeout_incr(HConfusion, duration), TRUE);
            if (Hallucination)
                You_feel_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "%s!", oldprop ? "trippier" : "trippy");
            else
                You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%sconfused!", oldprop ? "more " : "");
        }
        damage = 0;
        break;
    case CLC_CURE_SELF:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        play_sfx_sound_at_location(SFX_HEALING, mtmp->mx, mtmp->my);
        damage = m_cure_self(mtmp, is_prince(mtmp->data) ? d(12, 6) : is_lord(mtmp->data) ? d(6, 6) : d(2, 6));
        special_effect_wait_until_end(0);
        damage = 0;
        break;
    case CLC_OPEN_WOUNDS:
        if (Antimagic_or_resistance)
        {
            u_shieldeff();
            damage = damage / 2;
        }
        if (damage <= 5)
            Your_ex(ATR_NONE, CLR_MSG_WARNING, "skin itches badly for a moment.");
        else if (damage <= 10)
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Wounds appear on your body!");
        else if (damage <= 20)
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Severe wounds appear on your body!");
        else
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "body is covered with painful wounds!");
        break;
    default:
        impossible("mcastu: invalid clerical spell (%d)", spellnum);
        damage = 0;
        break;
    }

    if (damage > 0)
        mdamageu_with_hit_tile(mtmp, damage, TRUE, hit_tile);
}

STATIC_DCL
boolean
is_undirected_spell(adtyp, spellnum)
unsigned int adtyp;
int spellnum;
{
    if (adtyp == AD_SPEL) 
    {
        switch (spellnum) 
        {
        case MGC_CLONE_WIZ:
        case MGC_SUMMON_MONS:
        case MGC_SUMMON_NASTY:
        case MGC_AGGRAVATION:
        case MGC_DISAPPEAR:
        case MGC_HASTE_SELF:
        case MGC_CURE_SELF:
            return TRUE;
        default:
            break;
        }
    }
    else if (adtyp == AD_CLRC)
    {
        switch (spellnum) 
        {
        case CLC_INSECTS:
        case CLC_SNAKES:
        case CLC_CURE_SELF:
            return TRUE;
        default:
            break;
        }
    }
    return FALSE;
}

STATIC_DCL
boolean
is_ultimate_spell(adtyp, spellnum)
unsigned int adtyp;
int spellnum;
{
    if (adtyp == AD_SPEL)
    {
        switch (spellnum)
        {
        case MGC_CLONE_WIZ:
        case MGC_SUMMON_NASTY:
        case MGC_DEATH_TOUCH:
            return TRUE;
        default:
            break;
        }
    }
    else if (adtyp == AD_CLRC) 
    {
        switch (spellnum)
        {
        case CLC_SNAKES:
        case CLC_DEATH_TOUCH:
                return TRUE;
        default:
            break;
        }
    }
    return FALSE;
}

STATIC_DCL
boolean
is_intermediate_spell(adtyp, spellnum)
unsigned int adtyp;
int spellnum;
{
    if (adtyp == AD_SPEL) 
    {
        switch (spellnum)
        {
        case MGC_SUMMON_MONS:
            return TRUE;
        default:
            break;
        }
    }
    else if (adtyp == AD_CLRC)
    {
        switch (spellnum) 
        {
        case CLC_INSECTS:
        case CLC_GEYSER:
            return TRUE;
        default:
            break;
        }
    }
    return FALSE;
}

/* Some spells are useless under some circumstances. */
STATIC_DCL
boolean
spell_would_be_useless(mtmp, adtyp, spellnum)
struct monst *mtmp;
unsigned int adtyp;
int spellnum;
{
    /* Some spells don't require the player to really be there and can be cast
     * by the monster when you're invisible, yet still shouldn't be cast when
     * the monster doesn't even think you're there.
     * This check isn't quite right because it always uses your real position.
     * We really want something like "if the monster could see mux, muy".
     */
    boolean mcouldseeu = couldsee(mtmp->mx, mtmp->my);

    if (adtyp == AD_SPEL) 
    {
        /* aggravate monsters, etc. won't be cast by peaceful monsters */
        if (is_ultimate_spell(adtyp, spellnum) && mtmp->mmageultimate_used > 0)
            return TRUE;

        if (is_intermediate_spell(adtyp, spellnum) && mtmp->mmageintermediate_used > 0)
            return TRUE;

        if (is_peaceful(mtmp)
            && (spellnum == MGC_AGGRAVATION || spellnum == MGC_SUMMON_MONS || spellnum == MGC_SUMMON_NASTY || spellnum == MGC_DISAPPEAR
                || spellnum == MGC_CLONE_WIZ))
            return TRUE;

        /* haste self when already fast */
        if (has_very_fast(mtmp) && spellnum == MGC_HASTE_SELF)
            return TRUE;

        /* invisibility when already invisible */
        if ((has_invisibility(mtmp) || has_blocks_invisibility(mtmp)) && spellnum == MGC_DISAPPEAR)
            return TRUE;

        /* peaceful monster won't cast invisibility */
        if (spellnum == MGC_DISAPPEAR && is_peaceful(mtmp))
            return TRUE;

        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax && spellnum == MGC_CURE_SELF)
            return TRUE;

        /* don't summon monsters if it doesn't think you're around */
        if (!mcouldseeu && (spellnum == MGC_SUMMON_MONS || spellnum == MGC_SUMMON_NASTY
                            || (!mtmp->iswiz && spellnum == MGC_CLONE_WIZ)))
            return TRUE;

        if ((!mtmp->iswiz || context.no_of_wizards > 1)
            && spellnum == MGC_CLONE_WIZ)
            return TRUE;

        if ((!mtmp->iswiz || context.no_of_wizards > 1)
            && spellnum == MGC_CLONE_WIZ)
            return TRUE;

        if ((Curse_resistance || Antimagic) && spellnum == MGC_CURSE_ITEMS)
            return TRUE;

        if (Antimagic && (spellnum == MGC_WEAKEN_YOU || spellnum == MGC_STUN_YOU || spellnum == MGC_DESTRY_ARMR))
            return TRUE;

        if (spellnum == MGC_DESTRY_ARMR && !uarm && !uarmu && !uarmc && !uarms && !uarmg && !uarmf && !uarmh && !uarmo && !uarmb)
            return TRUE;

        /* aggravation (global wakeup) when everyone is already active */
        if (spellnum == MGC_AGGRAVATION) 
        {
            /* if nothing needs to be awakened then this spell is useless
               but caster might not realize that [chance to pick it then
               must be very small otherwise caller's many retry attempts
               will eventually end up picking it too often] */
            if (!has_aggravatables(mtmp))
                return rn2(100) ? TRUE : FALSE;
        }
    } 
    else if (adtyp == AD_CLRC) 
    {
        if (is_ultimate_spell(adtyp, spellnum) && mtmp->mclericultimate_used > 0)
            return TRUE;
        if (is_intermediate_spell(adtyp, spellnum) && mtmp->mclericintermediate_used > 0)
            return TRUE;
        /* summon insects/sticks to snakes won't be cast by peaceful monsters
         */
        if (is_peaceful(mtmp) && (spellnum == CLC_INSECTS || spellnum == CLC_SNAKES))
            return TRUE;
        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax && spellnum == CLC_CURE_SELF)
            return TRUE;
        /* don't summon insects if it doesn't think you're around */
        if (!mcouldseeu && (spellnum == CLC_INSECTS || spellnum == CLC_SNAKES))
            return TRUE;
        if (spellnum == CLC_INSECTS && mkclass(S_ANT, 0) == (struct permonst*)0)
            return TRUE;
        if (spellnum == CLC_SNAKES && mkclass(S_SNAKE, 0) == (struct permonst*)0)
            return TRUE;
        /* blindness spell on blinded player */
        if (spellnum == CLC_BLIND_YOU && Blinded)
            return TRUE;
        if (spellnum == CLC_PARALYZE && Free_action)
            return TRUE;
        if (spellnum == CLC_DEATH_TOUCH && (is_not_living(youmonst.data) || is_demon(youmonst.data) || Death_resistance))
            return TRUE;
        /* Only high priests and demon lords can cast the touch of death */
        if (mtmp->mnum != PM_HIGH_PRIEST && !((mtmp->data->geno & G_UNIQ) && (mtmp->data->mlet == S_DEMON))
            && spellnum == CLC_DEATH_TOUCH)
            return TRUE;

    }
    return FALSE;
}


/* monster uses spell (ranged) */
int
buzzmu(mtmp, mattk)
register struct monst *mtmp;
register struct attack *mattk;
{
    //Sanity check
    if (!mtmp || !mattk)
        return 0;

    /* Do not waste magic on reflecting targets */
    if (Reflecting && rn2(5))
        return 0;

    /* don't print constant stream of curse messages for 'normal'
       spellcasting monsters at range */

    int adtyp = mattk->adtyp, damn = mattk->damn, damd = mattk->damd, damp = mattk->damp;
    if (mattk->adtyp > AD_STON)
        set_m_ray_spell_stats(mtmp, mattk, &youmonst, &adtyp, &damn, &damd, &damp);

    if (adtyp < AD_MAGM || adtyp > AD_STON)
        return 0;

    if (is_cancelled(mtmp) || is_silenced(mtmp)) 
    {
        cursetxt(mtmp, FALSE);
        return (0);
    }

    unsigned short *appr_spec_ptr = &mtmp->mspec_used;
    boolean is_ultimate = FALSE;
    if (mattk->adtyp == AD_SPEL)
    {
        if (adtyp == AD_DRAY || adtyp == AD_DISN || adtyp == AD_STON)
        {
            appr_spec_ptr = &mtmp->mmageultimate_used;
            is_ultimate = TRUE;
        }
        else
            appr_spec_ptr = &mtmp->mmagespell_used;

    }
    else if (mattk->adtyp == AD_CLRC)
    {
        if (adtyp == AD_DRAY || adtyp == AD_DISN || adtyp == AD_STON)
        {
            appr_spec_ptr = &mtmp->mclericultimate_used;
            is_ultimate = TRUE;
        }
        else
            appr_spec_ptr = &mtmp->mclericspell_used;

    }

    int range = mattk->range ? mattk->range : M_RAY_RANGE;
    if (appr_spec_ptr && !(*appr_spec_ptr) && lined_up(mtmp, TRUE, adtyp, TRUE, range) && rn2(3))
    {
        nomul(0);
        if (adtyp && adtyp <= AD_STON)
        { /* no cf unsigned >0 */
            if (canseemon(mtmp))
            {
                int multicolors[2] = { NO_COLOR, CLR_BRIGHT_CYAN };
                pline_multi_ex(ATR_NONE, CLR_MSG_SPELL, no_multiattrs, multicolors, "%s casts \'%s\' at you!", Monnam(mtmp),
                    flash_types[ad_to_typ(adtyp)]);
            }
            buzz(-ad_to_typ(adtyp), (struct obj*)0, mtmp, damn, damd, damp, mtmp->mx, mtmp->my, sgn(tbx), sgn(tby));
            *appr_spec_ptr = (is_ultimate ? d(2, 8) + 100 : d(2, 4) + 10);
        }
        else
            impossible("Monster spell %d cast", mattk->adtyp - 1);
    }
    return (1);
}

/*mcastu.c*/
