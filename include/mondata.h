/* GnollHack 4.0	mondata.h	$NHDT-Date: 1550524558 2019/02/18 21:15:58 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.37 $ */
/* Copyright (c) 1989 Mike Threepoint				  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MONDATA_H
#define MONDATA_H

#define verysmall(ptr) ((ptr)->msize < MZ_SMALL)
#define bigmonst(ptr) ((ptr)->msize >= MZ_LARGE)
#define hugemonst(ptr) ((ptr)->msize >= MZ_HUGE)

#define is_vampshifter(mon)                                      \
    ((mon)->cham == PM_VAMPIRE || (mon)->cham == PM_VAMPIRE_LORD || (mon)->cham == PM_VAMPIRE_MAGE \
     || (mon)->cham == PM_VLAD_THE_IMPALER)

/* M1-4 flags*/
#define is_flyer(ptr) (((ptr)->mflags1 & M1_FLY) != 0L)
#define is_floater(ptr) ((ptr)->mlet == S_EYE || (ptr)->mlet == S_LIGHT)
#define is_clinger(ptr) (((ptr)->mflags1 & M1_CLING) != 0L)
#define is_swimmer(ptr) (((ptr)->mflags1 & M1_SWIM) != 0L)
#define has_innate_breathless(ptr) (((ptr)->mflags1 & M1_BREATHLESS) != 0L)
#define amphibious(ptr) \
    (((ptr)->mflags1 & (M1_AMPHIBIOUS | M1_BREATHLESS)) != 0L)
#define passes_walls(ptr) (((ptr)->mflags1 & M1_WALLWALK) != 0L)
#define amorphous(ptr) (((ptr)->mflags1 & M1_AMORPHOUS) != 0L)
#define tunnels(ptr) (((ptr)->mflags1 & M1_TUNNEL) != 0L)
#define needspick(ptr) (((ptr)->mflags1 & M1_NEEDPICK) != 0L)
#define hides_under(ptr) (((ptr)->mflags1 & M1_CONCEAL) != 0L)
#define is_hider(ptr) (((ptr)->mflags1 & M1_HIDE) != 0L)
#define haseyes(ptr) (((ptr)->mflags1 & M1_NOEYES) == 0L)
#define eyecount(ptr)                                         \
    (!haseyes(ptr) ? 0 : ((ptr) == &mons[PM_CYCLOPS]          \
                          || (ptr) == &mons[PM_FLOATING_EYE]) \
                             ? 1                              \
                             : 2)
#define nolimbs(ptr) (((ptr)->mflags1 & M1_NOLIMBS) == M1_NOLIMBS) /* two bits */
#define nohands(ptr) (((ptr)->mflags1 & M1_NOHANDS) != 0L) /* also covers no limbs because of the bit in M1_NOLIMBS  */
#define notake(ptr) (((ptr)->mflags1 & M1_NOTAKE) != 0L)
#define has_head(ptr) (((ptr)->mflags1 & M1_NOHEAD) == 0L)
#define has_horns(ptr) (num_horns(ptr) > 0)
#define is_whirly(ptr) \
    ((ptr)->mlet == S_VORTEX || (ptr) == &mons[PM_AIR_ELEMENTAL])
#define flaming(ptr)                                                     \
    ((ptr) == &mons[PM_FIRE_VORTEX] || (ptr) == &mons[PM_FLAMING_SPHERE] \
     || (ptr) == &mons[PM_FIRE_ELEMENTAL] || (ptr) == &mons[PM_SALAMANDER])
