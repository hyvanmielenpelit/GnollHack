/* GnollHack 4.0	youprop.h	$NHDT-Date: 1433291407 2015/06/03 00:30:07 $  $NHDT-Branch: master $:$NHDT-Revision: 1.23 $ */
/* Copyright (c) 1989 Mike Threepoint				  */
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
#define BBlocks_Invisibility u.uprops[BLOCKS_INVISIBILITY].blocked
#define Blocks_Invisibility ((HBlocks_Invisibility || EBlocks_Invisibility) && !BBlocks_Invisibility)

#define HBlocks_Blindness u.uprops[BLOCKS_BLINDNESS].intrinsic
#define EBlocks_Blindness u.uprops[BLOCKS_BLINDNESS].extrinsic
#define BBlocks_Blindness u.uprops[BLOCKS_BLINDNESS].blocked
#define Blocks_Blindness ((HBlocks_Blindness|| EBlocks_Blindness) && !BBlocks_Blindness)

#define HBlocks_Clairvoyance u.uprops[BLOCKS_CLAIRVOYANCE].intrinsic
#define EBlocks_Clairvoyance u.uprops[BLOCKS_CLAIRVOYANCE].extrinsic
#define BBlocks_Clairvoyance u.uprops[BLOCKS_CLAIRVOYANCE].blocked
#define Blocks_Clairvoyance ((HBlocks_Clairvoyance|| EBlocks_Clairvoyance) && !BBlocks_Clairvoyance)


/*** Resistances to troubles ***/
/* With intrinsics and extrinsics */
#define HFire_resistance u.uprops[FIRE_RES].intrinsic
#define EFire_resistance u.uprops[FIRE_RES].extrinsic
#define Fire_resistance (HFire_resistance || EFire_resistance)

#define HCold_resistance u.uprops[COLD_RES].intrinsic
#define ECold_resistance u.uprops[COLD_RES].extrinsic
#define Cold_resistance (HCold_resistance || ECold_resistance)

#define HSleep_resistance u.uprops[SLEEP_RES].intrinsic
#define ESleep_resistance u.uprops[SLEEP_RES].extrinsic
#define Sleep_resistance (HSleep_resistance || ESleep_resistance)

#define HDisint_resistance u.uprops[DISINT_RES].intrinsic
#define EDisint_resistance u.uprops[DISINT_RES].extrinsic
#define Disint_resistance (HDisint_resistance || EDisint_resistance)

#define HShock_resistance u.uprops[SHOCK_RES].intrinsic
#define EShock_resistance u.uprops[SHOCK_RES].extrinsic
#define Shock_resistance (HShock_resistance || EShock_resistance)

#define HDeath_resistance u.uprops[DEATH_RES].intrinsic
#define EDeath_resistance u.uprops[DEATH_RES].extrinsic
#define Death_resistance (HDeath_resistance || EDeath_resistance)

#define HLycanthropy_resistance u.uprops[LYCANTHROPY_RES].intrinsic
#define ELycanthropy_resistance u.uprops[LYCANTHROPY_RES].extrinsic
#define Lycanthropy_resistance (HLycanthropy_resistance || ELycanthropy_resistance)

#define HCurse_resistance u.uprops[CURSE_RES].intrinsic
#define ECurse_resistance u.uprops[CURSE_RES].extrinsic
#define Curse_resistance (HCurse_resistance || ECurse_resistance)

#define HPoison_resistance u.uprops[POISON_RES].intrinsic
#define EPoison_resistance u.uprops[POISON_RES].extrinsic
#define Poison_resistance (HPoison_resistance || EPoison_resistance)

#define HDrain_resistance u.uprops[DRAIN_RES].intrinsic
#define EDrain_resistance u.uprops[DRAIN_RES].extrinsic
#define Drain_resistance (HDrain_resistance || EDrain_resistance)

#define HCharm_resistance u.uprops[CHARM_RES].intrinsic
#define ECharm_resistance u.uprops[CHARM_RES].extrinsic
#define Charm_resistance (HCharm_resistance || ECharm_resistance)

#define HMind_shielding u.uprops[MIND_SHIELDING].intrinsic
#define EMind_shielding u.uprops[MIND_SHIELDING].extrinsic
#define Mind_shielding (HMind_shielding || EMind_shielding)


/* Hxxx due to FROM_FORM only */
#define HAntimagic u.uprops[ANTIMAGIC].intrinsic
#define EAntimagic u.uprops[ANTIMAGIC].extrinsic
#define Antimagic (HAntimagic || EAntimagic)

#define HAcid_resistance u.uprops[ACID_RES].intrinsic
#define EAcid_resistance u.uprops[ACID_RES].extrinsic
#define Acid_resistance (HAcid_resistance || EAcid_resistance)

