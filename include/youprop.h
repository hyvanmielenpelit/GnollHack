/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    youprop.h    $NHDT-Date: 1433291407 2015/06/03 00:30:07 $  $NHDT-Branch: master $:$NHDT-Revision: 1.23 $ */
/* Copyright (c) 1989 Mike Threepoint                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef YOUPROP_H
#define YOUPROP_H

#include "prop.h"
#include "permonst.h"
#include "mondata.h"
#include "pm.h"

/* KMH, intrinsics patch.
 * Reorganized and rewritten for >32-bit properties.
 * HXxx refers to intrinsic bitfields while in human form.
 * EXxx refers to extrinsic bitfields from worn objects.
 * BXxx refers to the cause of the property being blocked.
 * Xxx refers to any source, including polymorph forms.
 * [Post-3.4.3: HXxx now includes a FROM_FORM bit to handle
 * intrinsic conferred by being polymorphed.]
 */

#define maybe_polyd(if_so, if_not) (Upolyd ? (if_so) : (if_not))

/* Blocking abiliities */
#define HBlocks_Invisibility u.uprops[BLOCKS_INVISIBILITY].intrinsic
#define EBlocks_Invisibility u.uprops[BLOCKS_INVISIBILITY].extrinsic
#define Blocks_Invisibility (HBlocks_Invisibility || EBlocks_Invisibility)

#define HBlocks_Blindness u.uprops[BLOCKS_BLINDNESS].intrinsic
#define EBlocks_Blindness u.uprops[BLOCKS_BLINDNESS].extrinsic
#define Blocks_Blindness (HBlocks_Blindness|| EBlocks_Blindness)

#define HBlocks_Clairvoyance u.uprops[BLOCKS_CLAIRVOYANCE].intrinsic
#define EBlocks_Clairvoyance u.uprops[BLOCKS_CLAIRVOYANCE].extrinsic
#define Blocks_Clairvoyance (HBlocks_Clairvoyance|| EBlocks_Clairvoyance)

#define HBlocks_Levitation u.uprops[BLOCKS_LEVITATION].intrinsic
#define EBlocks_Levitation u.uprops[BLOCKS_LEVITATION].extrinsic
#define Blocks_Levitation (HBlocks_Levitation || EBlocks_Levitation)

#define HBlocks_Flying u.uprops[BLOCKS_FLYING].intrinsic
#define EBlocks_Flying u.uprops[BLOCKS_FLYING].extrinsic
#define Blocks_Flying (HBlocks_Flying || EBlocks_Flying)

/*** Resistances to troubles ***/
/* With intrinsics and extrinsics */
#define HFire_immunity u.uprops[FIRE_IMMUNITY].intrinsic
#define EFire_immunity u.uprops[FIRE_IMMUNITY].extrinsic
#define Fire_immunity (HFire_immunity || EFire_immunity)

#define HCold_immunity u.uprops[COLD_IMMUNITY].intrinsic
#define ECold_immunity u.uprops[COLD_IMMUNITY].extrinsic
#define Cold_immunity (HCold_immunity || ECold_immunity)

#define HShock_immunity u.uprops[SHOCK_IMMUNITY].intrinsic
#define EShock_immunity u.uprops[SHOCK_IMMUNITY].extrinsic
#define Shock_immunity (HShock_immunity || EShock_immunity)

#define HFire_resistance u.uprops[FIRE_RESISTANCE].intrinsic
#define EFire_resistance u.uprops[FIRE_RESISTANCE].extrinsic
#define Fire_resistance (HFire_resistance || EFire_resistance)

#define HCold_resistance u.uprops[COLD_RESISTANCE].intrinsic
#define ECold_resistance u.uprops[COLD_RESISTANCE].extrinsic
#define Cold_resistance (HCold_resistance || ECold_resistance)

#define HShock_resistance u.uprops[SHOCK_RESISTANCE].intrinsic
#define EShock_resistance u.uprops[SHOCK_RESISTANCE].extrinsic
#define Shock_resistance (HShock_resistance || EShock_resistance)

#define HSleep_resistance u.uprops[SLEEP_RESISTANCE].intrinsic
#define ESleep_resistance u.uprops[SLEEP_RESISTANCE].extrinsic
#define Sleep_resistance (HSleep_resistance || ESleep_resistance)

#define HDisint_resistance u.uprops[DISINTEGRATION_RESISTANCE].intrinsic
#define EDisint_resistance u.uprops[DISINTEGRATION_RESISTANCE].extrinsic
#define Disint_resistance (HDisint_resistance || EDisint_resistance)

#define HDeath_resistance u.uprops[DEATH_RESISTANCE].intrinsic
#define EDeath_resistance u.uprops[DEATH_RESISTANCE].extrinsic
#define Death_resistance (HDeath_resistance || EDeath_resistance)

#define HLycanthropy_resistance u.uprops[LYCANTHROPY_RESISTANCE].intrinsic
#define ELycanthropy_resistance u.uprops[LYCANTHROPY_RESISTANCE].extrinsic
#define Lycanthropy_resistance (HLycanthropy_resistance || ELycanthropy_resistance)

#define HCurse_resistance u.uprops[CURSE_RESISTANCE].intrinsic
#define ECurse_resistance u.uprops[CURSE_RESISTANCE].extrinsic
#define Curse_resistance (HCurse_resistance || ECurse_resistance)

#define HPoison_resistance u.uprops[POISON_RESISTANCE].intrinsic
#define EPoison_resistance u.uprops[POISON_RESISTANCE].extrinsic
#define Poison_resistance (HPoison_resistance || EPoison_resistance)

#define HDrain_resistance u.uprops[DRAIN_RESISTANCE].intrinsic
#define EDrain_resistance u.uprops[DRAIN_RESISTANCE].extrinsic
#define Drain_resistance (HDrain_resistance || EDrain_resistance)