#define is_silent(ptr) ((ptr)->msound == MS_SILENT)
#define unsolid(ptr) (((ptr)->mflags1 & M1_UNSOLID) != 0L)
#define mindless(ptr) (((ptr)->mflags1 & M1_MINDLESS) != 0L)
#define humanoid(ptr) (((ptr)->mflags1 & M1_HUMANOID) != 0L)
#define is_animal(ptr) (((ptr)->mflags1 & M1_ANIMAL) != 0L)
#define slithy(ptr) (((ptr)->mflags1 & M1_SLITHY) != 0L)
#define is_wooden(ptr) ((ptr) == &mons[PM_WOOD_GOLEM])
#define thick_skinned(ptr) (((ptr)->mflags1 & M1_THICK_HIDE) != 0L)
#define hug_throttles(ptr) ((ptr) == &mons[PM_ROPE_GOLEM])
#define lays_eggs(ptr) (((ptr)->mflags1 & M1_OVIPAROUS) != 0L)
#define eggs_in_water(ptr) \
    (lays_eggs(ptr) && (ptr)->mlet == S_EEL && is_swimmer(ptr))
#define carnivorous(ptr) (((ptr)->mflags1 & M1_CARNIVORE) != 0L)
#define herbivorous(ptr) (((ptr)->mflags1 & M1_HERBIVORE) != 0L)
#define metallivorous(ptr) (((ptr)->mflags1 & M1_METALLIVORE) != 0L)

#define polyok(ptr) (((ptr)->mflags2 & M2_NOPOLY) == 0L)
#define is_shapeshifter(ptr) (((ptr)->mflags2 & M2_SHAPESHIFTER) != 0L)
#define is_undead(ptr) (((ptr)->mflags2 & M2_UNDEAD) != 0L)
#define is_were(ptr) (((ptr)->mflags2 & M2_WERE) != 0L)
#define is_elf(ptr) (((ptr)->mflags2 & M2_ELF) != 0L)
#define is_dwarf(ptr) (((ptr)->mflags2 & M2_DWARF) != 0L)
#define is_gnoll(ptr) (((ptr)->mflags2 & M2_GNOLL) != 0L)
#define is_orc(ptr) (((ptr)->mflags2 & M2_ORC) != 0L)
#define is_gnome(ptr) (((ptr)->mflags2 & M2_GNOME) != 0L)
#define is_human(ptr) (((ptr)->mflags2 & M2_HUMAN) != 0L)
#define your_race(ptr) (((ptr)->mflags2 & urace.selfmask) != 0L)
#define is_bat(ptr)                                         \
    ((ptr) == &mons[PM_BAT] || (ptr) == &mons[PM_GIANT_BAT] \
     || (ptr) == &mons[PM_VAMPIRE_BAT] || (ptr) == &mons[PM_HELL_BAT])
#define is_bird(ptr) ((ptr)->mlet == S_BAT && !is_bat(ptr))
#define is_giant(ptr) (((ptr)->mflags2 & M2_GIANT) != 0L)
#define is_golem(ptr) ((ptr)->mlet == S_GOLEM)
#define is_treant(ptr) ((ptr)->mlet == S_TREANT)
#define is_dragon(ptr) ((ptr)->mlet == S_DRAGON)
#define is_domestic(ptr) (((ptr)->mflags2 & M2_DOMESTIC) != 0L)
#define is_demon(ptr) (((ptr)->mflags2 & M2_DEMON) != 0L)
#define is_angel(ptr) ((ptr)->mlet == S_ANGEL)
#define is_mercenary(ptr) (((ptr)->mflags2 & M2_MERC) != 0L)
#define is_modron(ptr) (((ptr)->mflags2 & M2_MODRON) != 0L)
#define is_male(ptr) (((ptr)->mflags2 & M2_MALE) != 0L)
#define is_female(ptr) (((ptr)->mflags2 & M2_FEMALE) != 0L)
#define is_neuter(ptr) (((ptr)->mflags2 & M2_NEUTER) != 0L)
#define is_wanderer(ptr) (((ptr)->mflags2 & M2_WANDER) != 0L)
#define always_hostile(ptr) (((ptr)->mflags2 & M2_HOSTILE) != 0L)
#define always_peaceful(ptr) (((ptr)->mflags2 & M2_PEACEFUL) != 0L)
#define race_hostile(ptr) (((ptr)->mflags2 & urace.hatemask) != 0L)
#define race_peaceful(ptr) (((ptr)->mflags2 & urace.lovemask) != 0L)
#define extra_nasty(ptr) (((ptr)->mflags2 & M2_NASTY) != 0L)
/*#define strongmonst(ptr) (((ptr)->mflags2 & M2_STRONG) != 0L)*/
#define strongmonst(ptr) (((ptr)->str) >= 17)
#define can_breathe(ptr) attacktype(ptr, AT_BREA)
#define cantwield(ptr) (nohands(ptr) || verysmall(ptr))
#define could_twoweap(ptr) ((ptr)->mattk[1].aatyp == AT_WEAP)
#define cantweararm(ptr) (breakarm(ptr) || sliparm(ptr))
#define throws_rocks(ptr) (((ptr)->mflags2 & M2_ROCKTHROW) != 0L)
#define type_is_pname(ptr) (((ptr)->mflags2 & M2_PNAME) != 0L)
#define is_lord(ptr) (((ptr)->mflags2 & M2_LORD) != 0L)
#define is_prince(ptr) (((ptr)->mflags2 & M2_PRINCE) != 0L)
#define is_ndemon(ptr) \
    (is_demon(ptr) && (((ptr)->mflags2 & (M2_LORD | M2_PRINCE)) == 0L))
