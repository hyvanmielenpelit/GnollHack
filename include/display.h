/* GnollHack 4.0	display.h	$NHDT-Date: 1546212620 2018/12/30 23:30:20 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.29 $ */
/* Copyright (c) Dean Luick, with acknowledgements to Kevin Darcy */
/* and Dave Cohrs, 1990.                                          */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef DISPLAY_H
#define DISPLAY_H

#ifndef VISION_H
#include "vision.h"
#endif
#ifndef MONDATA_H
#include "mondata.h" /* for mindless() */
#endif
#ifndef YOU_H
#include "you.h"
#endif
#ifndef LAYER_H
#include "layer.h"
#endif

/*
 * vobj_at()
 *
 * Returns the head of the list of objects that the player can see
 * at location (x,y).
 */
#define vobj_at(x, y) (level.objects[x][y])

/*
 * sensemon()
 *
 * Returns true if the hero can sense the given monster.  This includes
 * monsters that are hiding or mimicing other monsters.
 */
#define tp_sensemon(mon) \
    (/* The hero can always sense a monster IF:        */  \
     /* 1. the monster has a brain to sense            */  \
     (!mindless(mon->data))                                \
     /* AND     2a. hero is blind and telepathic       */  \
      && ((Blind && (Blind_telepat || Unblind_telepat))                         \
          /* OR 2b. hero is using a telepathy inducing */  \
          /*        object and in range                */  \
          || (Unblind_telepat                              \
              && (distu(mon->mx, mon->my) <= (TELEPATHY_RANGE * TELEPATHY_RANGE)))))

#define sensemon(mon) \
    (tp_sensemon(mon) || Detect_monsters || MATCH_WARN_OF_MON(mon))

/*
 * mon_warning() is used to warn of any dangerous monsters in your
 * vicinity, and a glyph representing the warning level is displayed.
 */

#define mon_warning(mon)                                                 \
    ((Warning || \
		(Undead_warning && mon->data->mflags2 & M2_UNDEAD) \
		|| (Demon_warning && mon->data->mflags2 & M2_DEMON) \
		|| (Orc_warning && mon->data->mflags2 & M2_ORC) \
		|| (Troll_warning && mon->data->mlet == S_TROLL) \
		|| (Giant_warning && mon->data->mflags2 & M2_GIANT) \
		|| (Dragon_warning && mon->data->mlet == S_DRAGON) \
		|| (Elf_warning && mon->data->mflags2 & M2_ELF) \
		|| (Dwarf_warning && mon->data->mflags2 & M2_DWARF) \
		|| (Gnoll_warning && mon->data->mflags2 & M2_GNOLL) \
		|| (Gnome_warning && mon->data->mflags2 & M2_GNOME) \
		|| (Human_warning && mon->data->mflags2 & M2_HUMAN) \
		|| (Lycanthrope_warning && mon->data->mflags2 & M2_WERE) \
		|| (Angel_warning && mon->data->mflags2 & M2_ANGEL) \
		|| (Ogre_warning && mon->data->mlet == S_OGRE) \
	) && !is_peaceful((mon)) && (distu((mon)->mx, (mon)->my) < 100))

/*     && (((int) ((mon)->m_lev / 4)) >= context.warnlevel)) */

/*
 * mon_visible()
 *
 * Returns true if the hero can see the monster.  It is assumed that the
 * hero can physically see the location of the monster.  The function
 * vobj_at() returns a pointer to an object that the hero can see there.
 * Infravision is not taken into account.
 */
#define mon_visible(mon) \
    (/* The hero can see the monster IF the monster                     */ \
     (!is_invisible(mon) || See_invisible)  /*     1. is not invisible        */ \
     && !mon->mundetected             /* AND 2. not an undetected hider */ \
     && !(mon->mburied || u.uburied)) /* AND 3. neither you nor it is buried */

/*
 * see_with_infrared()
 *
 * This function is true if the player can see a monster using infravision.
 * The caller must check for invisibility (invisible monsters are also
 * invisible to infravision), because this is usually called from within
 * canseemon() or canspotmon() which already check that.
 */
#define see_with_infrared(mon)                        \
    (!Blind && Infravision && mon && infravisible(mon->data) \
     && couldsee(mon->mx, mon->my))

/*
 * canseemon()
 *
 * This is the globally used canseemon().  It is not called within the display
 * routines.  Like mon_visible(), but it checks to see if the hero sees the
 * location instead of assuming it.  (And also considers worms.)
 */
#define canseemon(mon)                                                    \
    ((mon->wormno ? worm_known(mon)                                       \
                  : (cansee(mon->mx, mon->my) || see_with_infrared(mon))) \
     && mon_visible(mon))

/*
 * canspotmon(mon)
 *
 * This function checks whether you can either see a monster or sense it by
 * telepathy, and is what you usually call for monsters about which nothing is
 * known.
 */
#define canspotmon(mon) (canseemon(mon) || sensemon(mon))

/* knowninvisible(mon)
 * This one checks to see if you know a monster is both there and invisible.
 * 1) If you can see the monster and have see invisible, it is assumed the
 * monster is transparent, but visible in some manner.  (Earlier versions of
 * GnollHack were really inconsistent on this.)
 * 2) If you can't see the monster, but can see its location and you have
 * telepathy that works when you can see, you can tell that there is a
 * creature in an apparently empty spot.
 * Infravision is not relevant; we assume that invisible monsters are also
 * invisible to infravision.
 */
#define knowninvisible(mon)                                               \
    (is_invisible(mon)                                                        \
     && ((cansee(mon->mx, mon->my) && (See_invisible || Detect_monsters)) \
         || (Telepat && distu(mon->mx, mon->my) <= (TELEPATHY_RANGE * TELEPATHY_RANGE))))

/*
 * is_safepet(mon)
 *
 * A special case check used in attack() and domove().  Placing the
 * definition here is convenient.
 */
#define is_safepet(mon)                                                   \
    (mon && is_tame(mon) && canspotmon(mon) && flags.safe_dog && !Confusion \
     && !Hallucination && !Stunned)

/*
 * canseeself()
 * senseself()
 * canspotself()
 *
 * This returns true if the hero can see her/himself.
 *
 * Sensing yourself by touch is treated as seeing yourself, even if
 * unable to see.  So when blind, being invisible won't affect your
 * self-perception, and when swallowed, the enclosing monster touches.
 */
#define canseeself() (Blind || u.uswallow || (!Invisible && !u.uundetected))
#define senseself() (Unblind_telepat || Detect_monsters)
#define canspotself() (canseeself() || senseself())

/*
 * random_monster()
 * random_object()
 * random_trap()
 *
 * Respectively return a random monster, object, or trap number.
 */
#define random_monster(rng) rng(NUM_MONSTERS)
#define random_object(rng) (rng(NUM_OBJECTS - 1) + 1)
#define random_trap(rng) (rng(TRAPNUM - 1) + 1)

/*
 * what_obj()
 * what_mon()
 * what_trap()
 *
 * If hallucinating, choose a random object/monster, otherwise, use the one
 * given. Use the given rng to handle hallucination.
 */
#define what_obj(obj, rng) (Hallucination ? random_object(rng) : obj)
#define what_mon(mon, rng) (Hallucination ? random_monster(rng) : mon)
#define what_trap(trp, rng) (Hallucination ? random_trap(rng) : trp)

/*
 * newsym_rn2
 *
 * An appropriate random number generator for use with newsym(), when
 * randomness is needed there. This is currently hardcoded as
 * rn2_on_display_rng, but is futureproofed for cases where we might
 * want to prevent display-random objects entering the character's
 * memory (this isn't important at present but may be if we need
 * reproducible gameplay for some reason).
 */