#define HCharm_resistance u.uprops[CHARM_RESISTANCE].intrinsic
#define ECharm_resistance u.uprops[CHARM_RESISTANCE].extrinsic
#define Charm_resistance (HCharm_resistance || ECharm_resistance)

#define HFear_resistance u.uprops[FEAR_RESISTANCE].intrinsic
#define EFear_resistance u.uprops[FEAR_RESISTANCE].extrinsic
#define Fear_resistance (HFear_resistance || ECharm_resistance)

#define HMind_shielding u.uprops[MIND_SHIELDING].intrinsic
#define EMind_shielding u.uprops[MIND_SHIELDING].extrinsic
#define Mind_shielding (HMind_shielding || EMind_shielding)

#define HImproved_fire_resistance u.uprops[IMPROVED_FIRE_RESISTANCE].intrinsic
#define EImproved_fire_resistance u.uprops[IMPROVED_FIRE_RESISTANCE].extrinsic
#define Improved_fire_resistance (HImproved_fire_resistance || EImproved_fire_resistance)

#define HImproved_cold_resistance u.uprops[IMPROVED_COLD_RESISTANCE].intrinsic
#define EImproved_cold_resistance u.uprops[IMPROVED_COLD_RESISTANCE].extrinsic
#define Improved_cold_resistance (HImproved_cold_resistance || EImproved_cold_resistance)

#define HImproved_shock_resistance u.uprops[IMPROVED_SHOCK_RESISTANCE].intrinsic
#define EImproved_shock_resistance u.uprops[IMPROVED_SHOCK_RESISTANCE].extrinsic
#define Improved_shock_resistance (HImproved_shock_resistance || EImproved_shock_resistance)



/* More resistances */
#define HAntimagic u.uprops[ANTIMAGIC].intrinsic
#define EAntimagic u.uprops[ANTIMAGIC].extrinsic
#define Antimagic (HAntimagic || EAntimagic)

#define Antimagic_or_resistance check_magic_resistance_and_inflict_damage(&youmonst, (struct obj*)0, (struct monst*)0, FALSE, 0, 0, NOTELL)

#define HMagic_missile_immunity u.uprops[MAGIC_MISSILE_IMMUNITY].intrinsic
#define EMagic_missile_immunity u.uprops[MAGIC_MISSILE_IMMUNITY].extrinsic
#define Magic_missile_immunity (HMagic_missile_immunity || EMagic_missile_immunity)

#define HMagic_missile_resistance u.uprops[MAGIC_MISSILE_RESISTANCE].intrinsic
#define EMagic_missile_resistance u.uprops[MAGIC_MISSILE_RESISTANCE].extrinsic
#define Magic_missile_resistance (HMagic_missile_resistance || EMagic_missile_resistance)

#define HImproved_magic_missile_resistance u.uprops[IMPROVED_MAGIC_MISSILE_RESISTANCE].intrinsic
#define EImproved_magic_missile_resistance u.uprops[IMPROVED_MAGIC_MISSILE_RESISTANCE].extrinsic
#define Improved_magic_missile_resistance (HImproved_magic_missile_resistance || EImproved_magic_missile_resistance)


#define HAcid_immunity u.uprops[ACID_IMMUNITY].intrinsic
#define EAcid_immunity u.uprops[ACID_IMMUNITY].extrinsic
#define Acid_immunity (HAcid_immunity || EAcid_immunity)

#define HAcid_resistance u.uprops[ACID_RESISTANCE].intrinsic
#define EAcid_resistance u.uprops[ACID_RESISTANCE].extrinsic
#define Acid_resistance (HAcid_resistance || EAcid_resistance)

#define HImproved_acid_resistance u.uprops[IMPROVED_ACID_RESISTANCE].intrinsic
#define EImproved_acid_resistance u.uprops[IMPROVED_ACID_RESISTANCE].extrinsic
#define Improved_acid_resistance (HImproved_acid_resistance || EImproved_acid_resistance)


#define HStone_resistance u.uprops[STONE_RESISTANCE].intrinsic
#define EStone_resistance u.uprops[STONE_RESISTANCE].extrinsic
#define Stone_resistance (HStone_resistance || EStone_resistance)

#define HSick_resistance u.uprops[SICK_RESISTANCE].intrinsic
#define ESick_resistance u.uprops[SICK_RESISTANCE].extrinsic
#define Sick_resistance (HSick_resistance || ESick_resistance)

#define HInvulnerable u.uprops[INVULNERABLE].intrinsic
#define EInvulnerable u.uprops[INVULNERABLE].extrinsic
#define Invulnerable (HInvulnerable || EInvulnerable)


/*** Troubles ***/
/* Pseudo-property */
#define Punished (uball != 0)

/* Those implemented solely as timeouts (we use just intrinsic) */
#define HStun u.uprops[STUNNED].intrinsic
#define Stunned HStun

#define HConfusion u.uprops[CONFUSION].intrinsic
#define Confusion HConfusion

#define HBlindfolded u.uprops[BLINDFOLDED].intrinsic
#define EBlindfolded u.uprops[BLINDFOLDED].extrinsic
#define Blindfolded (HBlindfolded|| EBlindfolded)

/* Certain troubles act as timers only */
#define Blinded u.uprops[BLINDED].intrinsic

/* ...means blind because of a cover */
#define Blind                                     \
    ((u.uroleplay.blind || Blinded || Blindfolded \
      || !haseyes(youmonst.data))                 \
     && !Blocks_Blindness)
/* ...the Eyes operate even when you really are blind
    or don't have any eyes */