#define is_dlord(ptr) (is_demon(ptr) && is_lord(ptr))
#define is_dprince(ptr) (is_demon(ptr) && is_prince(ptr))
#define is_minion(ptr) (((ptr)->mflags2 & M2_MINION) != 0L)
#define is_lminion(mon) \
    (is_minion((mon)->data) && mon_aligntyp(mon) == A_LAWFUL)
#define likes_gold(ptr) (((ptr)->mflags2 & M2_GREEDY) != 0L)
#define likes_gems(ptr) (((ptr)->mflags2 & M2_JEWELS) != 0L)
#define likes_objs(ptr) (((ptr)->mflags2 & M2_COLLECT) != 0L || is_armed(ptr))
#define likes_magic(ptr) (((ptr)->mflags2 & M2_MAGIC) != 0L)

#define webmaker(ptr) (((ptr)->mflags3 & M3_WEBMAKER) != 0L)
#define is_unicorn(ptr) ((ptr)->mlet == S_UNICORN && likes_gems(ptr))
#define is_longworm(ptr)                                                   \
    (((ptr) == &mons[PM_BABY_LONG_WORM]) || ((ptr) == &mons[PM_LONG_WORM]) \
     || ((ptr) == &mons[PM_LONG_WORM_TAIL]))
#define is_covetous(ptr) (((ptr)->mflags3 & M3_COVETOUS))
#define wants_bell(ptr) (((ptr)->mflags3 & M3_WANTSBELL))
#define infravision(ptr) (((ptr)->mflags3 & M3_INFRAVISION))
#define infravisible(ptr) (((ptr)->mflags3 & M3_INFRAVISIBLE))
#define is_displacer(ptr) (((ptr)->mflags3 & M3_DISPLACES) != 0L)
#define noncorporeal(ptr) (((ptr)->mflags3 & M3_NONCORPOREAL) != 0L)   //((ptr)->mlet == S_GHOST)
#define is_nonliving(ptr) (((ptr)->mflags3 & M3_NONLIVING) != 0L)
#define is_multiweaponmonster(ptr) (((ptr)->mflags3 & M3_MULTIWEAPON) != 0L)
#define leaves_corpses_randomly(ptr) (((ptr)->mflags3 & M3_RANDOM_CORPSE) != 0L)
#define corpse_crumbles_to_dust(ptr) (((ptr)->mflags3 & M3_CORPSE_CRUMBLES_TO_DUST) != 0L)
#define is_speaking_monster(ptr) (((ptr)->mflags3 & M3_SPEAKING) != 0L)
#define is_constrictor(ptr) (((ptr)->mflags3 & M3_CONSTRICTOR) != 0L)
#define hug_requires_two_previous_attacks(ptr) (((ptr)->mflags3 & M3_HUG_HITS_IF_TWO_FIRST_ATTACKS_SUCCEEDED) != 0L)
#define knows_pits_and_holes(ptr) \
    (((ptr)->mflags3 & M3_KNOWS_PITS_AND_HOLES) != 0)