#define newsym_rn2 rn2_on_display_rng

/*
 * covers_objects()
 * covers_traps()
 *
 * These routines are true if what is really at the given location will
 * "cover" any objects or traps that might be there.
 */
#define covers_objects(xx, yy) \
    ((is_pool(xx, yy) && !Underwater) || (levl[xx][yy].typ == LAVAPOOL))

#define covers_traps(xx, yy) covers_objects(xx, yy)

/*
 * tmp_at() control calls.
 */
#define DISP_BEAM    (-1) /* Keep all glyphs showing & clean up at end. */
#define DISP_ALL     (-2) /* Like beam, but still displayed if not visible. */
#define DISP_TETHER  (-3) /* Like beam, but tether glyph differs from final */
#define DISP_FLASH   (-4) /* Clean up each glyph before displaying new one. */
#define DISP_ALWAYS  (-5) /* Like flash, but still displayed if not visible. */
#define DISP_CHANGE  (-6) /* Change glyph. */
#define DISP_END     (-7) /* Clean up. */
#define DISP_FREEMEM (-8) /* Free all memory during exit only. */
#define DISP_BEAM_DIG (-9) /* Keep all glyphs showing & clean up at end. */

/* Total number of cmap indices in the shield_static[] array. */
#define SHIELD_COUNT 21
/* Total number of object indices in the talk_static[] array. */
#define TALK_COUNT 21

#define BACKTRACK (-1)    /* flag for DISP_END to display each prior location */

/*
 * A glyph is an abstraction that represents a _unique_ monster, object,
 * dungeon part, or effect.  The uniqueness is important.  For example,
 * It is not enough to have four (one for each "direction") zap beam glyphs,
 * we need a set of four for each beam type.  Why go to so much trouble?
 * Because it is possible that any given window dependent display driver
 * [print_glyph()] can produce something different for each type of glyph.
 * That is, a beam of cold and a beam of fire would not only be different
 * colors, but would also be represented by different symbols.
 *
 * Glyphs are grouped for easy accessibility:
 *
 * monster      Represents all the wild (not tame) non-female monsters.  Count: NUM_MONSTERS.
 *
 * female mon   Represents all the wild (not tame) female monsters.  Count: NUM_MONSTERS.
 *
 * invisible    Invisible monster placeholder.  Count: 1
 *
 * object       One for each object.  Count: NUM_OBJECTS
 *
 * cmap         One for each entry in the character map for each dungeon type. 
 *              The character map is the dungeon features and other miscellaneous things.
 *              Count: MAX_CMAPPED_CHARS * CMAP_TYPE_MAX
 *
 * explosions   A set of nine for each of the following seven explosion types:
 *                   dark, noxious, muddy, wet, magical, fiery, frosty.
 *              The nine positions represent those surrounding the hero.
 *              Count: MAX_EXPLOSION_CHARS * EXPL_MAX (EXPL_MAX is defined in hack.h)
 *
 * zap beam     A set of four (there are four directions) for each beam type.
 *              The beam type is shifted over 2 positions and the direction
 *              is stored in the lower 2 bits.  Count: NUM_ZAP * MAX_ZAP_CHARS
 *
 * swallow      A set of eight for each monster.  The eight positions rep-
 *              resent those surrounding the hero.  The monster number is
 *              shifted over 3 positions and the swallow position is stored
 *              in the lower three bits.  Count: NUM_MONSTERS * MAX_SWALLOW_CHARS
 *
 * warning      A set of six representing the different warning levels.
 *
 * statue       One for each monster.  Count: NUM_MONSTERS
 *
 * female statue  One for each female monster.  Count: NUM_MONSTERS
 *
 * corpse       One for each monster.  Count: NUM_MONSTERS
 *
 * female corpse  One for each female monster.  Count: NUM_MONSTERS
 *
 * artifact     One for each artifact.  Count: NUM_ARTIFACTS
 *
 * player       One for each type of player character.  
 *              Count: number of genders (2) x number of roles (13) x number of races (5) x alignment (3) x player glyph levels (1)
 *
 * The following are offsets used to convert to and from a glyph.
 */
#define NUM_PLAYER_GLYPH_LEVELS 1
#define NUM_PLAYER_CHARACTERS NUM_ROLES * NUM_RACES * 2 * 3 * NUM_PLAYER_GLYPH_LEVELS
#define CMAP_TYPE_CHAR_NUM (MAX_CMAPPED_CHARS - MAX_EXPLOSION_CHARS - MAX_SWALLOW_CHARS)
#define NUM_INVIS_GLYPHS 1
#define NUM_MISSILE_DIRS 8