#define Blind_because_of_blindfold_only                                            \
    (Blindfolded && !Blocks_Blindness \
     && !u.uroleplay.blind && !Blinded && haseyes(youmonst.data))

#define Sick u.uprops[SICK].intrinsic
#define FoodPoisoned u.uprops[FOOD_POISONED].intrinsic
#define Stoned u.uprops[STONED].intrinsic
#define Strangled u.uprops[STRANGLED].intrinsic
#define Vomiting u.uprops[VOMITING].intrinsic
#define Glib u.uprops[GLIB].intrinsic
#define Slimed u.uprops[SLIMED].intrinsic /* [Tom] */
#define MummyRot u.uprops[MUMMY_ROT].intrinsic

#define HHallucination u.uprops[HALLUC].intrinsic
#define EHallucination u.uprops[HALLUC].extrinsic
#define HHalluc_resistance u.uprops[HALLUC_RES].intrinsic
#define EHalluc_resistance u.uprops[HALLUC_RES].extrinsic
#define Halluc_resistance (HHalluc_resistance || EHalluc_resistance)
#define Hallucination ((HHallucination || EHallucination) && !Halluc_resistance)


/* Timeout, plus a worn mask */
#define HDeaf u.uprops[DEAF].intrinsic
#define EDeaf u.uprops[DEAF].extrinsic
#define Deaf (HDeaf || EDeaf)

#define HAirless_environment u.uprops[AIRLESS_ENVIRONMENT].intrinsic
#define EAirless_environment u.uprops[AIRLESS_ENVIRONMENT].extrinsic
#define Airless_environment (HAirless_environment || EAirless_environment)

#define HFumbling u.uprops[FUMBLING].intrinsic
#define EFumbling u.uprops[FUMBLING].extrinsic
#define Fumbling (HFumbling || EFumbling)

#define HFlash_resistance u.uprops[FLASH_RESISTANCE].intrinsic
#define EFlash_resistance u.uprops[FLASH_RESISTANCE].extrinsic
#define Flash_resistance (HFlash_resistance || EFlash_resistance)

#define HStun_resistance u.uprops[STUN_RESISTANCE].intrinsic
#define EStun_resistance u.uprops[STUN_RESISTANCE].extrinsic
#define Stun_resistance (HStun_resistance || EStun_resistance)

#define HBisection_resistance u.uprops[BISECTION_RESISTANCE].intrinsic
#define EBisection_resistance u.uprops[BISECTION_RESISTANCE].extrinsic
#define Bisection_resistance (HBisection_resistance || EBisection_resistance)

#define HSlime_resistance u.uprops[SLIME_RESISTANCE].intrinsic
#define ESlime_resistance u.uprops[SLIME_RESISTANCE].extrinsic
#define Slime_resistance (HSlime_resistance || ESlime_resistance)

#define HEnhanced_vision u.uprops[ENHANCED_VISION].intrinsic
#define EEnhanced_vision u.uprops[ENHANCED_VISION].extrinsic
#define Enhanced_vision (HEnhanced_vision || EEnhanced_vision)

#define HEnhanced_untrap u.uprops[ENHANCED_UNTRAP].intrinsic
#define EEnhanced_untrap u.uprops[ENHANCED_UNTRAP].extrinsic
#define Enhanced_untrap (HEnhanced_untrap || EEnhanced_untrap)

#define HEye_protection u.uprops[EYE_PROTECTION].intrinsic
#define EEye_protection u.uprops[EYE_PROTECTION].extrinsic
#define Eye_protection (HEye_protection || EEye_protection)

#define HBrain_protection u.uprops[BRAIN_PROTECTION].intrinsic
#define EBrain_protection u.uprops[BRAIN_PROTECTION].extrinsic
#define Brain_protection (HBrain_protection || EBrain_protection)

#define HMagical_shielding u.uprops[MAGICAL_SHIELDING].intrinsic
#define EMagical_shielding u.uprops[MAGICAL_SHIELDING].extrinsic
#define Magical_shielding (HMagical_shielding || EMagical_shielding)

#define HMagical_barkskin u.uprops[MAGICAL_BARKSKIN].intrinsic
#define EMagical_barkskin u.uprops[MAGICAL_BARKSKIN].extrinsic
#define Magical_barkskin (HMagical_barkskin || EMagical_barkskin)

#define HMagical_stoneskin u.uprops[MAGICAL_STONESKIN].intrinsic
#define EMagical_stoneskin u.uprops[MAGICAL_STONESKIN].extrinsic
#define Magical_stoneskin (HMagical_stoneskin || EMagical_stoneskin)

#define HLaughing u.uprops[LAUGHING].intrinsic
#define ELaughing u.uprops[LAUGHING].extrinsic
#define Laughing (HLaughing || ELaughing)

#define HOddIdeas u.uprops[ODD_IDEAS].intrinsic
#define EOddIdeas u.uprops[ODD_IDEAS].extrinsic
#define OddIdeas (HOddIdeas || EOddIdeas)

#define HWounded_legs u.uprops[WOUNDED_LEGS].intrinsic
#define EWounded_legs u.uprops[WOUNDED_LEGS].extrinsic
#define Wounded_legs (HWounded_legs || EWounded_legs)

#define HSleepy u.uprops[SLEEPY].intrinsic
#define ESleepy u.uprops[SLEEPY].extrinsic
#define Sleepy (HSleepy || ESleepy)

#define HHunger u.uprops[HUNGER].intrinsic
#define EHunger u.uprops[HUNGER].extrinsic
#define Hunger (HHunger || EHunger)

/*** Vision and senses ***/
#define HSee_invisible u.uprops[SEE_INVISIBLE].intrinsic
#define ESee_invisible u.uprops[SEE_INVISIBLE].extrinsic
#define See_invisible (HSee_invisible || ESee_invisible)