#define knows_traps(ptr) \
    (((ptr)->mflags3 & M3_KNOWS_TRAPS) != 0)
#define is_priest(ptr) (((ptr)->mflags3 & M3_PRIEST) != 0L)

#define is_brave(ptr) (((ptr)->mflags4 & M4_BRAVE) != 0L)
#define is_fearless(ptr) (((ptr)->mflags4 & M4_FEARLESS) != 0L)
#define has_bloodlust(ptr) (((ptr)->mflags4 & M4_BLOODLUST) != 0L)
#define does_disregard_own_health(ptr) (is_brave(ptr) || mindless(ptr))
#define does_disregard_enemy_strength(ptr) (is_fearless(ptr) || mindless(ptr))
#define mon_has_bloodlust(mtmp) (has_bloodlust((mtmp)->data) || (mtmp)->hasbloodlust)
#define mon_disregards_own_health(mtmp) (does_disregard_own_health((mtmp)->data) || (mtmp)->disregards_own_health)
#define mon_disregards_enemy_strength(mtmp) (does_disregard_enemy_strength((mtmp)->data) || (mtmp)->disregards_enemy_strength)
#define does_split_upon_hit(ptr) (((ptr)->mflags4 & M4_SPLITS_UPON_HIT) != 0L)
#define is_vegetarian_food(ptr) (((ptr)->mflags4 & M4_VEGETARIAN_FOOD) != 0L)
#define is_vegan_food(ptr) (((ptr)->mflags4 & M4_VEGAN_FOOD) != 0L)
#define alternative_passive_defense_text(ptr) \
    (((ptr)->mflags4 & M4_ALTERNATIVE_PASSIVE_DEFENSE_TEXT) != 0)


/* combinations */
#define is_not_living(ptr) \
    (is_undead(ptr) || is_nonliving(ptr))
#define is_living(ptr) !is_not_living(ptr)
#define slimeproof(ptr) \
    ((ptr) == &mons[PM_GREEN_SLIME] || flaming(ptr) || noncorporeal(ptr))


/* Resistances and properties */
#define has_innate(ptr, bit) \
    (((ptr)->mresists & (bit)) != 0)
#define has_property(mon, propindex) \
	((mon)->mprops[(propindex)] != 0)
#define has_innate_or_property(mon, propindex)\
	(has_innate((mon)->data, prop_to_innate(propindex)) || has_property(mon, propindex))


/* innates */
#define has_innate_invisibility(ptr) \
	has_innate(ptr, MR_INVISIBLE)

#define has_innate_see_invisible(ptr) \
	has_innate(ptr, MR_SEE_INVISIBLE)

#define has_innate_regeneration(ptr) \
	has_innate(ptr, MR_REGENERATION)

#define has_innate_teleportation(ptr) \
	has_innate(ptr, MR_TELEPORT)

#define has_innate_teleport_control(ptr) \
	has_innate(ptr, MR_TELEPORT_CONTROL)

#define has_innate_blind_telepathy(ptr)   \
	has_innate(ptr, MR_BLIND_TELEPATHY)

#define has_innate_telepathy(ptr) \
	has_innate(ptr, MR_TELEPATHY)


/* general properties */
#define has_telepathy(mon)   \
     (has_innate_telepathy((mon)->data) || has_property(mon, TELEPAT))

#define has_blind_telepathy(mon)   \
     (has_innate_blind_telepathy((mon)->data) || has_property(mon, BLIND_TELEPAT))

#define has_blocks_invisibility(mon) \
	has_property(mon, BLOCKS_INVISIBILITY)