#define GLYPH_MON_OFF     (1) /* Changed to 1 so that mirror image of mon 0 maps to -1 */
#define GLYPH_ATTACK_OFF  (NUM_MONSTERS + GLYPH_MON_OFF) /* Must be in same order as enum action_tile_types */
#define GLYPH_THROW_OFF  (NUM_MONSTERS + GLYPH_ATTACK_OFF)
#define GLYPH_FIRE_OFF  (NUM_MONSTERS + GLYPH_THROW_OFF)
#define GLYPH_CAST_NODIR_OFF  (NUM_MONSTERS + GLYPH_FIRE_OFF)
#define GLYPH_CAST_DIR_OFF  (NUM_MONSTERS + GLYPH_CAST_NODIR_OFF)
#define GLYPH_SPECIAL_ATTACK_OFF  (NUM_MONSTERS + GLYPH_CAST_DIR_OFF)
#define GLYPH_KICK_OFF  (NUM_MONSTERS + GLYPH_SPECIAL_ATTACK_OFF)
#define GLYPH_PASSIVE_DEFENSE_OFF  (NUM_MONSTERS + GLYPH_KICK_OFF)
#define GLYPH_DEFEND_OFF  (NUM_MONSTERS + GLYPH_PASSIVE_DEFENSE_OFF)
#define GLYPH_RECEIVE_DAMAGE_OFF  (NUM_MONSTERS + GLYPH_DEFEND_OFF)
#define GLYPH_ITEM_USE_OFF  (NUM_MONSTERS + GLYPH_RECEIVE_DAMAGE_OFF)
#define GLYPH_DOOR_USE_OFF  (NUM_MONSTERS + GLYPH_ITEM_USE_OFF)
#define GLYPH_DEATH_OFF  (NUM_MONSTERS + GLYPH_DOOR_USE_OFF)
#define GLYPH_FEMALE_MON_OFF  (NUM_MONSTERS + GLYPH_DEATH_OFF) /* Must be immediately after base monsters */
#define GLYPH_FEMALE_ATTACK_OFF  (NUM_MONSTERS + GLYPH_FEMALE_MON_OFF) /* Must be in same order as enum action_tile_types */
#define GLYPH_FEMALE_THROW_OFF  (NUM_MONSTERS + GLYPH_FEMALE_ATTACK_OFF)
#define GLYPH_FEMALE_FIRE_OFF  (NUM_MONSTERS + GLYPH_FEMALE_THROW_OFF)
#define GLYPH_FEMALE_CAST_NODIR_OFF  (NUM_MONSTERS + GLYPH_FEMALE_FIRE_OFF)
#define GLYPH_FEMALE_CAST_DIR_OFF  (NUM_MONSTERS + GLYPH_FEMALE_CAST_NODIR_OFF)
#define GLYPH_FEMALE_SPECIAL_ATTACK_OFF  (NUM_MONSTERS + GLYPH_FEMALE_CAST_DIR_OFF)
#define GLYPH_FEMALE_KICK_OFF  (NUM_MONSTERS + GLYPH_FEMALE_SPECIAL_ATTACK_OFF)
#define GLYPH_FEMALE_PASSIVE_DEFENSE_OFF  (NUM_MONSTERS + GLYPH_FEMALE_KICK_OFF)
#define GLYPH_FEMALE_DEFEND_OFF  (NUM_MONSTERS + GLYPH_FEMALE_PASSIVE_DEFENSE_OFF)
#define GLYPH_FEMALE_RECEIVE_DAMAGE_OFF  (NUM_MONSTERS + GLYPH_FEMALE_DEFEND_OFF)
#define GLYPH_FEMALE_ITEM_USE_OFF  (NUM_MONSTERS + GLYPH_FEMALE_RECEIVE_DAMAGE_OFF)
#define GLYPH_FEMALE_DOOR_USE_OFF  (NUM_MONSTERS + GLYPH_FEMALE_ITEM_USE_OFF)
#define GLYPH_FEMALE_DEATH_OFF  (NUM_MONSTERS + GLYPH_FEMALE_DOOR_USE_OFF)
#define GLYPH_INVIS_OFF   (NUM_MONSTERS + GLYPH_FEMALE_DEATH_OFF)
#define GLYPH_OBJ_OFF     (NUM_INVIS_GLYPHS + GLYPH_INVIS_OFF)
#define GLYPH_OBJ_MISSILE_OFF    (NUM_OBJECTS + GLYPH_OBJ_OFF)
#define GLYPH_CMAP_OFF    (NUM_OBJECTS * NUM_MISSILE_DIRS + GLYPH_OBJ_MISSILE_OFF)
#define GLYPH_BROKEN_CMAP_OFF    (CMAP_TYPE_CHAR_NUM * CMAP_TYPE_MAX + GLYPH_CMAP_OFF)
#define GLYPH_CMAP_VARIATION_OFF    (CMAP_TYPE_CHAR_NUM * CMAP_TYPE_MAX + GLYPH_BROKEN_CMAP_OFF)
#define GLYPH_BROKEN_CMAP_VARIATION_OFF (MAX_VARIATIONS * CMAP_TYPE_MAX  + GLYPH_CMAP_VARIATION_OFF)
#define GLYPH_EXPLODE_OFF (MAX_VARIATIONS * CMAP_TYPE_MAX  + GLYPH_BROKEN_CMAP_VARIATION_OFF)
#define GLYPH_ZAP_OFF     ((MAX_EXPLOSION_CHARS * EXPL_MAX) + GLYPH_EXPLODE_OFF)
#define GLYPH_SWALLOW_OFF ((NUM_ZAP * MAX_ZAP_CHARS) + GLYPH_ZAP_OFF)
#define GLYPH_WARNING_OFF ((NUM_MONSTERS * MAX_SWALLOW_CHARS) + GLYPH_SWALLOW_OFF)
#define GLYPH_STATUE_OFF  (WARNCOUNT + GLYPH_WARNING_OFF)
#define GLYPH_FEMALE_STATUE_OFF (NUM_MONSTERS + GLYPH_STATUE_OFF)
#define GLYPH_BODY_OFF    (NUM_MONSTERS + GLYPH_FEMALE_STATUE_OFF)
#define GLYPH_FEMALE_BODY_OFF    (NUM_MONSTERS + GLYPH_BODY_OFF)
#define GLYPH_ARTIFACT_OFF (NUM_MONSTERS + GLYPH_FEMALE_BODY_OFF)
#define GLYPH_ARTIFACT_MISSILE_OFF (NUM_ARTIFACTS + GLYPH_ARTIFACT_OFF)
#define GLYPH_PLAYER_OFF  (NUM_ARTIFACTS * NUM_MISSILE_DIRS + GLYPH_ARTIFACT_MISSILE_OFF)
#define GLYPH_PLAYER_ATTACK_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_OFF) /* Must be in same order as enum action_tile_types */
#define GLYPH_PLAYER_THROW_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_ATTACK_OFF)
#define GLYPH_PLAYER_FIRE_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_THROW_OFF)
#define GLYPH_PLAYER_CAST_NODIR_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_FIRE_OFF)
#define GLYPH_PLAYER_CAST_DIR_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_CAST_NODIR_OFF)
#define GLYPH_PLAYER_SPECIAL_ATTACK_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_CAST_DIR_OFF)
#define GLYPH_PLAYER_KICK_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_SPECIAL_ATTACK_OFF)
#define GLYPH_PLAYER_PASSIVE_DEFENSE_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_KICK_OFF)
#define GLYPH_PLAYER_DEFEND_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_PASSIVE_DEFENSE_OFF)
#define GLYPH_PLAYER_RECEIVE_DAMAGE_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_DEFEND_OFF)
#define GLYPH_PLAYER_ITEM_USE_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_RECEIVE_DAMAGE_OFF)
#define GLYPH_PLAYER_DOOR_USE_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_ITEM_USE_OFF)
#define GLYPH_PLAYER_DEATH_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_DOOR_USE_OFF)
#define GLYPH_CURSOR_OFF  (NUM_PLAYER_CHARACTERS + GLYPH_PLAYER_DEATH_OFF)
#define GLYPH_UI_TILE_OFF  (MAX_CURSORS + GLYPH_CURSOR_OFF)
#define GLYPH_REPLACEMENT_OFF  (MAX_UI_TILES + GLYPH_UI_TILE_OFF)
#define GLYPH_ANIMATION_OFF  (TOTAL_REPLACEMENT_TILES + GLYPH_REPLACEMENT_OFF)
#define GLYPH_ENLARGEMENT_OFF  (TOTAL_ANIMATION_FRAMES + GLYPH_ANIMATION_OFF)
#define MAX_GLYPH         (TOTAL_ENLARGEMENT_TILES + GLYPH_ENLARGEMENT_OFF)

#define NO_GLYPH          MAX_GLYPH
#define GLYPH_INVISIBLE   GLYPH_INVIS_OFF

#define warning_to_glyph(mwarnlev) ((mwarnlev) + GLYPH_WARNING_OFF)

#define mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_MON_OFF))
#define attacking_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_ATTACK_OFF))
#define throwing_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_THROW_OFF))
#define firing_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FIRE_OFF))
#define nodir_casting_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_CAST_NODIR_OFF))
#define dir_casting_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_CAST_DIR_OFF))
#define special_attacking_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_SPECIAL_ATTACK_OFF))
#define kicking_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_KICK_OFF))
#define passive_defense_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_PASSIVE_DEFENSE_OFF))
#define defending_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_DEFEND_OFF))
#define damage_receiving_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_RECEIVE_DAMAGE_OFF))
#define item_using_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_ITEM_USE_OFF))
#define door_using_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_DOOR_USE_OFF))
#define dying_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_DEATH_OFF))
#define passive_defense_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_PASSIVE_DEFENSE_OFF))
#define defending_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_DEFEND_OFF))
#define damage_receiving_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_RECEIVE_DAMAGE_OFF))

#define female_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_MON_OFF))
#define female_attacking_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_ATTACK_OFF))
#define female_throwing_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_THROW_OFF))
#define female_firing_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_FIRE_OFF))
#define female_nodir_casting_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_CAST_NODIR_OFF))
#define female_dir_casting_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_CAST_DIR_OFF))
#define female_special_attacking_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_SPECIAL_ATTACK_OFF))
#define female_kicking_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_KICK_OFF))
#define female_passive_defense_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_PASSIVE_DEFENSE_OFF))
#define female_defending_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_DEFEND_OFF))
#define female_damage_receiving_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_RECEIVE_DAMAGE_OFF))
#define female_item_using_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_ITEM_USE_OFF))
#define female_door_using_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_DOOR_USE_OFF))
#define female_dying_mon_to_glyph(mon, rng)                               \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_DEATH_OFF))
#define female_passive_defense_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_PASSIVE_DEFENSE_OFF))
#define female_defending_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_DEFEND_OFF))
#define female_damage_receiving_mon_to_glyph(mon, rng)                                      \
    (((mon)->facing_right ? -1 : 1) * ((int) what_mon(monsndx((mon)->data), rng) + GLYPH_FEMALE_RECEIVE_DAMAGE_OFF))