#define HTelepat u.uprops[TELEPAT].intrinsic
#define ETelepat u.uprops[TELEPAT].extrinsic
#define Telepat (HTelepat || ETelepat)
#define Unblind_telepat Telepat

#define HBlind_telepat u.uprops[BLIND_TELEPATHY].intrinsic
#define EBlind_telepat u.uprops[BLIND_TELEPATHY].extrinsic
#define Blind_telepat (HBlind_telepat || EBlind_telepat)

#define HXRay_vision u.uprops[XRAY_VISION].intrinsic
#define EXRay_vision u.uprops[XRAY_VISION].extrinsic
#define XRay_vision (HXRay_vision || EXRay_vision)

#define HWarning u.uprops[WARNING].intrinsic
#define EWarning u.uprops[WARNING].extrinsic
#define Warning (HWarning || EWarning)

/* Warning for a specific type of monster */
#define HWarn_of_mon u.uprops[WARN_OF_MON].intrinsic
#define EWarn_of_mon u.uprops[WARN_OF_MON].extrinsic
#define Warn_of_mon (HWarn_of_mon || EWarn_of_mon)

#define HUndead_warning u.uprops[WARN_UNDEAD].intrinsic
#define EUndead_warning u.uprops[WARN_UNDEAD].extrinsic
#define Undead_warning (HUndead_warning || EUndead_warning)

#define HDemon_warning u.uprops[WARN_DEMON].intrinsic
#define EDemon_warning u.uprops[WARN_DEMON].extrinsic
#define Demon_warning (HDemon_warning || EDemon_warning)

#define HOrc_warning u.uprops[WARN_ORC].intrinsic
#define EOrc_warning u.uprops[WARN_ORC].extrinsic
#define Orc_warning (HOrc_warning || EOrc_warning)

#define HTroll_warning u.uprops[WARN_TROLL].intrinsic
#define ETroll_warning u.uprops[WARN_TROLL].extrinsic
#define Troll_warning (HTroll_warning || ETroll_warning)

#define HGiant_warning u.uprops[WARN_GIANT].intrinsic
#define EGiant_warning u.uprops[WARN_GIANT].extrinsic
#define Giant_warning (HGiant_warning || EGiant_warning)

#define HDragon_warning u.uprops[WARN_DRAGON].intrinsic
#define EDragon_warning u.uprops[WARN_DRAGON].extrinsic
#define Dragon_warning (HDragon_warning || EDragon_warning)

#define HElf_warning u.uprops[WARN_ELF].intrinsic
#define EElf_warning u.uprops[WARN_ELF].extrinsic
#define Elf_warning (HElf_warning || EElf_warning)

#define HDwarf_warning u.uprops[WARN_DWARF].intrinsic
#define EDwarf_warning u.uprops[WARN_DWARF].extrinsic
#define Dwarf_warning (HDwarf_warning || EDwarf_warning)

#define HGnoll_warning u.uprops[WARN_GNOLL].intrinsic
#define EGnoll_warning u.uprops[WARN_GNOLL].extrinsic
#define Gnoll_warning (HGnoll_warning || EGnoll_warning)

#define HGnome_warning u.uprops[WARN_GNOME].intrinsic
#define EGnome_warning u.uprops[WARN_GNOME].extrinsic
#define Gnome_warning (HGnome_warning || EGnome_warning)

#define HOgre_warning u.uprops[WARN_OGRE].intrinsic
#define EOgre_warning u.uprops[WARN_OGRE].extrinsic
#define Ogre_warning (HOgre_warning || EOgre_warning)


#define HHuman_warning u.uprops[WARN_HUMAN].intrinsic
#define EHuman_warning u.uprops[WARN_HUMAN].extrinsic
#define Human_warning (HHuman_warning || EHuman_warning)

#define HLycanthrope_warning u.uprops[WARN_LYCANTHROPE].intrinsic
#define ELycanthrope_warning u.uprops[WARN_LYCANTHROPE].extrinsic
#define Lycanthrope_warning (HLycanthrope_warning || ELycanthrope_warning)

#define HAngel_warning u.uprops[WARN_ANGEL].intrinsic
#define EAngel_warning u.uprops[WARN_ANGEL].extrinsic
#define Angel_warning (HAngel_warning || EAngel_warning)

#define Any_warning (Undead_warning || Demon_warning || Orc_warning || Troll_warning \
                    || Giant_warning || Dragon_warning || Elf_warning || Dwarf_warning \
                    || Gnoll_warning || Human_warning || Lycanthrope_warning || Angel_warning \
                    || Ogre_warning || Gnome_warning)


#define HSearching u.uprops[SEARCHING].intrinsic
#define ESearching u.uprops[SEARCHING].extrinsic
#define Searching (HSearching || ESearching)

#define HClairvoyant u.uprops[CLAIRVOYANT].intrinsic
#define EClairvoyant u.uprops[CLAIRVOYANT].extrinsic
#define Clairvoyant ((HClairvoyant || EClairvoyant) && !Blocks_Clairvoyance)

#define HInfravision u.uprops[INFRAVISION].intrinsic
#define EInfravision u.uprops[INFRAVISION].extrinsic
#define Infravision (HInfravision || EInfravision)

#define HDetect_monsters u.uprops[DETECT_MONSTERS].intrinsic
#define EDetect_monsters u.uprops[DETECT_MONSTERS].extrinsic
#define Detect_monsters (HDetect_monsters || EDetect_monsters)

/*** Appearance and behavior ***/
#define Adornment u.uprops[ADORNED].extrinsic

#define HInvis u.uprops[INVISIBILITY].intrinsic
#define EInvis u.uprops[INVISIBILITY].extrinsic
#define Invis ((HInvis || EInvis) && !Blocks_Invisibility)
#define Invisib (Invis && !See_invisible)
/* Note: invisibility also hides inventory and steed */