#define has_slowed(mon) \
	has_property(mon, SLOWED)

#define has_slimed(mon) \
	has_property(mon, SLIMED)

#define has_stoned(mon) \
	has_property(mon, STONED)

#define has_magical_breathing(mon) \
	(has_property(mon, MAGICAL_BREATHING))

#define is_breathless(mon) \
	(has_magical_breathing(mon) || has_innate_breathless((mon)->data))


#define mon_survives_without_air  (is_breathless(mon))

#define has_strangled(mon) \
	has_property(mon, STRANGLED)

#define has_airless_environment(mon) \
	has_property(mon, AIRLESS_ENVIRONMENT)

#define is_suffocating(mon) \
	(has_airless_environment(mon) && !has_magical_breathing(mon))

#define is_slow(mon) \
	(has_slowed(mon) || has_slimed(mon) || has_stoned(mon))

#define has_cancelled(mon) \
	has_property(mon, CANCELLED)

#define has_half_magic_resistance(mon) \
	has_property(mon, HALF_MAGIC_RES)

#define has_no_magic_resistance(mon) \
	has_property(mon, NO_MAGIC_RES)

#define has_paralyzed(mon) \
	has_property(mon, PARALYZED)

#define has_free_action(mon) \
	has_innate_or_property(mon, FREE_ACTION)

#define is_paralyzed(mon) \
	(has_paralyzed(mon) && !has_free_action(mon) && !is_undead((mon)->data) && !is_vampshifter(mon))

#define has_sleeping(mon) \
	has_property(mon, SLEEPING)

#define is_sleeping(mon) \
	(has_sleeping(mon) || (mon)->msleeping)

#define mon_can_move(mon) \
	((mon)->mcanmove && !is_sleeping(mon) && !is_paralyzed(mon))

#define has_summon_forbidden(mon) \
	has_property(mon, SUMMON_FORBIDDEN)

#define has_charmed(mon) \
	has_property(mon, CHARMED)

#define has_stunned(mon) \
	has_property(mon, STUNNED)

#define is_stunned(mon) \
	(has_stunned(mon))

#define has_confused(mon) \
	has_property(mon, CONFUSION)

#define is_confused(mon) \
	(has_confused(mon))

#define has_blinded(mon) \
	has_property(mon, BLINDED)

#define is_blinded(mon) \
	has_blinded(mon)

#define mon_can_see(mon) \
	(!is_blinded(mon))

#define has_hallucination(mon) \
	has_property(mon, HALLUC)

#define has_hallucination_resistance(mon) \
	has_innate_or_property(mon, HALLUC_RES)

#define is_hallucinating(mon) \
	(has_hallucination(mon) && !has_hallucination_resistance(mon))


#define has_sick(mon) \
	has_property(mon, SICK)

#define has_sickness_resistance(mon) \
	has_innate_or_property(mon, SICK_RES)

#define is_sick(mon) \
	(has_sick(mon) && !has_sickness_resistance(mon) && !is_undead((mon)->data) && !is_demon((mon)->data) && !is_vampshifter(mon))


#define has_charmed(mon) \
	has_property(mon, CHARMED)

#define has_charm_resistance(mon) \
	(has_innate_or_property(mon, CHARM_RES))

#define is_charmed(mon) \
	(has_charmed(mon) && !has_charm_resistance(mon))

#define has_fearful(mon) \
	has_property(mon, FEARFUL)

#define has_fear_resistance(mon) \
	(has_innate_or_property(mon, FEAR_RES))

#define is_fearful(mon) \
	(has_fearful(mon) && !has_fear_resistance(mon) && !is_undead((mon)->data) && !mindless((mon)->data) && !is_vampshifter(mon))

#define has_drain_resistance(mon) \
	(has_innate_or_property(mon, DRAIN_RES))

#define has_death_resistance(mon) \
	(has_innate_or_property(mon, DEATH_RES))


#define has_levitation(mon) \
	has_property(mon, LEVITATION)