#define any_mon_to_glyph(mon, rng)    \
  ((mon) == &youmonst ? u_to_glyph() : \
   ( \
        (mon)->action == ACTION_TILE_ATTACK ? ((mon)->female ? female_attacking_mon_to_glyph(mon, rng) : attacking_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_THROW ? ((mon)->female ? female_throwing_mon_to_glyph(mon, rng) : throwing_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_FIRE ? ((mon)->female ? female_firing_mon_to_glyph(mon, rng) : firing_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_CAST_NODIR ? ((mon)->female ? female_nodir_casting_mon_to_glyph(mon, rng) : nodir_casting_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_CAST_DIR ? ((mon)->female ? female_dir_casting_mon_to_glyph(mon, rng) : dir_casting_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_SPECIAL_ATTACK ? ((mon)->female ? female_special_attacking_mon_to_glyph(mon, rng) : special_attacking_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_KICK ? ((mon)->female ? female_kicking_mon_to_glyph(mon, rng) : kicking_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_PASSIVE_DEFENSE ? ((mon)->female ? female_passive_defense_mon_to_glyph(mon, rng) : passive_defense_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_DEFEND ? ((mon)->female ? female_defending_mon_to_glyph(mon, rng) : defending_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_RECEIVE_DAMAGE ? ((mon)->female ? female_damage_receiving_mon_to_glyph(mon, rng) : damage_receiving_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_ITEM_USE ? ((mon)->female ? female_item_using_mon_to_glyph(mon, rng) : item_using_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_DOOR_USE ? ((mon)->female ? female_door_using_mon_to_glyph(mon, rng) : door_using_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_DEATH ? ((mon)->female ? female_dying_mon_to_glyph(mon, rng) : dying_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_PASSIVE_DEFENSE ? ((mon)->female ? female_passive_defense_mon_to_glyph(mon, rng) : passive_defense_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_DEFEND ? ((mon)->female ? female_defending_mon_to_glyph(mon, rng) : defending_mon_to_glyph(mon, rng)) : \
        (mon)->action == ACTION_TILE_RECEIVE_DAMAGE ? ((mon)->female ? female_damage_receiving_mon_to_glyph(mon, rng) : damage_receiving_mon_to_glyph(mon, rng)) : \
        ((mon)->female ? female_mon_to_glyph(mon, rng) : mon_to_glyph(mon, rng)) \
   ))



#define is_obj_activated(obj) \
  ((obj)->lamplit || (obj)->invokeon || (obj)->detectioncount > 0)

/* This has the unfortunate side effect of needing a global variable    */
/* to store a result. 'otg_temp' is defined and declared in decl.{ch}.  */
#define random_obj_to_glyph(rng)                \
    ((otg_temp = random_object(rng)) == CORPSE  \
         ? random_monster(rng) + (!rn2(2) ? GLYPH_BODY_OFF : GLYPH_FEMALE_BODY_OFF) \
         : otg_temp + GLYPH_OBJ_OFF)

#define random_obj_to_missile_glyph(dir_index, rng)                \
    ((dir_index) < 0 || (dir_index) >= NUM_MISSILE_DIRS ? NO_GLYPH : \
    ((otg_temp = random_object(rng)) == CORPSE  \
         ? (((dir_index) == 2 || (dir_index) == 4 || (dir_index) == 7 ? -1 : 1) * (random_monster(rng) + (!rn2(2) ? GLYPH_BODY_OFF : GLYPH_FEMALE_BODY_OFF)))  \
         : otg_temp == STATUE  \
         ? (((dir_index) == 2 || (dir_index) == 4 || (dir_index) == 7 ? -1 : 1) * (random_monster(rng) + (!rn2(2) ? GLYPH_STATUE_OFF : GLYPH_FEMALE_STATUE_OFF))) \
         : otg_temp * NUM_MISSILE_DIRS + (dir_index) + GLYPH_OBJ_MISSILE_OFF))

#define statue_to_glyph(obj, rng)                              \
    (Hallucination ? (is_corpse_or_statue_facing_right(obj) ? -1 : 1) * (random_monster(rng) + GLYPH_MON_OFF)     \
                   : (is_corpse_or_statue_facing_right(obj) ? -1 : 1) * ((int) (obj)->corpsenm + GLYPH_STATUE_OFF))

#define female_statue_to_glyph(obj, rng)                              \
    (Hallucination ? (is_corpse_or_statue_facing_right(obj) ? -1 : 1) * (random_monster(rng) + GLYPH_FEMALE_MON_OFF)    \
                   : (is_corpse_or_statue_facing_right(obj) ? -1 : 1) * ((int) (obj)->corpsenm + GLYPH_FEMALE_STATUE_OFF))


/* MRKR: Statues now have glyphs corresponding to the monster they    */
/*       represent and look like monsters when you are hallucinating. */

#define base_cmap_to_glyph(cmap_idx) ((int) (cmap_idx) + GLYPH_CMAP_OFF)
#define base_cmap_variation_to_glyph(var_idx) ((int) (var_idx) + GLYPH_CMAP_VARIATION_OFF)
#define cmap_to_glyph(cmap_idx) ((int) (cmap_idx) + get_current_cmap_type_index() * CMAP_TYPE_CHAR_NUM + GLYPH_CMAP_OFF)
#define broken_cmap_to_glyph(bcmap_idx) ((int) (bcmap_idx) + get_current_cmap_type_index() * CMAP_TYPE_CHAR_NUM + GLYPH_BROKEN_CMAP_OFF)
#define cmap_variation_to_glyph(var_idx) ((int) (var_idx)+ get_current_cmap_type_index() * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF)
#define broken_cmap_variation_to_glyph(var_idx) ((int) (var_idx)+ get_current_cmap_type_index() * MAX_VARIATIONS + GLYPH_BROKEN_CMAP_VARIATION_OFF)

#define obj_to_glyph(obj, rng)                                          \
    (((obj)->otyp == STATUE)                                            \
         ? (is_female_corpse_or_statue(obj) ? female_statue_to_glyph(obj, rng) : statue_to_glyph(obj, rng) )                                   \
         : Hallucination                                                \
               ? random_obj_to_glyph(rng)                               \
               : ((obj)->otyp == CORPSE)                                \
                     ?  (is_female_corpse_or_statue(obj) ? ((is_corpse_or_statue_facing_right(obj) ? -1 : 1) * ((int) (obj)->corpsenm + GLYPH_FEMALE_BODY_OFF))  : ((is_corpse_or_statue_facing_right(obj) ? -1 : 1) * ((int) (obj)->corpsenm + GLYPH_BODY_OFF)) )         \
                     : ((obj)->otyp == BOULDER) \
                        ? cmap_to_glyph(S_extra_boulder) \
                            : ((obj)->oartifact > 0) \
                                ? ((int)(obj)->oartifact - 1 + GLYPH_ARTIFACT_OFF) \
						            :  ((int)(obj)->otyp + GLYPH_OBJ_OFF))

#define obj_to_missile_glyph(obj, dir_index, rng)                                          \
    ((dir_index) < 0 || (dir_index) >= NUM_MISSILE_DIRS ? NO_GLYPH \
     : (((obj)->otyp == STATUE)                                            \
         ? (is_female_corpse_or_statue(obj) ? female_statue_to_glyph(obj, rng) : statue_to_glyph(obj, rng) )                                   \
         : Hallucination                                                \
               ? random_obj_to_missile_glyph(dir_index, rng)                               \
               : ((obj)->otyp == CORPSE)                                \
                     ?  (is_female_corpse_or_statue(obj) ? ((is_corpse_or_statue_facing_right(obj) ? -1 : 1) * ((int) (obj)->corpsenm + GLYPH_FEMALE_BODY_OFF))  : ((is_corpse_or_statue_facing_right(obj) ? -1 : 1) * ((int) (obj)->corpsenm + GLYPH_BODY_OFF)) )         \
                     : ((obj)->otyp == BOULDER) \
                        ? cmap_to_glyph(S_extra_boulder) \
                            : ((obj)->oartifact > 0) \
                                ? ((int)((obj)->oartifact - 1) * NUM_MISSILE_DIRS + (dir_index) + GLYPH_ARTIFACT_MISSILE_OFF) \
						            :  ((int)(obj)->otyp * NUM_MISSILE_DIRS + (dir_index) + GLYPH_OBJ_MISSILE_OFF)) )


#define explosion_to_glyph(expltype, idx) \
    ((((expltype) * MAX_EXPLOSION_CHARS) + ((idx) - S_explode1)) + GLYPH_EXPLODE_OFF)

#define trap_to_glyph(trap, rng)                                \
    ((trap)->tsubtyp ? (Hallucination ? cmap_to_glyph(trap_to_defsym(what_trap((trap)->ttyp, rng))) : cmap_variation_to_glyph((trap)->tsubtyp - 1 + defsyms[trap_to_defsym((trap)->ttyp)].variation_offset)) \
     : cmap_to_glyph(trap_to_defsym(what_trap((trap)->ttyp, rng))))

/* Not affected by hallucination.  Gives a generic body for CORPSE */
/* MRKR: ...and the generic statue */
#define objnum_to_glyph(onum) ((int) (onum) + GLYPH_OBJ_OFF)
#define monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_MON_OFF)
#define attacking_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_ATTACK_OFF)
#define throwing_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_THROW_OFF)
#define firing_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FIRE_OFF)
#define nodir_casting_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_CAST_NODIR_OFF)
#define dir_casting_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_CAST_DIR_OFF)
#define special_attacking_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_SPECIAL_ATTACK_OFF)
#define kicking_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_KICK_OFF)
#define passive_defense_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_PASSIVE_DEFENSE_OFF)
#define defending_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_DEFEND_OFF)
#define damage_receiving_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_RECEIVE_DAMAGE_OFF)
#define item_using_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_ITEM_USE_OFF)
#define door_using_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_DOOR_USE_OFF)
#define dying_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_DEATH_OFF)
#define passive_defense_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_PASSIVE_DEFENSE_OFF)
#define defending_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_DEFEND_OFF)
#define damage_receiving_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_RECEIVE_DAMAGE_OFF)

#define female_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_MON_OFF)
#define female_attacking_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_ATTACK_OFF)
#define female_throwing_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_THROW_OFF)
#define female_firing_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_FIRE_OFF)
#define female_nodir_casting_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_CAST_NODIR_OFF)
#define female_dir_casting_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_CAST_DIR_OFF)
#define female_special_attacking_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_SPECIAL_ATTACK_OFF)
#define female_kicking_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_KICK_OFF)
#define female_passive_defense_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_PASSIVE_DEFENSE_OFF)
#define female_defending_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_DEFEND_OFF)
#define female_damage_receiving_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_RECEIVE_DAMAGE_OFF)
#define female_item_using_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_ITEM_USE_OFF)
#define female_door_using_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_DOOR_USE_OFF)
#define female_dying_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_DEATH_OFF)
#define female_passive_defense_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_PASSIVE_DEFENSE_OFF)
#define female_defending_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_DEFEND_OFF)
#define female_damage_receiving_monnum_to_glyph(mnum) ((int) (mnum) + GLYPH_FEMALE_RECEIVE_DAMAGE_OFF)

#define any_monnum_to_glyph(isfemale, mnum) (isfemale ? female_monnum_to_glyph(mnum) : monnum_to_glyph(mnum) )

/* The hero's glyph when seen as a monster. OBSOLETE --JG
 */
#define hero_glyph                                                    \
    flags.female ? \
        female_monnum_to_glyph((Upolyd || !flags.showrace)                       \
                        ? u.umonnum                                   \
                        : urace.monsternum) : \
        monnum_to_glyph((Upolyd || !flags.showrace)                       \
                        ? u.umonnum                                   \
                        : urace.monsternum)

/*
 * Return true if the given glyph is what we want.  Note that bodies are
 * considered objects.
 */
#define glyph_is_normal_monster(glyph) \
    ((abs(glyph)) >= GLYPH_MON_OFF && (abs(glyph)) < (GLYPH_MON_OFF + NUM_MONSTERS))
#define glyph_is_attacking_monster(glyph) \
    ((abs(glyph)) >= GLYPH_ATTACK_OFF && (abs(glyph)) < (GLYPH_ATTACK_OFF + NUM_MONSTERS))
#define glyph_is_throwing_monster(glyph) \
    ((abs(glyph)) >= GLYPH_THROW_OFF && (abs(glyph)) < (GLYPH_THROW_OFF + NUM_MONSTERS))
#define glyph_is_firing_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FIRE_OFF && (abs(glyph)) < (GLYPH_FIRE_OFF + NUM_MONSTERS))
#define glyph_is_nodir_casting_monster(glyph) \
    ((abs(glyph)) >= GLYPH_CAST_NODIR_OFF && (abs(glyph)) < (GLYPH_CAST_NODIR_OFF + NUM_MONSTERS))
#define glyph_is_dir_casting_monster(glyph) \
    ((abs(glyph)) >= GLYPH_CAST_DIR_OFF && (abs(glyph)) < (GLYPH_CAST_DIR_OFF + NUM_MONSTERS))
#define glyph_is_special_attacking_monster(glyph) \
    ((abs(glyph)) >= GLYPH_SPECIAL_ATTACK_OFF && (abs(glyph)) < (GLYPH_SPECIAL_ATTACK_OFF + NUM_MONSTERS))
#define glyph_is_kicking_monster(glyph) \
    ((abs(glyph)) >= GLYPH_KICK_OFF && (abs(glyph)) < (GLYPH_KICK_OFF + NUM_MONSTERS))
#define glyph_is_item_using_monster(glyph) \
    ((abs(glyph)) >= GLYPH_ITEM_USE_OFF && (abs(glyph)) < (GLYPH_ITEM_USE_OFF + NUM_MONSTERS))
#define glyph_is_door_using_monster(glyph) \
    ((abs(glyph)) >= GLYPH_DOOR_USE_OFF && (abs(glyph)) < (GLYPH_DOOR_USE_OFF + NUM_MONSTERS))
#define glyph_is_dying_monster(glyph) \
    ((abs(glyph)) >= GLYPH_DEATH_OFF && (abs(glyph)) < (GLYPH_DEATH_OFF + NUM_MONSTERS))
#define glyph_is_passive_defense_monster(glyph) \
    ((abs(glyph)) >= GLYPH_PASSIVE_DEFENSE_OFF && (abs(glyph)) < (GLYPH_PASSIVE_DEFENSE_OFF + NUM_MONSTERS))
#define glyph_is_defending_monster(glyph) \
    ((abs(glyph)) >= GLYPH_DEFEND_OFF && (abs(glyph)) < (GLYPH_DEFEND_OFF + NUM_MONSTERS))
#define glyph_is_damage_receiving_monster(glyph) \
    ((abs(glyph)) >= GLYPH_RECEIVE_DAMAGE_OFF && (abs(glyph)) < (GLYPH_RECEIVE_DAMAGE_OFF + NUM_MONSTERS))

#define glyph_is_statue(glyph) \
    ((abs(glyph)) >= GLYPH_STATUE_OFF && (abs(glyph)) < (GLYPH_STATUE_OFF + NUM_MONSTERS))
#define glyph_is_body(glyph) \
    ((abs(glyph)) >= GLYPH_BODY_OFF && (abs(glyph)) < (GLYPH_BODY_OFF + NUM_MONSTERS))

#define glyph_is_female_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_MON_OFF && (abs(glyph)) < (GLYPH_FEMALE_MON_OFF + NUM_MONSTERS))
#define glyph_is_female_attacking_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_ATTACK_OFF && (abs(glyph)) < (GLYPH_FEMALE_ATTACK_OFF + NUM_MONSTERS))
#define glyph_is_female_throwing_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_THROW_OFF && (abs(glyph)) < (GLYPH_FEMALE_THROW_OFF + NUM_MONSTERS))
#define glyph_is_female_firing_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_FIRE_OFF && (abs(glyph)) < (GLYPH_FEMALE_FIRE_OFF + NUM_MONSTERS))
#define glyph_is_female_nodir_casting_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_CAST_NODIR_OFF && (abs(glyph)) < (GLYPH_FEMALE_CAST_NODIR_OFF + NUM_MONSTERS))
#define glyph_is_female_dir_casting_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_CAST_DIR_OFF && (abs(glyph)) < (GLYPH_FEMALE_CAST_DIR_OFF + NUM_MONSTERS))
#define glyph_is_female_special_attacking_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_SPECIAL_ATTACK_OFF && (abs(glyph)) < (GLYPH_FEMALE_SPECIAL_ATTACK_OFF + NUM_MONSTERS))
#define glyph_is_female_kicking_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_KICK_OFF && (abs(glyph)) < (GLYPH_FEMALE_KICK_OFF + NUM_MONSTERS))
#define glyph_is_female_item_using_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_ITEM_USE_OFF && (abs(glyph)) < (GLYPH_FEMALE_ITEM_USE_OFF + NUM_MONSTERS))
#define glyph_is_female_door_using_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_DOOR_USE_OFF && (abs(glyph)) < (GLYPH_FEMALE_DOOR_USE_OFF + NUM_MONSTERS))
#define glyph_is_female_dying_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_DEATH_OFF && (abs(glyph)) < (GLYPH_FEMALE_DEATH_OFF + NUM_MONSTERS))
#define glyph_is_female_passive_defense_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_PASSIVE_DEFENSE_OFF && (abs(glyph)) < (GLYPH_FEMALE_PASSIVE_DEFENSE_OFF + NUM_MONSTERS))
#define glyph_is_female_defending_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_DEFEND_OFF && (abs(glyph)) < (GLYPH_FEMALE_DEFEND_OFF + NUM_MONSTERS))
#define glyph_is_female_damage_receiving_monster(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_RECEIVE_DAMAGE_OFF && (abs(glyph)) < (GLYPH_FEMALE_RECEIVE_DAMAGE_OFF + NUM_MONSTERS))

#define glyph_is_female_statue(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_STATUE_OFF && (abs(glyph)) < (GLYPH_FEMALE_STATUE_OFF + NUM_MONSTERS))
#define glyph_is_female_body(glyph) \
    ((abs(glyph)) >= GLYPH_FEMALE_BODY_OFF && (abs(glyph)) < (GLYPH_FEMALE_BODY_OFF + NUM_MONSTERS))

#define glyph_is_any_statue(glyph) \
    (glyph_is_statue(glyph) || glyph_is_female_statue(glyph))
#define glyph_is_any_body(glyph) \
    (glyph_is_body(glyph) || glyph_is_female_body(glyph))

#define glyph_is_invisible(glyph) ((abs(glyph)) == GLYPH_INVISIBLE)

#define glyph_is_normal_object(glyph) \
    ((abs(glyph)) >= GLYPH_OBJ_OFF && (abs(glyph)) < (GLYPH_OBJ_OFF + NUM_OBJECTS))
#define glyph_is_object_missile(glyph) \
    ((abs(glyph)) >= GLYPH_OBJ_MISSILE_OFF && (abs(glyph)) < (GLYPH_OBJ_MISSILE_OFF + NUM_OBJECTS * NUM_MISSILE_DIRS))

#define glyph_is_artifact(glyph) \
    ((abs(glyph)) >= GLYPH_ARTIFACT_OFF && (abs(glyph)) < (GLYPH_ARTIFACT_OFF + NUM_ARTIFACTS))
#define glyph_to_artifact(glyph) \
    (glyph_is_artifact(glyph) ? ((abs(glyph)) + 1 - GLYPH_ARTIFACT_OFF) : NO_GLYPH)
#define glyph_is_artifact_missile(glyph) \
    ((abs(glyph)) >= GLYPH_ARTIFACT_MISSILE_OFF && (abs(glyph)) < (GLYPH_ARTIFACT_MISSILE_OFF + NUM_ARTIFACTS * NUM_MISSILE_DIRS))

#define glyph_is_cmap(glyph) \
    ((abs(glyph)) >= GLYPH_CMAP_OFF && (abs(glyph)) < (GLYPH_CMAP_OFF + CMAP_TYPE_CHAR_NUM * CMAP_TYPE_MAX))
#define glyph_is_broken_cmap(glyph) \
    ((abs(glyph)) >= GLYPH_BROKEN_CMAP_OFF && (abs(glyph)) < (GLYPH_BROKEN_CMAP_OFF + CMAP_TYPE_CHAR_NUM * CMAP_TYPE_MAX))
#define glyph_is_cmap_variation(glyph) \
    ((abs(glyph)) >= GLYPH_CMAP_VARIATION_OFF && (abs(glyph)) < (GLYPH_CMAP_VARIATION_OFF + MAX_VARIATIONS * CMAP_TYPE_MAX))
#define glyph_is_broken_cmap_variation(glyph) \
    ((abs(glyph)) >= GLYPH_BROKEN_CMAP_VARIATION_OFF && (abs(glyph)) < (GLYPH_BROKEN_CMAP_VARIATION_OFF + MAX_VARIATIONS * CMAP_TYPE_MAX))
#define glyph_is_cmap_or_cmap_variation(glyph) \
    (glyph_is_cmap(glyph) || glyph_is_broken_cmap(glyph) || glyph_is_cmap_variation(glyph) || glyph_is_broken_cmap_variation(glyph))

#define glyph_is_explosion(glyph)   \
    ((abs(glyph)) >= GLYPH_EXPLODE_OFF \
     && (abs(glyph)) < (GLYPH_EXPLODE_OFF + (MAX_EXPLOSION_CHARS * EXPL_MAX)))
#define glyph_is_zap(glyph)   \
    ((abs(glyph)) >= GLYPH_ZAP_OFF \
     && (abs(glyph)) < (GLYPH_ZAP_OFF + (NUM_ZAP * MAX_ZAP_CHARS)))
#define glyph_is_swallow(glyph)   \
    ((abs(glyph)) >= GLYPH_SWALLOW_OFF \
     && (abs(glyph)) < (GLYPH_SWALLOW_OFF + (NUM_MONSTERS * MAX_SWALLOW_CHARS)))
#define glyph_is_warning(glyph)   \
    ((abs(glyph)) >= GLYPH_WARNING_OFF \
     && (abs(glyph)) < (GLYPH_WARNING_OFF + WARNCOUNT))

#define glyph_is_cmap_boulder(glyph) \
    ((glyph_is_cmap(glyph) && (((abs(glyph)) - GLYPH_CMAP_OFF) % CMAP_TYPE_CHAR_NUM) == S_extra_boulder) \
     || (glyph_is_cmap_variation(glyph) && defsym_variations[max(0, (((abs(glyph)) - GLYPH_CMAP_VARIATION_OFF) % CMAP_TYPE_CHAR_NUM))].base_screen_symbol == S_extra_boulder)  \
    )

/* Note: excludes missiles */
#define glyph_is_object(glyph)                               \
    (glyph_is_normal_object(glyph)\
     || glyph_is_statue(glyph)  || glyph_is_female_statue(glyph)\
     || glyph_is_body(glyph) || glyph_is_female_body(glyph) \
     || glyph_is_cmap_boulder(glyph) \
     || glyph_is_artifact(glyph))

#define glyph_is_missile(glyph) \
     (glyph_is_object_missile(glyph) || glyph_is_artifact_missile(glyph))

#define glyph_is_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_OFF && (abs(glyph)) < (GLYPH_PLAYER_OFF + NUM_PLAYER_CHARACTERS))

#define glyph_to_player(glyph) \
    (glyph_is_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_OFF) : NO_GLYPH)

#define glyph_is_attacking_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_ATTACK_OFF && (abs(glyph)) < (GLYPH_PLAYER_ATTACK_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_attacking_player(glyph) \
    (glyph_is_attacking_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_ATTACK_OFF) : NO_GLYPH)

#define glyph_is_throwing_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_THROW_OFF && (abs(glyph)) < (GLYPH_PLAYER_THROW_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_throwing_player(glyph) \
    (glyph_is_throwing_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_THROW_OFF) : NO_GLYPH)

#define glyph_is_firing_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_FIRE_OFF && (abs(glyph)) < (GLYPH_PLAYER_FIRE_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_firing_player(glyph) \
    (glyph_is_firing_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_FIRE_OFF) : NO_GLYPH)

#define glyph_is_nodir_casting_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_CAST_NODIR_OFF && (abs(glyph)) < (GLYPH_PLAYER_CAST_NODIR_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_nodir_casting_player(glyph) \
    (glyph_is_nodir_casting_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_CAST_NODIR_OFF) : NO_GLYPH)

#define glyph_is_dir_casting_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_CAST_DIR_OFF && (abs(glyph)) < (GLYPH_PLAYER_CAST_DIR_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_dir_casting_player(glyph) \
    (glyph_is_dir_casting_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_CAST_DIR_OFF) : NO_GLYPH)

#define glyph_is_special_attacking_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_SPECIAL_ATTACK_OFF && (abs(glyph)) < (GLYPH_PLAYER_SPECIAL_ATTACK_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_special_attacking_player(glyph) \
    (glyph_is_special_attacking_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_SPECIAL_ATTACK_OFF) : NO_GLYPH)

#define glyph_is_kicking_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_KICK_OFF && (abs(glyph)) < (GLYPH_PLAYER_KICK_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_kicking_player(glyph) \
    (glyph_is_kicking_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_KICK_OFF) : NO_GLYPH)

#define glyph_is_item_using_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_ITEM_USE_OFF && (abs(glyph)) < (GLYPH_PLAYER_ITEM_USE_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_item_using_player(glyph) \
    (glyph_is_item_using_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_ITEM_USE_OFF) : NO_GLYPH)

#define glyph_is_door_using_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_DOOR_USE_OFF && (abs(glyph)) < (GLYPH_PLAYER_DOOR_USE_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_door_using_player(glyph) \
    (glyph_is_door_using_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_DOOR_USE_OFF) : NO_GLYPH)

#define glyph_is_dying_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_DEATH_OFF && (abs(glyph)) < (GLYPH_PLAYER_DEATH_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_dying_player(glyph) \
    (glyph_is_dying_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_DEATH_OFF) : NO_GLYPH)

#define glyph_is_passive_defense_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_PASSIVE_DEFENSE_OFF && (abs(glyph)) < (GLYPH_PLAYER_PASSIVE_DEFENSE_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_passive_defense_player(glyph) \
    (glyph_is_passive_defense_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_PASSIVE_DEFENSE_OFF) : NO_GLYPH)

#define glyph_is_defending_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_DEFEND_OFF && (abs(glyph)) < (GLYPH_PLAYER_DEFEND_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_defending_player(glyph) \
    (glyph_is_defending_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_DEFEND_OFF) : NO_GLYPH)

#define glyph_is_damage_receiving_player(glyph) \
    ((abs(glyph)) >= GLYPH_PLAYER_RECEIVE_DAMAGE_OFF && (abs(glyph)) < (GLYPH_PLAYER_RECEIVE_DAMAGE_OFF + NUM_PLAYER_CHARACTERS))
#define glyph_to_damage_receiving_player(glyph) \
    (glyph_is_damage_receiving_player(glyph) ? ((abs(glyph)) - GLYPH_PLAYER_RECEIVE_DAMAGE_OFF) : NO_GLYPH)

#define glyph_is_monster(glyph)                            \
    ((abs(glyph) >= GLYPH_MON_OFF && abs(glyph) < GLYPH_INVIS_OFF)     \
     || (abs(glyph) >= GLYPH_PLAYER_OFF && abs(glyph) < GLYPH_CURSOR_OFF))

 /*
  * Change the given glyph into it's given type.  Note:
  *      1) Pets, detected, and ridden monsters are animals and are converted
  *         to the proper monster number.
  *      2) Bodies are all mapped into the generic CORPSE object
  *      3) If handed a glyph out of range for the type, these functions
  *         will return NO_GLYPH (see exception below)
  *      4) glyph_to_swallow() does not return a showsyms[] index, but an
  *         offset from the first swallow symbol.  If handed something
  *         out of range, it will return zero (for lack of anything better
  *         to return).
  */

#define glyph_to_mon(glyph) \
    (glyph_is_normal_monster(glyph)                             \
         ? ((abs(glyph)) - GLYPH_MON_OFF)                            \
                     : glyph_is_attacking_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_ATTACK_OFF)       \
                     : glyph_is_throwing_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_THROW_OFF)       \
                     : glyph_is_firing_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FIRE_OFF)       \
                     : glyph_is_nodir_casting_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_CAST_NODIR_OFF)       \
                     : glyph_is_dir_casting_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_CAST_DIR_OFF)       \
                     : glyph_is_special_attacking_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_SPECIAL_ATTACK_OFF)       \
                     : glyph_is_kicking_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_KICK_OFF)       \
                     : glyph_is_item_using_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_ITEM_USE_OFF)       \
                     : glyph_is_door_using_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_DOOR_USE_OFF)       \
                     : glyph_is_dying_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_DEATH_OFF)       \
                     : glyph_is_passive_defense_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_PASSIVE_DEFENSE_OFF)       \
                     : glyph_is_defending_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_DEFEND_OFF)       \
                     : glyph_is_damage_receiving_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_RECEIVE_DAMAGE_OFF)       \
                     : glyph_is_female_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_MON_OFF)   \
                     : glyph_is_female_attacking_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_ATTACK_OFF)       \
                     : glyph_is_female_throwing_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_THROW_OFF)       \
                     : glyph_is_female_firing_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_FIRE_OFF)       \
                     : glyph_is_female_nodir_casting_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_CAST_NODIR_OFF)       \
                     : glyph_is_female_dir_casting_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_CAST_DIR_OFF)       \
                     : glyph_is_female_special_attacking_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_SPECIAL_ATTACK_OFF)       \
                     : glyph_is_female_kicking_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_KICK_OFF)       \
                     : glyph_is_female_item_using_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_ITEM_USE_OFF)       \
                     : glyph_is_female_door_using_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_DOOR_USE_OFF)       \
                     : glyph_is_female_dying_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_DEATH_OFF)       \
                     : glyph_is_female_passive_defense_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_PASSIVE_DEFENSE_OFF)       \
                     : glyph_is_female_defending_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_DEFEND_OFF)       \
                     : glyph_is_female_damage_receiving_monster(glyph)           \
                           ? ((abs(glyph)) - GLYPH_FEMALE_RECEIVE_DAMAGE_OFF)       \
                           : glyph_is_statue(glyph)             \
                                 ? ((abs(glyph)) - GLYPH_STATUE_OFF) \
                           : glyph_is_female_statue(glyph)             \
                                 ? ((abs(glyph)) - GLYPH_FEMALE_STATUE_OFF) \
                                   : glyph_is_player(glyph)     \
                                         ? (glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_attacking_player(glyph)     \
                                         ? (attack_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_throwing_player(glyph)     \
                                         ? (throw_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_firing_player(glyph)     \
                                         ? (fire_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_nodir_casting_player(glyph)     \
                                         ? (nodir_cast_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_dir_casting_player(glyph)     \
                                         ? (dir_cast_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_special_attacking_player(glyph)     \
                                         ? (special_attack_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_kicking_player(glyph)     \
                                         ? (kick_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_item_using_player(glyph)     \
                                         ? (item_use_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_door_using_player(glyph)     \
                                         ? (door_use_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_dying_player(glyph)     \
                                         ? (death_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_passive_defense_player(glyph)     \
                                         ? (passive_defense_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_defending_player(glyph)     \
                                         ? (defend_glyph_to_player_mon(abs(glyph)))                    \
                                   : glyph_is_damage_receiving_player(glyph)     \
                                         ? (damage_receive_glyph_to_player_mon(abs(glyph)))                    \
                                             : NO_GLYPH)


#define glyph_to_cmap(glyph) \
    (glyph_is_cmap(glyph) ? (abs(glyph) < GLYPH_CMAP_OFF + get_current_cmap_type_index() * CMAP_TYPE_CHAR_NUM ? (abs(glyph)) - GLYPH_CMAP_OFF : (abs(glyph)) - get_current_cmap_type_index() * CMAP_TYPE_CHAR_NUM - GLYPH_CMAP_OFF) : S_unexplored)

#define glyph_to_broken_cmap(glyph) \
    (glyph_is_broken_cmap(glyph) ? (abs(glyph) < GLYPH_BROKEN_CMAP_OFF + get_current_cmap_type_index() * CMAP_TYPE_CHAR_NUM ? (abs(glyph)) - GLYPH_BROKEN_CMAP_OFF : (abs(glyph)) - get_current_cmap_type_index() * CMAP_TYPE_CHAR_NUM - GLYPH_BROKEN_CMAP_OFF) : S_unexplored)

#define glyph_to_cmap_variation(glyph) \
    (glyph_is_cmap_variation(glyph) ? (abs(glyph) < GLYPH_CMAP_VARIATION_OFF + get_current_cmap_type_index() * MAX_VARIATIONS ? (abs(glyph)) - GLYPH_CMAP_VARIATION_OFF : (abs(glyph)) - get_current_cmap_type_index() * MAX_VARIATIONS - GLYPH_CMAP_VARIATION_OFF) : 0)

#define glyph_to_broken_cmap_variation(glyph) \
    (glyph_is_broken_cmap_variation(glyph) ? (abs(glyph) < GLYPH_BROKEN_CMAP_VARIATION_OFF + get_current_cmap_type_index() * MAX_VARIATIONS ? (abs(glyph)) - GLYPH_BROKEN_CMAP_VARIATION_OFF : (abs(glyph)) - get_current_cmap_type_index() * MAX_VARIATIONS - GLYPH_BROKEN_CMAP_VARIATION_OFF) : 0)

#define generic_glyph_to_cmap(glyph) \
    (glyph_is_cmap(glyph) ? glyph_to_cmap(glyph) \
      : glyph_is_broken_cmap(glyph) ? glyph_to_broken_cmap(glyph) \
      : glyph_is_cmap_variation(glyph) ? (defsym_variations[(abs(glyph) < GLYPH_CMAP_VARIATION_OFF + get_current_cmap_type_index() * MAX_VARIATIONS ? (abs(glyph)) - GLYPH_CMAP_VARIATION_OFF : (abs(glyph)) - get_current_cmap_type_index() * MAX_VARIATIONS - GLYPH_CMAP_VARIATION_OFF)].base_screen_symbol) \
      : glyph_is_broken_cmap_variation(glyph) ? (defsym_variations[(abs(glyph) < GLYPH_BROKEN_CMAP_VARIATION_OFF + get_current_cmap_type_index() * MAX_VARIATIONS ? (abs(glyph)) - GLYPH_BROKEN_CMAP_VARIATION_OFF : (abs(glyph)) - get_current_cmap_type_index() * MAX_VARIATIONS - GLYPH_BROKEN_CMAP_VARIATION_OFF)].base_screen_symbol) \
      : S_unexplored)

#define glyph_is_trap(glyph)                         \
    (glyph_is_cmap_or_cmap_variation(glyph) && generic_glyph_to_cmap(glyph) >= trap_to_defsym(1) \
     && generic_glyph_to_cmap(glyph) < trap_to_defsym(1) + TRAPNUM)

#define glyph_to_trap(glyph) \
    (glyph_is_trap(glyph) ? (glyph_is_cmap(glyph) ? defsym_to_trap(generic_glyph_to_cmap(glyph)) : glyph_is_broken_cmap(glyph) ? defsym_to_trap(glyph_to_broken_cmap(glyph)) : glyph_is_cmap_variation(glyph) ? defsym_to_trap(defsym_variations[glyph_to_cmap_variation(glyph)].base_screen_symbol) : glyph_is_broken_cmap_variation(glyph) ? defsym_to_trap(defsym_variations[glyph_to_broken_cmap_variation(glyph)].base_screen_symbol) : NO_TRAP) : NO_TRAP)
//      ((int) defsym_to_trap((abs(glyph)) - get_current_cmap_type_index() * CMAP_TYPE_CHAR_NUM - GLYPH_CMAP_OFF)) \
//                          : NO_GLYPH)

#define glyph_to_swallow(glyph) \
    (glyph_is_swallow(glyph) ? (((abs(glyph)) - GLYPH_SWALLOW_OFF) % MAX_SWALLOW_CHARS) : 0)
#define glyph_to_warning(glyph) \
    (glyph_is_warning(glyph) ? ((abs(glyph)) - GLYPH_WARNING_OFF) : NO_GLYPH);

#define glyph_to_obj(glyph) \
    (glyph_is_body(glyph) || glyph_is_female_body(glyph)  \
         ? CORPSE                                \
         : glyph_is_statue(glyph) || glyph_is_female_statue(glyph) \
               ? STATUE                          \
         : glyph_is_cmap_boulder(glyph) \
               ? BOULDER                          \
			 : glyph_is_artifact(glyph)                \
				   ? artifact_to_obj(glyph_to_artifact(glyph))   \
				   : glyph_is_normal_object(glyph)   \
						 ? ((abs(glyph)) - GLYPH_OBJ_OFF) \
						 : NO_GLYPH)


/*
 * display_self()
 *
 * Display the hero.  It is assumed that all checks necessary to determine
 * _if_ the hero can be seen have already been done.
 */
#define maybe_display_usteed(otherwise_self)                            \
    ((u.usteed && mon_visible(u.usteed)) == TRUE                                \
     ? (u.usteed->female == TRUE ? female_mon_to_glyph(u.usteed, rn2_on_display_rng) : mon_to_glyph(u.usteed, rn2_on_display_rng))  \
     : (otherwise_self))

#define display_self() display_self_with_extra_info(0UL, 0)


#endif /* DISPLAY_H */