#define HDisplaced u.uprops[DISPLACED].intrinsic
#define EDisplaced u.uprops[DISPLACED].extrinsic
#define Displaced (HDisplaced || EDisplaced)

#define HStealth u.uprops[STEALTH].intrinsic
#define EStealth u.uprops[STEALTH].extrinsic
#define Stealth (HStealth || EStealth)

#define HMagical_kicking u.uprops[MAGICAL_KICKING].intrinsic
#define EMagical_kicking u.uprops[MAGICAL_KICKING].extrinsic
#define Magical_kicking (HMagical_kicking || EMagical_kicking)

#define HAggravate_monster u.uprops[AGGRAVATE_MONSTER].intrinsic
#define EAggravate_monster u.uprops[AGGRAVATE_MONSTER].extrinsic
#define Aggravate_monster (HAggravate_monster || EAggravate_monster)

#define HConflict u.uprops[CONFLICT].intrinsic
#define EConflict u.uprops[CONFLICT].extrinsic
#define Conflict (HConflict || EConflict)

/*** Transportation ***/
#define HJumping u.uprops[JUMPING].intrinsic
#define EJumping u.uprops[JUMPING].extrinsic
#define Jumping (HJumping || EJumping)

#define HTeleportation u.uprops[TELEPORT].intrinsic
#define ETeleportation u.uprops[TELEPORT].extrinsic
#define Teleportation (HTeleportation || ETeleportation)

#define HTeleport_control u.uprops[TELEPORT_CONTROL].intrinsic
#define ETeleport_control u.uprops[TELEPORT_CONTROL].extrinsic
#define Teleport_control (HTeleport_control || ETeleport_control)

/* HLevitation has I_SPECIAL set if levitating due to blessed potion
   which allows player to use the '>' command to end levitation early */
#define HLevitation u.uprops[LEVITATION].intrinsic
#define ELevitation u.uprops[LEVITATION].extrinsic
/* BLevitation has I_SPECIAL set if trapped in the floor,
   FROM_ACQUIRED set if inside solid rock (or in water on Plane of Water) */
#define Levitation ((HLevitation || ELevitation) && !Blocks_Levitation)

#define HLevitation_control u.uprops[LEVITATION_CONTROL].intrinsic
#define ELevitation_control u.uprops[LEVITATION_CONTROL].extrinsic
#define Levitation_control (HLevitation_control || ELevitation_control)

/* Can't touch surface, can't go under water; overrides all others */
#define Lev_at_will                                                    \
    ((Levitation && Levitation_control) || (((HLevitation & I_SPECIAL) != 0L || (ELevitation & W_ARTIFACT_INVOKED) != 0L) \
     && (HLevitation & ~(I_SPECIAL | TIMEOUT)) == 0L                   \
     && (ELevitation & ~W_ARTIFACT_INVOKED) == 0L))

/* Flying is overridden by Levitation */
#define HFlying u.uprops[FLYING].intrinsic
#define EFlying u.uprops[FLYING].extrinsic
/* BFlying has I_SPECIAL set if levitating or trapped in the floor or both,
   FROM_ACQUIRED set if inside solid rock (or in water on Plane of Water) */
#define Flying                                                      \
    (((HFlying || EFlying) && !Blocks_Flying) || (u.usteed && is_flying(u.usteed)))
/* May touch surface; does not override any others */

#define HWwalking u.uprops[WATER_WALKING].intrinsic
#define EWwalking u.uprops[WATER_WALKING].extrinsic
#define Wwalking ((EWwalking || HWwalking) && !Is_waterlevel(&u.uz))
/* Don't get wet, can't go under water; overrides others except levitation */
/* Wwalking is meaningless on water level */

#define HSwimming u.uprops[SWIMMING].intrinsic
#define ESwimming u.uprops[SWIMMING].extrinsic /* [Tom] */
#define Swimming \
    (HSwimming || ESwimming || (u.usteed && is_swimmer(u.usteed->data)))
/* Get wet, don't go under water unless if amphibious */

#define HMagical_breathing u.uprops[MAGICAL_BREATHING].intrinsic
#define EMagical_breathing u.uprops[MAGICAL_BREATHING].extrinsic
#define Magical_breathing (EMagical_breathing || HMagical_breathing)
#define Amphibious \
    (Magical_breathing || amphibious(youmonst.data))
/* Get wet, may go under surface */

#define Breathless \
    (Magical_breathing || has_innate_breathless(youmonst.data))

#define Underwater (u.uinwater)

#define Survives_without_air (Breathless || (Underwater && (Amphibious || Swimming)))

/* Note that Underwater and u.uinwater are both used in code.
   The latter form is for later implementation of other in-water
   states, like swimming, wading, etc. */

#define HPasses_walls u.uprops[PASSES_WALLS].intrinsic
#define EPasses_walls u.uprops[PASSES_WALLS].extrinsic
#define Passes_walls (HPasses_walls || EPasses_walls)

/*** Physical attributes ***/
#define HSlow_digestion u.uprops[SLOW_DIGESTION].intrinsic
#define ESlow_digestion u.uprops[SLOW_DIGESTION].extrinsic
#define Slow_digestion (HSlow_digestion || ESlow_digestion) /* KMH */

#define HHalf_slow_digestion u.uprops[HALF_SLOW_DIGESTION].intrinsic
#define EHalf_slow_digestion u.uprops[HALF_SLOW_DIGESTION].extrinsic
#define Half_slow_digestion (HHalf_slow_digestion || EHalf_slow_digestion) /* JG */