#define has_flying(mon) \
	has_innate_or_property(mon, FLYING)

#define is_flying(mon) \
	(has_flying(mon))

#define is_levitating(mon) \
	(has_levitation(mon) && !has_flying(mon))



#define has_invisibility(mon) \
	((has_innate_invisibility(mon->data) || has_property(mon, INVISIBILITY)))

#define is_invisible(mon) \
	(has_invisibility(mon) && !has_blocks_invisibility(mon))

#define has_fast(mon) \
	has_property(mon, FAST)

#define has_very_fast(mon) \
	has_property(mon, VERY_FAST)

#define is_very_fast(mon) \
	(has_very_fast(mon) && !is_slow(mon))

#define is_fast(mon) \
	(has_fast(mon) && !is_slow(mon) && !is_very_fast(mon))




#define resists_fire(mon) \
    (has_innate((mon)->data, MR_FIRE) || has_property(mon, FIRE_RES))
#define resists_cold(mon) \
    (has_innate((mon)->data, MR_COLD) || has_property(mon, COLD_RES))
#define resists_sleep(mon) \
    (has_innate((mon)->data, MR_SLEEP) || has_property(mon, SLEEP_RES))
#define resists_disint(mon) \
    (has_innate((mon)->data, MR_DISINT) || has_property(mon, DISINT_RES))
#define resists_elec(mon) \
    (has_innate((mon)->data, MR_ELEC) || has_property(mon, SHOCK_RES))
#define resists_death(mon) \
    (has_innate((mon)->data, MR_DEATH) || has_property(mon, DEATH_RES) || is_not_living((mon)->data) || is_demon((mon)->data) || is_vampshifter(mon))
#define resists_lycanthropy(mon) \
    (has_innate((mon)->data, MR_LYCANTHROPY) || has_property(mon, LYCANTHROPY_RES) ||  is_not_living((mon)->data) || is_demon((mon)->data) || is_vampshifter(mon))
#define resists_poison(mon) \
    (has_innate((mon)->data, MR_POISON) || has_property(mon, POISON_RES))
#define resists_acid(mon) \
    (has_innate((mon)->data, MR_ACID) || has_property(mon, ACID_RES))
#define resists_ston(mon) \
    (has_innate((mon)->data, MR_STONE) || has_property(mon, STONE_RES))
#define resists_magicmissile(mon) \
    has_innate((mon)->data, MR_MAGICMISSILE)
#define resists_magic(mon) \
    (has_innate((mon)->data, MR_MAGIC) || has_property(mon, ANTIMAGIC))
#define resists_charm(mon) \
    (has_innate((mon)->data, MR_CHARM) || has_property(mon, CHARM_RES) || is_undead((mon)->data) || mindless((mon)->data))
#define resists_fear(mon) \
    (has_innate((mon)->data, MR_FEAR) || has_property(mon, FEAR_RES) || is_undead((mon)->data) || mindless((mon)->data) || is_vampshifter(mon))
#define is_reflecting(mon) \
	(has_innate((mon)->data, MR_REFLECTING) || has_property(mon, REFLECTING))
#define resists_drain(mon) \
    (has_innate((mon)->data, MR_DRAIN) || has_property(mon, DRAIN_RES) ||  is_not_living((mon)->data) || is_were((mon)->data) || is_demon((mon)->data) || is_vampshifter(mon))
#define resists_flash(mon) \
    (has_innate((mon)->data, MR_FLASH) || has_property(mon, FLASH_RES) || is_blinded(mon) || !haseyes((mon)->data) )
#define resists_sickness(mon) \
    (has_innate((mon)->data, MR_SICK) || has_property(mon, SICK_RES) ||  is_not_living((mon)->data) || is_demon((mon)->data) || is_vampshifter(mon) )
#define resists_paralysis(mon) \
    (has_innate((mon)->data, MR_FREE_ACTION) || has_property(mon, FREE_ACTION) ||  is_not_living((mon)->data) || is_vampshifter(mon))