#define HStone_resistance u.uprops[STONE_RES].intrinsic
#define EStone_resistance u.uprops[STONE_RES].extrinsic
#define Stone_resistance (HStone_resistance || EStone_resistance)

/* Intrinsics only */
#define HSick_resistance u.uprops[SICK_RES].intrinsic
#define Sick_resistance (HSick_resistance || defends(AD_DISE, uwep))

#define Invulnerable u.uprops[INVULNERABLE].intrinsic /* [Tom] */


/*** Troubles ***/
/* Pseudo-property */
#define Punished (uball != 0)

/* Those implemented solely as timeouts (we use just intrinsic) */
#define HStun u.uprops[STUNNED].intrinsic
#define Stunned HStun

#define HConfusion u.uprops[CONFUSION].intrinsic
#define Confusion HConfusion

#define Blinded u.uprops[BLINDED].intrinsic
#define Blindfolded (ublindf != 0)
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
#define Stoned u.uprops[STONED].intrinsic
#define Strangled u.uprops[STRANGLED].intrinsic
#define Vomiting u.uprops[VOMITING].intrinsic
#define Glib u.uprops[GLIB].intrinsic
#define Slimed u.uprops[SLIMED].intrinsic /* [Tom] */

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

#define HFlash_resistance u.uprops[FLASH_RES].intrinsic
#define EFlash_resistance u.uprops[FLASH_RES].extrinsic
#define Flash_resistance (HFlash_resistance || EFlash_resistance)

#define HEnhanced_vision u.uprops[ENHANCED_VISION].intrinsic
#define EEnhanced_vision u.uprops[ENHANCED_VISION].extrinsic
#define Enhanced_vision (HEnhanced_vision || EEnhanced_vision)

#define HEye_protection u.uprops[EYE_PROTECTION].intrinsic
#define EEye_protection u.uprops[EYE_PROTECTION].extrinsic
#define Eye_protection (HEye_protection || EEye_protection)

#define HBrain_protection u.uprops[BRAIN_PROTECTION].intrinsic
#define EBrain_protection u.uprops[BRAIN_PROTECTION].extrinsic
#define Brain_protection (HBrain_protection || EBrain_protection)

#define HMagical_shielding u.uprops[MAGICAL_SHIELDING].intrinsic
#define EMagical_shielding u.uprops[MAGICAL_SHIELDING].extrinsic
#define BMagical_shielding u.uprops[MAGICAL_SHIELDING].blocked
#define Magical_shielding ((HMagical_shielding || EMagical_shielding) && !BMagical_shielding)

#define HMagical_barkskin u.uprops[MAGICAL_BARKSKIN].intrinsic
#define EMagical_barkskin u.uprops[MAGICAL_BARKSKIN].extrinsic
#define BMagical_barkskin u.uprops[MAGICAL_BARKSKIN].blocked
#define Magical_barkskin ((HMagical_barkskin || EMagical_barkskin) && !BMagical_barkskin)

#define HMagical_stoneskin u.uprops[MAGICAL_STONESKIN].intrinsic
#define EMagical_stoneskin u.uprops[MAGICAL_STONESKIN].extrinsic
#define BMagical_stoneskin u.uprops[MAGICAL_STONESKIN].blocked
#define Magical_stoneskin ((HMagical_stoneskin || EMagical_stoneskin) && !BMagical_stoneskin)

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
#define HSee_invisible u.uprops[SEE_INVISIBILITY].intrinsic
#define ESee_invisible u.uprops[SEE_INVISIBILITY].extrinsic
#define See_invisible (HSee_invisible || ESee_invisible)

#define HTelepat u.uprops[TELEPAT].intrinsic
#define ETelepat u.uprops[TELEPAT].extrinsic
#define Telepat (HTelepat || ETelepat)
#define Unblind_telepat Telepat

#define HBlind_telepat u.uprops[BLIND_TELEPAT].intrinsic
#define EBlind_telepat u.uprops[BLIND_TELEPAT].extrinsic
#define Blind_telepat (HBlind_telepat || EBlind_telepat)

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
#define BClairvoyant u.uprops[CLAIRVOYANT].blocked
#define Clairvoyant ((HClairvoyant || EClairvoyant) && !BClairvoyant && !Blocks_Clairvoyance)

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
#define BInvis u.uprops[INVISIBILITY].blocked
#define Invis ((HInvis || EInvis) && !BInvis && !Blocks_Invisibility)
#define Invisible (Invis && !See_invisible)
/* Note: invisibility also hides inventory and steed */

#define EDisplaced u.uprops[DISPLACED].extrinsic
#define Displaced EDisplaced