#define HHalf_spell_damage u.uprops[HALF_SPELL_DAMAGE].intrinsic
#define EHalf_spell_damage u.uprops[HALF_SPELL_DAMAGE].extrinsic
#define Half_spell_damage (HHalf_spell_damage || EHalf_spell_damage)

/*
 * Physical damage
 *
 * Damage is NOT physical damage if (in order of priority):
 * 1. it already qualifies for some other special category
 *    for which a special resistance already exists in the game
 *    including: cold, fire, shock, acid, and magic.
 *    Note that fire is extended to include all non-acid forms of
 *    burning, even boiling water since that is already dealt with
 *    by fire resistance, and in most or all cases is caused by fire.
 * 2. it doesn't leave a mark. Marks include destruction of, or
 *    damage to, an internal organ (including the brain),
 *    lacerations, bruises, crushed body parts, bleeding.
 *
 * The following were evaluated and determined _NOT_ to be
 * susceptible to Half_physical_damage protection:
 *   Being caught in a fireball                      [fire damage]
 *   Sitting in lava                                 [lava damage]
 *   Thrown potion (acid)                            [acid damage]
 *   Splattered burning oil from thrown potion       [fire damage]
 *   Mixing water and acid                           [acid damage]
 *   Molten lava (entering or being splashed)        [lava damage]
 *   boiling water from a sink                       [fire damage]
 *   Fire traps                                      [fire damage]
 *   Scrolls of fire (confused and otherwise)        [fire damage]
 *   Alchemical explosion                            [not physical]
 *   System shock                                    [shock damage]
 *   Bag of holding explosion                        [magical]
 *   Being undead-turned by your god                 [magical]
 *   Level-drain                                     [magical]
 *   Magical explosion of a magic trap               [magical]
 *   Sitting on a throne with a bad effect           [magical]
 *   Contaminated water from a sink                  [poison/sickness]
 *   Contact-poisoned spellbooks                     [poison/sickness]
 *   Eating acidic/poisonous/mildly-old corpses      [poison/sickness]
 *   Eating a poisoned weapon while polyselfed       [poison/sickness]
 *   Engulfing a zombie or mummy (AT_ENGL in hmonas) [poison/sickness]
 *   Quaffed potions of sickness, lit oil, acid      [poison/sickness]
 *   Pyrolisks' fiery gaze                           [fire damage]
 *   Any passive attack                              [most don't qualify]
 */

#define HHalf_physical_damage u.uprops[HALF_PHYSICAL_DAMAGE].intrinsic
#define EHalf_physical_damage u.uprops[HALF_PHYSICAL_DAMAGE].extrinsic
#define Half_physical_damage (HHalf_physical_damage || EHalf_physical_damage)

#define HHalf_physical_damage_against_undead_and_demons u.uprops[HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS].intrinsic
#define EHalf_physical_damage_against_undead_and_demons u.uprops[HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS].extrinsic
#define Half_physical_damage_against_undead_and_demons (HHalf_physical_damage_against_undead_and_demons || EHalf_physical_damage_against_undead_and_demons)

#define HDouble_physical_damage u.uprops[DOUBLE_PHYSICAL_DAMAGE].intrinsic
#define EDouble_physical_damage u.uprops[DOUBLE_PHYSICAL_DAMAGE].extrinsic
#define Double_physical_damage (HDouble_physical_damage || EDouble_physical_damage)

#define HDouble_spell_damage u.uprops[DOUBLE_SPELL_DAMAGE].intrinsic
#define EDouble_spell_damage u.uprops[DOUBLE_SPELL_DAMAGE].extrinsic
#define Double_spell_damage (HDouble_spell_damage || EDouble_spell_damage)

#define HFire_vulnerability u.uprops[FIRE_VULNERABILITY].intrinsic
#define EFire_vulnerability u.uprops[FIRE_VULNERABILITY].extrinsic
#define Fire_vulnerability (HFire_vulnerability || EFire_vulnerability)

#define HCold_vulnerability u.uprops[COLD_VULNERABILITY].intrinsic
#define ECold_vulnerability u.uprops[COLD_VULNERABILITY].extrinsic
#define Cold_vulnerability (HCold_vulnerability || ECold_vulnerability)

#define HElec_vulnerability u.uprops[SHOCK_VULNERABILITY].intrinsic
#define EElec_vulnerability u.uprops[SHOCK_VULNERABILITY].extrinsic
#define Elec_vulnerability (HElec_vulnerability || EElec_vulnerability)

#define HMagic_damage_vulnerability u.uprops[MAGIC_MISSILE_VULNERABILITY].intrinsic
#define EMagic_damage_vulnerability u.uprops[MAGIC_MISSILE_VULNERABILITY].extrinsic
#define Magic_damage_vulnerability (HMagic_damage_vulnerability || EMagic_damage_vulnerability)

#define HRegeneration u.uprops[REGENERATION].intrinsic
#define ERegeneration u.uprops[REGENERATION].extrinsic
#define Regeneration (HRegeneration || ERegeneration)

#define HEnergy_regeneration u.uprops[ENERGY_REGENERATION].intrinsic
#define EEnergy_regeneration u.uprops[ENERGY_REGENERATION].extrinsic
#define Energy_regeneration (HEnergy_regeneration || EEnergy_regeneration)

#define HMagical_protection u.uprops[MAGICAL_PROTECTION].intrinsic
#define EMagical_protection u.uprops[MAGICAL_PROTECTION].extrinsic
#define Magical_protection (HMagical_protection || EMagical_protection)

#define HProtection_from_shape_changers \
    u.uprops[PROT_FROM_SHAPE_CHANGERS].intrinsic
#define EProtection_from_shape_changers \
    u.uprops[PROT_FROM_SHAPE_CHANGERS].extrinsic
#define Protection_from_shape_changers \
    (HProtection_from_shape_changers || EProtection_from_shape_changers)