/* Conveyed propreties */
#define conveys_strength(ptr) \
    (((ptr)->mconveys & MC_STRENGTH) != 0)
#define conveys_dexterity(ptr) \
    (((ptr)->mconveys & MC_DEXTERITY) != 0)
#define conveys_constitution(ptr) \
    (((ptr)->mconveys & MC_CONSTITUTION) != 0)
#define conveys_intelligence(ptr) \
    (((ptr)->mconveys & MC_INTELLIGENCE) != 0)
#define conveys_wisdom(ptr) \
    (((ptr)->mconveys & MC_WISDOM) != 0)
#define conveys_charisma(ptr) \
    (((ptr)->mconveys & MC_CHARISMA) != 0)


#define has_see_invisible(mon) \
	(has_innate_see_invisible((mon)->data) || has_property(mon, SEE_INVISIBLE))
#define has_regeneration(mon) \
	(has_innate_regeneration((mon)->data) || has_property(mon, REGENERATION))
#define has_teleportation(mon) \
	(has_innate_teleportation((mon)->data) || has_property(mon, TELEPORT))
#define has_teleport_control(mon) \
	(has_innate_teleport_control((mon)->data) || has_property(mon, TELEPORT_CONTROL))
#define has_blind_telepathy(mon) \
	(has_innate_blind_telepathy((mon)->data) || has_property(mon, BLIND_TELEPAT))
#define has_telepathy(mon) \
	(has_innate_telepathy((mon)->data) || has_property(mon, TELEPAT))

#define acidic(ptr) (((ptr)->mconveys & MC_ACIDIC_CORPSE) != 0L)
#define poisonous(ptr) (((ptr)->mconveys & MC_POISONOUS_CORPSE) != 0L)
#define hallucinating_corpse(ptr) (((ptr)->mconveys & MC_HALLUCINATING_CORPSE) != 0L)

#define is_armed(ptr) attacktype(ptr, AT_WEAP)


#define is_mplayer(ptr) \
    (((ptr) >= &mons[PM_ARCHEOLOGIST]) && ((ptr) <= &mons[PM_WIZARD]))
#define is_watch(ptr) \
    ((ptr) == &mons[PM_WATCHMAN] || (ptr) == &mons[PM_WATCH_CAPTAIN])
#define is_rider(ptr)                                      \
    ((ptr) == &mons[PM_DEATH] || (ptr) == &mons[PM_FAMINE] \
     || (ptr) == &mons[PM_PESTILENCE])
#define is_placeholder(ptr)                             \
    ((ptr) == &mons[PM_ORC] || (ptr) == &mons[PM_GIANT] \
     || (ptr) == &mons[PM_ELF] || (ptr) == &mons[PM_HUMAN])
/* return TRUE if the monster tends to revive */
#define is_reviver(ptr) (((ptr)->mflags3 & M3_REVIVES_FROM_DEAD) != 0)
#define is_reviver_corpse(mnum) ((mons[(mnum)].mflags3 & M3_REVIVES_FROM_DEAD) != 0)
//(is_rider(ptr) || (ptr) == &mons[PM_PHOENIX] || (ptr)->mlet == S_TROLL)
/* monsters whose corpses and statues need special handling;
   note that high priests and the Wizard of Yendor are flagged
   as unique even though they really aren't; that's ok here */
#define unique_corpstat(ptr) (((ptr)->geno & G_UNIQ) != 0)

/* this returns the light's range, or 0 if none; if we add more light emitting
   monsters, we'll likely have to add a new light range field to mons[] */
#define emits_light(ptr)                                          \
    (((ptr)->mlet == S_LIGHT || (ptr) == &mons[PM_FLAMING_SPHERE] \
      || (ptr) == &mons[PM_SHOCKING_SPHERE]                       \
      || (ptr) == &mons[PM_FIRE_VORTEX])                          \
         ? 1                                                      \
         : ((ptr) == &mons[PM_FIRE_ELEMENTAL]) ? 1 : 0)