#define HStealth u.uprops[STEALTH].intrinsic
#define EStealth u.uprops[STEALTH].extrinsic
#define BStealth u.uprops[STEALTH].blocked
#define Stealth ((HStealth || EStealth) && !BStealth)

#define HMagical_kicking u.uprops[MAGICAL_KICKING].intrinsic
#define EMagical_kicking u.uprops[MAGICAL_KICKING].extrinsic
#define BMagical_kicking u.uprops[MAGICAL_KICKING].blocked
#define Magical_kicking ((HMagical_kicking || EMagical_kicking) && !BMagical_kicking)

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
#define BLevitation u.uprops[LEVITATION].blocked
#define Levitation ((HLevitation || ELevitation) && !BLevitation)
/* Can't touch surface, can't go under water; overrides all others */
#define Lev_at_will                                                    \
    (((HLevitation & I_SPECIAL) != 0L || (ELevitation & W_ARTIFACT_INVOKED) != 0L) \
     && (HLevitation & ~(I_SPECIAL | TIMEOUT)) == 0L                   \
     && (ELevitation & ~W_ARTIFACT_INVOKED) == 0L)

/* Flying is overridden by Levitation */
#define HFlying u.uprops[FLYING].intrinsic
#define EFlying u.uprops[FLYING].extrinsic
/* BFlying has I_SPECIAL set if levitating or trapped in the floor or both,
   FROM_ACQUIRED set if inside solid rock (or in water on Plane of Water) */
#define BFlying u.uprops[FLYING].blocked
#define Flying                                                      \
    ((HFlying || EFlying || (u.usteed && is_flyer(u.usteed->data))) \
     && !BFlying)
/* May touch surface; does not override any others */

#define HWwalking u.uprops[WWALKING].intrinsic
#define EWwalking u.uprops[WWALKING].extrinsic
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
    (Magical_breathing || breathless(youmonst.data))

#define Underwater (u.uinwater)

#define Survives_without_air (!Underwater && Breathless) || (Underwater && (Amphibious || Swimming))

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

#define HHalf_spell_damage u.uprops[HALF_SPDAM].intrinsic
#define EHalf_spell_damage u.uprops[HALF_SPDAM].extrinsic
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

#define HHalf_physical_damage u.uprops[HALF_PHDAM].intrinsic
#define EHalf_physical_damage u.uprops[HALF_PHDAM].extrinsic
#define Half_physical_damage (HHalf_physical_damage || EHalf_physical_damage)

#define HHalf_physical_damage_against_undead_and_demons u.uprops[HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS].intrinsic
#define EHalf_physical_damage_against_undead_and_demons u.uprops[HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS].extrinsic
#define Half_physical_damage_against_undead_and_demons (HHalf_physical_damage_against_undead_and_demons || EHalf_physical_damage_against_undead_and_demons)

#define HRegeneration u.uprops[REGENERATION].intrinsic
#define ERegeneration u.uprops[REGENERATION].extrinsic
#define Regeneration (HRegeneration || ERegeneration)

#define HEnergy_regeneration u.uprops[ENERGY_REGENERATION].intrinsic
#define EEnergy_regeneration u.uprops[ENERGY_REGENERATION].extrinsic
#define Energy_regeneration (HEnergy_regeneration || EEnergy_regeneration)

#define HProtection u.uprops[PROTECTION].intrinsic
#define EProtection u.uprops[PROTECTION].extrinsic
#define Protection (HProtection || EProtection)

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

#define HFast u.uprops[FAST].intrinsic
#define EFast u.uprops[FAST].extrinsic
#define Fast (HFast || EFast)
#define Very_fast ((HFast & ~INTRINSIC) || EFast)

#define HReflecting u.uprops[REFLECTING].intrinsic
#define EReflecting u.uprops[REFLECTING].extrinsic
#define Reflecting (HReflecting || EReflecting)

#define Free_action u.uprops[FREE_ACTION].extrinsic /* [Tom] */

#define Fixed_abil u.uprops[FIXED_ABIL].extrinsic /* KMH */

#define HLifesaved u.uprops[LIFESAVED].intrinsic
#define ELifesaved u.uprops[LIFESAVED].extrinsic
#define Lifesaved (HLifesaved || ELifesaved)

/*
 * Some pseudo-properties.
 */

/* unconscious() includes u.usleep but not is_fainted(); the multi test is
   redundant but allows the function calls to be skipped most of the time */
#define Unaware (multi < 0 && (unconscious() || is_fainted()))

#define Hate_silver (u.ulycn >= LOW_PM || hates_silver(youmonst.data))

#endif /* YOUPROP_H */