#define HPolymorph u.uprops[POLYMORPH].intrinsic
#define EPolymorph u.uprops[POLYMORPH].extrinsic
#define Polymorph (HPolymorph || EPolymorph)

#define HPolymorph_control u.uprops[POLYMORPH_CONTROL].intrinsic
#define EPolymorph_control u.uprops[POLYMORPH_CONTROL].extrinsic
#define Polymorph_control (HPolymorph_control || EPolymorph_control)

#define HUnchanging u.uprops[UNCHANGING].intrinsic
#define EUnchanging u.uprops[UNCHANGING].extrinsic
#define Unchanging (HUnchanging || EUnchanging) /* KMH */

#define HCancelled u.uprops[CANCELLED].intrinsic
#define ECancelled u.uprops[CANCELLED].extrinsic
#define Cancelled (HCancelled || ECancelled)

#define HCancellation_resistance u.uprops[CANCELLATION_RESISTANCE].intrinsic
#define ECancellation_resistance u.uprops[CANCELLATION_RESISTANCE].extrinsic
#define Cancellation_resistance (HCancellation_resistance || ECancellation_resistance)


#define HThree_fourths_magic_resistance u.uprops[THREE_FOURTHS_MAGIC_RESISTANCE].intrinsic
#define EThree_fourths_magic_resistance u.uprops[THREE_FOURTHS_MAGIC_RESISTANCE].extrinsic
#define Three_fourths_magic_resistance (HThree_fourths_magic_resistance || EThree_fourths_magic_resistance)

#define HHalf_magic_resistance u.uprops[HALVED_MAGIC_RESISTANCE].intrinsic
#define EHalf_magic_resistance u.uprops[HALVED_MAGIC_RESISTANCE].extrinsic
#define Half_magic_resistance (HHalf_magic_resistance || EHalf_magic_resistance)

#define HOne_fourth_magic_resistance u.uprops[ONE_FOURTH_MAGIC_RESISTANCE].intrinsic
#define EOne_fourth_magic_resistance u.uprops[ONE_FOURTH_MAGIC_RESISTANCE].extrinsic
#define One_fourth_magic_resistance (HOne_fourth_magic_resistance || EOne_fourth_magic_resistance)

#define HNo_magic_resistance u.uprops[NO_MAGIC_RESISTANCE].intrinsic
#define ENo_magic_resistance u.uprops[NO_MAGIC_RESISTANCE].extrinsic
#define No_magic_resistance (HNo_magic_resistance || ENo_magic_resistance)

#define HSleeping u.uprops[SLEEPING].intrinsic
#define ESleeping u.uprops[SLEEPING].extrinsic
#define Sleeping (HSleeping || ESleeping)

#define HFearful u.uprops[FEARFUL].intrinsic
#define EFearful u.uprops[FEARFUL].extrinsic
#define Fearful (HFearful|| EFearful)

#define HSummon_forbidden u.uprops[SUMMON_FORBIDDEN].intrinsic
#define ESummon_forbidden u.uprops[SUMMON_FORBIDDEN].extrinsic
#define Summon_forbidden (HSummon_forbidden || ESummon_forbidden)

#define HCharmed u.uprops[CHARMED].intrinsic
#define ECharmed u.uprops[CHARMED].extrinsic
#define Charmed (HCharmed || ECharmed)

#define HUndead_control u.uprops[UNDEAD_CONTROL].intrinsic
#define EUndead_control u.uprops[UNDEAD_CONTROL].extrinsic
#define Undead_control (HUndead_control || EUndead_control)

#define Charmed_or_controlled ((Charmed && !Charm_resistance) || (Undead_control && is_undead(youmonst.data)))


#define HSlowed u.uprops[SLOWED].intrinsic
#define ESlowed u.uprops[SLOWED].extrinsic
#define Slowed (HSlowed || ESlowed)

#define HSilenced u.uprops[SILENCED].intrinsic
#define ESilenced u.uprops[SILENCED].extrinsic
#define Silenced (HSilenced || ESilenced)


#define HFast u.uprops[FAST].intrinsic
#define EFast u.uprops[FAST].extrinsic
#define Fast ((HFast || EFast) && !Slowed && !Stoned)

#define HVery_fast u.uprops[VERY_FAST].intrinsic
#define EVery_fast u.uprops[VERY_FAST].extrinsic
#define Very_fast ((HVery_fast || EVery_fast) && !Slowed && !Stoned)

#define HUltra_fast u.uprops[ULTRA_FAST].intrinsic
#define EUltra_fast u.uprops[ULTRA_FAST].extrinsic
#define Ultra_fast ((HUltra_fast || EUltra_fast) && !Slowed && !Stoned)

#define HSuper_fast u.uprops[SUPER_FAST].intrinsic
#define ESuper_fast u.uprops[SUPER_FAST].extrinsic
#define Super_fast ((HSuper_fast || ESuper_fast) && !Slowed && !Stoned)

#define HLightning_fast u.uprops[LIGHTNING_FAST].intrinsic
#define ELightning_fast u.uprops[LIGHTNING_FAST].extrinsic
#define Lightning_fast ((HLightning_fast || ELightning_fast) && !Slowed && !Stoned)

#define HHeroism u.uprops[HEROISM].intrinsic
#define EHeroism u.uprops[HEROISM].extrinsic
#define Heroism (HHeroism || EHeroism)

#define HSuper_heroism u.uprops[SUPER_HEROISM].intrinsic
#define ESuper_heroism u.uprops[SUPER_HEROISM].extrinsic
#define Super_heroism (HSuper_heroism || ESuper_heroism)