/*	[note: the light ranges above were reduced to 1 for performance...] */
#define likes_lava(ptr) \
    (ptr == &mons[PM_FIRE_ELEMENTAL] || ptr == &mons[PM_SALAMANDER])

/* could probably add more */
#define likes_fire(ptr)                                                  \
    ((ptr) == &mons[PM_FIRE_VORTEX] || (ptr) == &mons[PM_FLAMING_SPHERE] \
     || likes_lava(ptr))

#define touch_petrifies(ptr) \
    ((ptr) == &mons[PM_COCKATRICE] || (ptr) == &mons[PM_CHICKATRICE])

#define is_mind_flayer(ptr) \
    ((ptr) == &mons[PM_MIND_FLAYER] || (ptr) == &mons[PM_MASTER_MIND_FLAYER] || (ptr) == &mons[PM_ILLITHILICH])

#define is_mimic(ptr) \
    ((ptr->mlet == S_MIMIC)


#define is_vampire(ptr) ((ptr)->mlet == S_VAMPIRE)
#define hates_light(ptr) ((ptr) == &mons[PM_GREMLIN])


/* cursed items are good for some evil creatures */
#define cursed_items_are_positive(ptr) \
	(is_undead(ptr) || is_demon(ptr))

/* no corpse (ie, blank scrolls) if killed by fire */
#define completelyburns(ptr) \
    ((ptr) == &mons[PM_PAPER_GOLEM] || (ptr) == &mons[PM_STRAW_GOLEM])

/* Used for conduct with corpses, tins, and digestion attacks */
/* G_NOCORPSE monsters might still be swallowed as a purple worm */
/* Maybe someday this could be in mflags... */
#define vegan(ptr)                                                 \
    (is_vegan_food(ptr) || noncorporeal(ptr))

#define vegetarian(ptr) \
    (vegan(ptr) || is_vegetarian_food(ptr))

#define nonrotting_corpse(mnum) \
    ((mons[(mnum)].mflags4 & M4_NONROTTING_CORPSE) != 0)

#define nonrotting_corpse_ptr(ptr) \
    (((ptr)->mflags4 & M4_NONROTTING_CORPSE) != 0)

#define has_pitwalk(ptr) \
    (((ptr)->mflags4 & M4_PITWALK) != 0)

/* Overall resistances */
#define resists_drli(mon) \
  (is_undead((mon)->data) || is_demon((mon)->data) || is_were((mon)->data) \
	|| ((mon) == &youmonst && u.ulycn >= LOW_PM) || is_vampshifter(mon) || resists_drain(mon))

#define resists_blnd(mon) \
	(((mon) == &youmonst && (Blind || Unaware)) || (is_blinded(mon) \
	|| !haseyes((mon)->data) || is_sleeping(mon) || resists_flash(mon)))


/* monkeys are tameable via bananas but not pacifiable via food,
   otherwise their theft attack could be nullified too easily;
   dogs and cats can be tamed by anything they like to eat and are
   pacified by any other food;
   horses can be tamed by always-veggy food or lichen corpses but
   not tamed or pacified by other corpses or tins of veggy critters */
#define befriend_with_obj(ptr, obj) \
    (((ptr) == &mons[PM_MONKEY] || (ptr) == &mons[PM_APE])               \
     ? (obj)->otyp == BANANA                                             \
     : (is_domestic(ptr) && (obj)->oclass == FOOD_CLASS                  \
        && ((ptr)->mlet != S_UNICORN                                     \
            || objects[(obj)->otyp].oc_material == MAT_VEGGY                 \
            || ((obj)->otyp == CORPSE && (((obj)->corpsenm == PM_LICHEN) || ((obj)->corpsenm == PM_WHITE_LICHEN)  || ((obj)->corpsenm == PM_BLACK_LICHEN))   ))))

#endif /* MONDATA_H */
