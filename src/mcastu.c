/* GnollHack 4.0	mcastu.c	$NHDT-Date: 1436753517 2015/07/13 02:11:57 $  $NHDT-Branch: master $:$NHDT-Revision: 1.44 $ */
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
    MGC_CLONE_WIZ,
    MGC_DEATH_TOUCH
};

/* monster cleric spells */
enum mcast_cleric_spells {
    CLC_OPEN_WOUNDS = 0,
    CLC_CURE_SELF,
    CLC_CONFUSE_YOU,
    CLC_PARALYZE,
    CLC_BLIND_YOU,
    CLC_INSECTS,
    CLC_CURSE_ITEMS,
    CLC_LIGHTNING,
    CLC_FIRE_PILLAR,
    CLC_GEYSER,
	CLC_DEATH_TOUCH
};

STATIC_DCL void FDECL(cursetxt, (struct monst *, BOOLEAN_P));
STATIC_DCL int FDECL(choose_magic_spell, (int));
STATIC_DCL int FDECL(choose_clerical_spell, (int));
STATIC_DCL double FDECL(m_cure_self, (struct monst *, double));
STATIC_DCL void FDECL(cast_wizard_spell, (struct monst *, double, int));
STATIC_DCL void FDECL(cast_cleric_spell, (struct monst *, double, int));
STATIC_DCL boolean FDECL(is_undirected_spell, (unsigned int, int));
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

        pline("%s points %s.", Monnam(mtmp), point_msg);
    } else if ((!(moves % 4) || !rn2(4))) {
        if (!Deaf)
            Norep("You hear a mumbled curse.");
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
    while (spellval > 24 && rn2(25))
        spellval = rn2(spellval);

    switch (spellval) {
    case 24:
    case 23:
        if (Death_resistance || Hallucination) //Antimagic || 
            return MGC_PSI_BOLT;
        /*FALLTHRU*/
    case 22:
    case 21:
    case 20:
        return MGC_DEATH_TOUCH;
    case 19:
    case 18:
        return MGC_CLONE_WIZ;
    case 17:
    case 16:
    case 15:
        return MGC_SUMMON_MONS;
    case 14:
    case 13:
        return MGC_AGGRAVATION;
    case 12:
    case 11:
		return MGC_DISAPPEAR;
	case 10:
    case 9:
		return MGC_CURSE_ITEMS;
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
	if(mattk->mcadj > 0)
		ml = mattk->mcadj;
	else
		ml = mtmp->m_lev;

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
    if ((mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) && ml) {
        int cnt = 40;

        do {
            spellnum = rn2(ml);
            if (mattk->adtyp == AD_SPEL)
                spellnum = choose_magic_spell(spellnum);
            else
                spellnum = choose_clerical_spell(spellnum);
            /* not trying to attack?  don't allow directed spells */
            if (!thinks_it_foundyou) {
                if (!is_undirected_spell(mattk->adtyp, spellnum)
                    || spell_would_be_useless(mtmp, mattk->adtyp, spellnum)) {
                    if (foundyou)
                        impossible(
                       "spellcasting monster found you and doesn't know it?");
                    return 0;
                }
                break;
            }
        } while (--cnt > 0
                 && spell_would_be_useless(mtmp, mattk->adtyp, spellnum));
        if (cnt == 0)
            return 0;
    }

    /* monster unable to cast spells? */
    if (is_cancelled(mtmp) || is_silenced(mtmp) || mtmp->mspec_used || !ml) {
        cursetxt(mtmp, is_undirected_spell(mattk->adtyp, spellnum));
        return (0);
    }

    if (mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) {
        mtmp->mspec_used = 10 - mtmp->m_lev;
        if (mtmp->mspec_used < 2)
            mtmp->mspec_used = 2;
    }

    /* monster can cast spells, but is casting a directed spell at the
       wrong place?  If so, give a message, and return.  Do this *after*
       penalizing mspec_used. */
    if (!foundyou && thinks_it_foundyou
        && !is_undirected_spell(mattk->adtyp, spellnum)) {
        pline("%s casts a spell at %s!",
              canseemon(mtmp) ? Monnam(mtmp) : "Something",
              levl[mtmp->mux][mtmp->muy].typ == WATER ? "empty water"
                                                      : "thin air");
        return (0);
    }

    nomul(0);
	/*Spellnum + 1 is used as spell level 1...ml; chance of fail is 50% for ml and 0% for ml /2,
	  interpolated linearly*/
	int	failchance = 0;
	int sl = spellnum + 1;
	if (sl > ml / 2 && ml > 0) { // fail only if spell level is high enough
		failchance = (50 * (sl - ml / 2)) / (ml - ml / 2);
	}
	if (ml == 0)
		failchance = 100;

    if (rn2(100) < failchance) {//(rn2(ml * 10) < (is_confused(mtmp) ? 100 : 20)) { /* fumbled attack */
        if (canseemon(mtmp) && !Deaf)
            pline_The("air crackles around %s.", mon_nam(mtmp));
        return (0);
    }
    if (canspotmon(mtmp) || !is_undirected_spell(mattk->adtyp, spellnum)) {
        pline("%s casts a spell%s!",
              canspotmon(mtmp) ? Monnam(mtmp) : "Something",
              is_undirected_spell(mattk->adtyp, spellnum)
                  ? ""
                  : (Invisible && !has_see_invisible(mtmp)
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
            return (0);
        }
    } 
	else if (mattk->damd)
        damage = adjust_damage(d((int) ((ml / 2) + mattk->damn), (int) mattk->damd), mtmp, &youmonst, mattk->adtyp, TRUE);
    else
        damage = adjust_damage(d((int) ((ml / 2) + 1), 6), mtmp, &youmonst, mattk->adtyp, TRUE);

    ret = 1;

    switch (mattk->adtyp) {
    case AD_FIRE:
        pline("You're enveloped in flames.");
        if (Fire_resistance || Invulnerable) {
            shieldeff(u.ux, u.uy);
            pline("But you resist the effects.");
            damage = 0;
        }
        burn_away_slime();
        break;
    case AD_COLD:
        pline("You're covered in frost.");
        if (Cold_resistance || Invulnerable) {
            shieldeff(u.ux, u.uy);
            pline("But you resist the effects.");
            damage = 0;
        }
        break;
    case AD_MAGM:
        You("are hit by a shower of missiles!");
        if (Antimagic_or_resistance || Invulnerable) {
            shieldeff(u.ux, u.uy);
            pline_The("missiles bounce off!");
            damage = 0;
        } else
            damage = adjust_damage(d((int) mtmp->m_lev / 2 + 1, 6), mtmp, &youmonst, mattk->adtyp, TRUE);
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
            pline("%s looks better.", Monnam(mtmp));
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
    if (damage == 0 && !is_undirected_spell(AD_SPEL, spellnum)) {
        impossible("cast directed wizard spell (%d) with damage=0?", spellnum);
        return;
    }

    switch (spellnum) 
	{
    case MGC_DEATH_TOUCH:
        pline("Oh no, %s's using the touch of death!", mhe(mtmp));
		//boolean magic_resistance_success = check_magic_resistance_and_inflict_damage(&youmonst, (struct obj*)0, mtmp->m_lev, 0, 0, NOTELL);

        if (is_not_living(youmonst.data) || is_demon(youmonst.data) || Death_resistance)
		{ //Invulnerability does not protect against death attacks
            You("seem no deader than before.");
        }
		else if (!check_magic_cancellation_success(&youmonst, -4)) //(rn2(mtmp->m_lev) > 12) //!Antimagic && !magic_resistance_success && 
		{
            if (Hallucination) 
			{
                You("have an out of body experience.");
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
            //if (Antimagic || magic_resistance_success)
            //    shieldeff(u.ux, u.uy);
            pline("Lucky for you, it didn't work!");
        }
        damage = 0;
        break;
    case MGC_CLONE_WIZ:
        if (mtmp->iswiz && context.no_of_wizards == 1)
		{
            pline("Double Trouble...");
            clonewiz();
            damage = 0;
        } else
            impossible("bad wizard cloning?");
        break;
    case MGC_SUMMON_MONS: {
        int count;

        count = nasty(mtmp); /* summon something nasty */
        if (mtmp->iswiz)
            verbalize("Destroy the thief, my pet%s!", plur(count));
        else {
            const char *mappear =
                (count == 1) ? "A monster appears" : "Monsters appear";

            /* messages not quite right if plural monsters created but
               only a single monster is seen */
            if (Invisible && !has_see_invisible(mtmp)
                && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline("%s around a spot near you!", mappear);
            else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline("%s around your displaced image!", mappear);
            else
                pline("%s from nowhere!", mappear);
        }
        damage = 0;
        break;
    }
    case MGC_AGGRAVATION:
        You_feel("that monsters are aware of your presence.");
        aggravate();
        damage = 0;
        break;
    case MGC_CURSE_ITEMS:
        You_feel("as if you need some help.");
        rndcurse();
        damage = 0;
        break;
    case MGC_DESTRY_ARMR:
		if (Antimagic_or_resistance) {
			shieldeff(u.ux, u.uy);
			pline("A field of force surrounds you!");
		}
		else if (uarmc && uarmc->otyp == CLOAK_OF_INTEGRITY) {
            shieldeff(u.ux, u.uy);
            pline("Your cloak neutralizes the destructive energies of the spell!");
			makeknown(uarmc->otyp);
        } else if (!destroy_arm(some_armor(&youmonst))) {
            Your("skin itches.");
        }
        damage = 0;
        break;
    case MGC_WEAKEN_YOU: /* drain strength */
        if (Antimagic_or_resistance) {
            shieldeff(u.ux, u.uy);
            You_feel("momentarily weakened.");
        }
		else 
		{
            You("suddenly feel weaker!");
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
                pline("%s suddenly %s!", Monnam(mtmp),
                      !See_invisible ? "disappears" : "becomes transparent");
			increase_mon_property(mtmp, INVISIBILITY, d(2, 10) + 80);
            if (cansee(mtmp->mx, mtmp->my) && !canspotmon(mtmp))
                map_invisible(mtmp->mx, mtmp->my);
            damage = 0;
        } else
            impossible("no reason for monster to cast disappear spell?");
        break;
    case MGC_STUN_YOU:
        if (Antimagic_or_resistance || Free_action || Invulnerable) {
            shieldeff(u.ux, u.uy);
            if (!Stunned)
                You_feel("momentarily disoriented.");
            make_stunned(1L, FALSE);
        } else {
            You(Stunned ? "struggle to keep your balance." : "reel...");
            int stun_duration = d(ACURR(A_DEX) < 12 ? 6 : 4, 4);
            if (Half_spell_damage)
				stun_duration = (stun_duration + 1) / 2;
            make_stunned((HStun & TIMEOUT) + (long)stun_duration, FALSE);
        }
        damage = 0;
        break;
    case MGC_HASTE_SELF:
		increase_mon_property_verbosely(mtmp, VERY_FAST, 150 + rnd(50));
		damage = 0;
        break;
    case MGC_CURE_SELF:
        damage = m_cure_self(mtmp, damage);
        break;
    case MGC_PSI_BOLT:
        /* prior to 3.4.0 Antimagic was setting the damage to 1--this
           made the spell virtually harmless to players with magic res. */
        if (Antimagic_or_resistance || Invulnerable)
		{
            shieldeff(u.ux, u.uy);
            damage = damage / 2;
        }
        if (damage <= 5)
            You("get a slight %sache.", body_part(HEAD));
        else if (damage <= 10)
            Your("brain is on fire!");
        else if (damage <= 20)
            Your("%s suddenly aches painfully!", body_part(HEAD));
        else
            Your("%s suddenly aches very painfully!", body_part(HEAD));
        break;
    default:
        impossible("mcastu: invalid magic spell (%d)", spellnum);
        damage = 0;
        break;
    }

    if (damage > 0)
        mdamageu(mtmp, damage, TRUE);
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

    if (damage == 0 && !is_undirected_spell(AD_CLRC, spellnum)) {
        impossible("cast directed cleric spell (%d) with damage=0?", spellnum);
        return;
    }

    switch (spellnum) 
	{
	case CLC_DEATH_TOUCH:
		pline("Oh no, %s's using the touch of death!", mhe(mtmp));

		if (is_not_living(youmonst.data) || is_demon(youmonst.data) || Death_resistance)
		{
			You("seem no deader than before.");
		}
		else if (!check_magic_cancellation_success(&youmonst, -4))
		{
			if (Hallucination)
			{
				You("have an out of body experience.");
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
			pline("Lucky for you, it didn't work!");
		}
		damage = 0;
		break;
	case CLC_GEYSER:
        /* this is physical damage (force not heat),
         * not magical damage or fire damage
         */
        pline("A sudden geyser slams into you from nowhere!");
        damage = adjust_damage(d(8, 6), mtmp, &youmonst, AD_PHYS, TRUE);
        break;
    case CLC_FIRE_PILLAR:
        pline("A pillar of fire strikes all around you!");
        if (Fire_resistance) 
		{
            shieldeff(u.ux, u.uy);
            damage = 0;
        } else
            damage = adjust_damage(d(8, 6), mtmp, &youmonst, AD_FIRE, TRUE);
        burn_away_slime();
        (void) burnarmor(&youmonst);
        destroy_item(SCROLL_CLASS, AD_FIRE);
        destroy_item(POTION_CLASS, AD_FIRE);
        destroy_item(SPBOOK_CLASS, AD_FIRE);
        (void) burn_floor_objects(u.ux, u.uy, TRUE, FALSE);
        break;
    case CLC_LIGHTNING: {
        boolean reflects;

        pline("A bolt of lightning strikes down at you from above!");
        reflects = ureflects("It bounces off your %s%s.", "");
        if (reflects || Shock_resistance) {
            shieldeff(u.ux, u.uy);
            damage = 0;
            if (reflects)
                break;
        } else
            damage = adjust_damage(d(8, 6), mtmp, &youmonst, AD_ELEC, TRUE);
        destroy_item(WAND_CLASS, AD_ELEC);
        destroy_item(RING_CLASS, AD_ELEC);
        (void) flashburn((long) rnd(100));
        break;
    }
    case CLC_CURSE_ITEMS:
        You_feel("as if you need some help.");
        rndcurse();
        damage = 0;
        break;
    case CLC_INSECTS: {
        /* Try for insects, and if there are none
           left, go for (sticks to) snakes.  -3. */
        struct permonst *pm = mkclass(S_ANT, 0);
        struct monst *mtmp2 = (struct monst *) 0;
        char let = (pm ? S_ANT : S_SNAKE);
        boolean success = FALSE, seecaster;
        int i, quan, oldseen, newseen;
        coord bypos;
        const char *fmt;

        oldseen = monster_census(TRUE);
        quan = (mtmp->m_lev < 2) ? 1 : rnd((int) mtmp->m_lev / 2);
        if (quan < 3)
            quan = 3;
        for (i = 0; i <= quan; i++) {
            if (!enexto(&bypos, mtmp->mux, mtmp->muy, mtmp->data))
                break;
            if ((pm = mkclass(let, 0)) != 0
                && (mtmp2 = makemon(pm, bypos.x, bypos.y, MM_ANGRY)) != 0) {
                success = TRUE;
                mtmp2->msleeping = mtmp2->mpeaceful = mtmp2->mtame = 0;
				if (!mtmp2->mtame)
					mtmp2->ispartymember = FALSE;
				set_malign(mtmp2);
            }
        }
        newseen = monster_census(TRUE);

        /* not canspotmon(), which includes unseen things sensed via warning
         */
        seecaster = canseemon(mtmp) || tp_sensemon(mtmp) || Detect_monsters;

        fmt = 0;
        if (!seecaster) {
            char *arg; /* [not const: upstart(N==1 ? an() : makeplural())] */
            const char *what = (let == S_SNAKE) ? "snake" : "insect";

            if (newseen <= oldseen || Unaware) {
                /* unseen caster fails or summons unseen critters,
                   or unconscious hero ("You dream that you hear...") */
                You_hear("someone summoning %s.", makeplural(what));
            } else {
                /* unseen caster summoned seen critter(s) */
                arg = (newseen == oldseen + 1) ? an(what) : makeplural(what);
                if (!Deaf)
                    You_hear("someone summoning something, and %s %s.", arg,
                             vtense(arg, "appear"));
                else
                    pline("%s %s.", upstart(arg), vtense(arg, "appear"));
            }

            /* seen caster, possibly producing unseen--or just one--critters;
               hero is told what the caster is doing and doesn't necessarily
               observe complete accuracy of that caster's results (in other
               words, no need to fuss with visibility or singularization;
               player is told what's happening even if hero is unconscious) */
        } else if (!success)
            fmt = "%s casts at a clump of sticks, but nothing happens.";
        else if (let == S_SNAKE)
            fmt = "%s transforms a clump of sticks into snakes!";
        else if (Invisible && !has_see_invisible(mtmp)
                 && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            fmt = "%s summons insects around a spot near you!";
        else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            fmt = "%s summons insects around your displaced image!";
        else
            fmt = "%s summons insects!";
        if (fmt)
            pline(fmt, Monnam(mtmp));

        damage = 0;
        break;
    }
    case CLC_BLIND_YOU:
        /* note: resists_blnd() doesn't apply here */
        if (!Blinded) {
            int num_eyes = eyecount(youmonst.data);
            pline("Scales cover your %s!", (num_eyes == 1)
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
            shieldeff(u.ux, u.uy);
            if (multi >= 0)
                You("stiffen briefly.");
            nomul(-1);
            multi_reason = "paralyzed by a monster";
        } 
		else 
		{
            if (multi >= 0)
                You("are frozen in place!");
            int duration = 4 + (int) mtmp->m_lev;
            if (Half_spell_damage)
				duration = (duration + 1) / 2;
            nomul(-duration);
            multi_reason = "paralyzed by a monster";
        }
        nomovemsg = 0;
        damage = 0;
        break;
    case CLC_CONFUSE_YOU:
        if (Antimagic_or_resistance) {
            shieldeff(u.ux, u.uy);
            You_feel("momentarily dizzy.");
        } else {
            boolean oldprop = !!Confusion;

			int duration = (int) mtmp->m_lev;
            if (Half_spell_damage)
				duration = (duration + 1) / 2;
            make_confused(itimeout_incr(HConfusion, duration), TRUE);
            if (Hallucination)
                You_feel("%s!", oldprop ? "trippier" : "trippy");
            else
                You_feel("%sconfused!", oldprop ? "more " : "");
        }
        damage = 0;
        break;
    case CLC_CURE_SELF:
        damage = m_cure_self(mtmp, damage);
        break;
    case CLC_OPEN_WOUNDS:
        if (Antimagic_or_resistance)
		{
            shieldeff(u.ux, u.uy);
            damage = damage / 2;
        }
        if (damage <= 5)
            Your("skin itches badly for a moment.");
        else if (damage <= 10)
            pline("Wounds appear on your body!");
        else if (damage <= 20)
            pline("Severe wounds appear on your body!");
        else
            Your("body is covered with painful wounds!");
        break;
    default:
        impossible("mcastu: invalid clerical spell (%d)", spellnum);
        damage = 0;
        break;
    }

    if (damage > 0)
        mdamageu(mtmp, damage, TRUE);
}

STATIC_DCL
boolean
is_undirected_spell(adtyp, spellnum)
unsigned int adtyp;
int spellnum;
{
    if (adtyp == AD_SPEL) {
        switch (spellnum) {
        case MGC_CLONE_WIZ:
        case MGC_SUMMON_MONS:
        case MGC_AGGRAVATION:
        case MGC_DISAPPEAR:
        case MGC_HASTE_SELF:
        case MGC_CURE_SELF:
            return TRUE;
        default:
            break;
        }
    } else if (adtyp == AD_CLRC) {
        switch (spellnum) {
        case CLC_INSECTS:
        case CLC_CURE_SELF:
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

        if (is_peaceful(mtmp)
            && (spellnum == MGC_AGGRAVATION || spellnum == MGC_SUMMON_MONS
                || spellnum == MGC_CLONE_WIZ))
            return TRUE;

        /* haste self when already fast */
        if (has_very_fast(mtmp) && spellnum == MGC_HASTE_SELF)
            return TRUE;

        /* invisibility when already invisible */
        if ((has_invisibility(mtmp) || has_blocks_invisibility(mtmp)) && spellnum == MGC_DISAPPEAR)
            return TRUE;

        /* peaceful monster won't cast invisibility if you can't see
           invisible,
           same as when monsters drink potions of invisibility.  This doesn't
           really make a lot of sense, but lets the player avoid hitting
           peaceful monsters by mistake */
        if (is_peaceful(mtmp) && !See_invisible && spellnum == MGC_DISAPPEAR)
            return TRUE;

        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax && spellnum == MGC_CURE_SELF)
            return TRUE;

        /* don't summon monsters if it doesn't think you're around */
        if (!mcouldseeu && (spellnum == MGC_SUMMON_MONS
                            || (!mtmp->iswiz && spellnum == MGC_CLONE_WIZ)))
            return TRUE;

        if ((!mtmp->iswiz || context.no_of_wizards > 1)
            && spellnum == MGC_CLONE_WIZ)
            return TRUE;

		if (mtmp->mnum != PM_HIGH_PRIEST && mtmp->mnum != PM_WIZARD_OF_YENDOR && !((mtmp->data->geno & G_UNIQ) && (mtmp->data->mlet == S_DEMON))
			&& (spellnum == MGC_DEATH_TOUCH || spellnum == CLC_DEATH_TOUCH))
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
        /* summon insects/sticks to snakes won't be cast by peaceful monsters
         */
        if (is_peaceful(mtmp) && spellnum == CLC_INSECTS)
            return TRUE;
        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax && spellnum == CLC_CURE_SELF)
            return TRUE;
        /* don't summon insects if it doesn't think you're around */
        if (!mcouldseeu && spellnum == CLC_INSECTS)
            return TRUE;
        /* blindness spell on blinded player */
        if (Blinded && spellnum == CLC_BLIND_YOU)
            return TRUE;
    }
    return FALSE;
}

/* convert 1..10 to 0..9; add 10 for second group (spell casting) */
#define ad_to_typ(k) (10 + (int) k - 1)

/* monster uses spell (ranged) */
int
buzzmu(mtmp, mattk)
register struct monst *mtmp;
register struct attack *mattk;
{
	//Sanity check
	if (!mtmp || !mattk)
		return 0;
	
	/* don't print constant stream of curse messages for 'normal'
       spellcasting monsters at range */

    if (mattk->adtyp > AD_SPC2)
        return (0);

    if (is_cancelled(mtmp) || is_silenced(mtmp)) {
        cursetxt(mtmp, FALSE);
        return (0);
    }
    if (lined_up(mtmp, TRUE, mattk->adtyp, TRUE) && rn2(3))
	{
        nomul(0);
        if (mattk->adtyp && (mattk->adtyp <= AD_SPC2))
		{ /* no cf unsigned >0 */
            if (canseemon(mtmp))
                pline("%s zaps you with a %s!", Monnam(mtmp),
                      flash_types[ad_to_typ(mattk->adtyp)]);
			buzz(-ad_to_typ(mattk->adtyp), (struct obj*)0, (int)mattk->damn, (int)mattk->damd, (int)mattk->damp, mtmp->mx,
                 mtmp->my, sgn(tbx), sgn(tby));
        } else
            impossible("Monster spell %d cast", mattk->adtyp - 1);
    }
    return (1);
}

/*mcastu.c*/