#define HRapid_regeneration u.uprops[RAPID_REGENERATION].intrinsic
#define ERapid_regeneration u.uprops[RAPID_REGENERATION].extrinsic
#define Rapid_regeneration (HRapid_regeneration || ERapid_regeneration)

#define HRapider_regeneration u.uprops[RAPIDER_REGENERATION].intrinsic
#define ERapider_regeneration u.uprops[RAPIDER_REGENERATION].extrinsic
#define Rapider_regeneration (HRapider_regeneration || ERapider_regeneration)

#define HRapidest_regeneration u.uprops[RAPIDEST_REGENERATION].intrinsic
#define ERapidest_regeneration u.uprops[RAPIDEST_REGENERATION].extrinsic
#define Rapidest_regeneration (HRapidest_regeneration || ERapidest_regeneration)

#define HRapid_energy_regeneration u.uprops[RAPID_ENERGY_REGENERATION].intrinsic
#define ERapid_energy_regeneration u.uprops[RAPID_ENERGY_REGENERATION].extrinsic
#define Rapid_energy_regeneration (HRapid_energy_regeneration || ERapid_energy_regeneration)

#define HRapider_energy_regeneration u.uprops[RAPIDER_ENERGY_REGENERATION].intrinsic
#define ERapider_energy_regeneration u.uprops[RAPIDER_ENERGY_REGENERATION].extrinsic
#define Rapider_energy_regeneration (HRapider_energy_regeneration || ERapider_energy_regeneration)

#define HRapidest_energy_regeneration u.uprops[RAPIDEST_ENERGY_REGENERATION].intrinsic
#define ERapidest_energy_regeneration u.uprops[RAPIDEST_ENERGY_REGENERATION].extrinsic
#define Rapidest_energy_regeneration (HRapidest_energy_regeneration || ERapidest_energy_regeneration)

#define HMelee_life_leech u.uprops[MELEE_LIFE_LEECH].intrinsic
#define EMelee_life_leech u.uprops[MELEE_LIFE_LEECH].extrinsic
#define Melee_life_leech (HMelee_life_leech || EMelee_life_leech)

#define HCrazed u.uprops[CRAZED].intrinsic
#define ECrazed u.uprops[CRAZED].extrinsic
#define Crazed (HCrazed || ECrazed)

#define HDivine_regeneration u.uprops[DIVINE_REGENERATION].intrinsic
#define EDivine_regeneration u.uprops[DIVINE_REGENERATION].extrinsic
#define Divine_regeneration (HDivine_regeneration || EDivine_regeneration)



#define HReflecting u.uprops[REFLECTING].intrinsic
#define EReflecting u.uprops[REFLECTING].extrinsic
#define Reflecting (HReflecting || EReflecting)

#define HFree_action u.uprops[FREE_ACTION].intrinsic
#define EFree_action u.uprops[FREE_ACTION].extrinsic
#define Free_action (HFree_action || EFree_action)

#define HUndead_immobility u.uprops[UNDEAD_IMMOBILITY].intrinsic
#define EUndead_immobility u.uprops[UNDEAD_IMMOBILITY].extrinsic
#define Undead_immobility (HUndead_immobility || EUndead_immobility)

#define HParalyzed u.uprops[PARALYZED].intrinsic
#define EParalyzed u.uprops[PARALYZED].extrinsic
#define Paralyzed (HParalyzed || EParalyzed)

#define Paralyzed_or_immobile ((Paralyzed || (is_undead(youmonst.data) && Undead_immobility))) // && !Free_action

#define HFixed_abil u.uprops[FIXED_ABIL].intrinsic
#define EFixed_abil u.uprops[FIXED_ABIL].extrinsic
#define Fixed_abil (HFixed_abil || EFixed_abil)

#define HLifesaved u.uprops[LIFESAVED].intrinsic
#define ELifesaved u.uprops[LIFESAVED].extrinsic
#define Lifesaved (HLifesaved || ELifesaved)

/* Spell intrinsics / buffs */
#define HTitan_strength u.uprops[TITAN_STRENGTH].intrinsic
#define ETitan_strength u.uprops[TITAN_STRENGTH].extrinsic
#define Titan_strength (HTitan_strength || ETitan_strength)

#define HDivine_dexterity u.uprops[DIVINE_DEXTERITY].intrinsic
#define EDivine_dexterity u.uprops[DIVINE_DEXTERITY].extrinsic
#define Divine_dexterity (HDivine_dexterity || EDivine_dexterity)

#define HDivine_endurance u.uprops[DIVINE_ENDURANCE].intrinsic
#define EDivine_endurance u.uprops[DIVINE_ENDURANCE].extrinsic
#define Divine_endurance (HDivine_endurance || EDivine_endurance)

#define HDivine_intellect u.uprops[DIVINE_INTELLECT].intrinsic
#define EDivine_intellect u.uprops[DIVINE_INTELLECT].extrinsic
#define Divine_intellect (HDivine_intellect || EDivine_intellect)

#define HDivine_wisdom u.uprops[DIVINE_WISDOM].intrinsic
#define EDivine_wisdom u.uprops[DIVINE_WISDOM].extrinsic
#define Divine_wisdom (HDivine_wisdom || EDivine_wisdom)

#define HDivine_charisma u.uprops[DIVINE_CHARISMA].intrinsic
#define EDivine_charisma u.uprops[DIVINE_CHARISMA].extrinsic
#define Divine_charisma (HDivine_charisma || EDivine_charisma)

/*
 * Some pseudo-properties.
 */

/* unconscious() includes u.usleep but not is_fainted(); the multi test is
   redundant but allows the function calls to be skipped most of the time */
#define Unaware (multi < 0 && (unconscious() || is_fainted() || Sleeping))

#define Hate_silver (u.ulycn >= LOW_PM || hates_silver(youmonst.data))

#endif /* YOUPROP_H */
