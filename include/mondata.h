/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

/* GnollHack 4.0    mondata.h    $NHDT-Date: 1550524558 2019/02/18 21:15:58 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.37 $ */
/* Copyright (c) 1989 Mike Threepoint                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MONDATA_H
#define MONDATA_H

#define verysmall(ptr) ((ptr)->msize < MZ_SMALL)
#define bigmonst(ptr) ((ptr)->msize >= MZ_LARGE)
#define hugemonst(ptr) ((ptr)->msize >= MZ_HUGE)

#define is_vampshifter(mon)                                      \
    ((mon)->cham >= LOW_PM && mons[(mon)->cham].mlet == S_VAMPIRE && (mons[(mon)->cham].mflags2 & M2_SHAPESHIFTER) != 0)

/* M1-4 flags*/
#define is_flyer(ptr) (((ptr)->mflags1 & M1_FLY) != 0L)
#define is_floater(ptr) (((ptr)->mflags1 & M1_FLOATER) != 0L)
#define is_telekinetic_operator(ptr) (((ptr)->mflags1 & M1_TELEKINETIC_OPERATOR) != 0L)
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
                             : (ptr) == &mons[PM_GAZER] || (ptr) == &mons[PM_ELDER_GAZER] ? 7    \
                             : (ptr) == &mons[PM_DEATH_GAZER] ? 4    \
                             : 2 * (ptr)->heads)
#define nolimbs(ptr) (((ptr)->mflags1 & M1_NOLIMBS) == M1_NOLIMBS) /* two bits */
#define nohands(ptr) (((ptr)->mflags1 & M1_NOHANDS) != 0L) /* also covers no limbs because of the bit in M1_NOLIMBS  */
#define notake(ptr) (((ptr)->mflags1 & M1_NOTAKE) != 0L)
#define can_operate_objects(ptr) (!(nohands(ptr) || verysmall(ptr)) || is_telekinetic_operator(ptr))

#define has_head(ptr) ((ptr)->heads > 0)
#define has_neck(ptr) (((ptr)->mflags1 & M1_NONECK) == 0L && has_head(ptr))
#define has_place_to_put_helmet_on(ptr) has_head(ptr)
#define has_horns(ptr) (num_horns(ptr) > 0)
#define is_silent(ptr) ((ptr)->msound == MS_SILENT)
#define unsolid(ptr) (((ptr)->mflags1 & M1_UNSOLID) != 0L)
#define mindless(ptr) (((ptr)->mflags1 & M1_MINDLESS) != 0L)
#define humanoid(ptr) (((ptr)->mflags1 & M1_HUMANOID) != 0L)
#define is_animal(ptr) (((ptr)->mflags1 & M1_ANIMAL) != 0L)
#define slithy(ptr) (((ptr)->mflags1 & M1_SLITHY) != 0L)
#define thick_skinned(ptr) (((ptr)->mflags1 & M1_THICK_HIDE) != 0L)
#define lays_eggs(ptr) (((ptr)->mflags1 & M1_OVIPAROUS) != 0L)
#define eggs_in_water(ptr) \
    (lays_eggs(ptr) && (ptr)->mlet == S_EEL && is_swimmer(ptr))
#define carnivorous(ptr) (((ptr)->mflags1 & M1_CARNIVORE) != 0L)
#define herbivorous(ptr) (((ptr)->mflags1 & M1_HERBIVORE) != 0L)
#define metallivorous(ptr) (((ptr)->mflags1 & M1_METALLIVORE) != 0L)
#define rust_causing_and_ironvorous(ptr) (((ptr)->mflags1 & M1_RUST_CAUSING_IRONVORE) != 0L)
#define is_steed(ptr) (((ptr)->mflags1 & M1_STEED) != 0L)

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
#define is_giant(ptr) (((ptr)->mflags2 & M2_GIANT) != 0L)
#define is_golem(ptr) ((ptr)->mlet == S_GOLEM)
#define is_treant(ptr) ((ptr)->mlet == S_TREANT)
#define is_dragon(ptr) ((ptr)->mlet == S_DRAGON)
#define is_domestic(ptr) (((ptr)->mflags2 & M2_DOMESTIC) != 0L)
#define is_demon(ptr) (((ptr)->mflags2 & M2_DEMON) != 0L)
#define is_angel(ptr) (((ptr)->mflags2 & M2_ANGEL) != 0L)
#define is_modron(ptr) (((ptr)->mflags2 & M2_MODRON) != 0L)
#define is_tentacled_one(ptr) (((ptr)->mflags2 & M2_TENTACLED_ONE) != 0)
#define is_mimic(ptr) (((ptr)->mflags2 & M2_MIMIC) != 0)
#define is_elemental(ptr) (((ptr)->mflags2 & M2_ELEMENTAL) != 0)
#define is_cockatrice(ptr) (((ptr)->mflags2 & M2_COCKATRICE) != 0L)
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
#define throws_rocks(ptr) (((ptr)->mflags1 & M1_ROCKTHROW) != 0L)
#define is_mname_proper_name(ptr) (((ptr)->mflags4 & M4_PROPER_NAME) != 0L)
#define is_minion(ptr) (((ptr)->mflags2 & M2_MINION) != 0L)
#define is_lminion(mon) \
    (is_minion((mon)->data) && mon_aligntyp(mon) == A_LAWFUL)
#define likes_gold(ptr) (((ptr)->mflags2 & M2_GREEDY) != 0L)
#define likes_gems(ptr) (((ptr)->mflags2 & M2_JEWELS) != 0L)
#define likes_objs(ptr) (((ptr)->mflags2 & M2_COLLECT) != 0L || is_armed(ptr))
#define likes_magic(ptr) (((ptr)->mflags2 & M2_MAGIC) != 0L)

#define webmaker(ptr) (((ptr)->mflags3 & M3_WEBMAKER) != 0L)
#define is_unicorn(ptr) ((ptr)->mlet == S_UNICORN && likes_gems(ptr))
#define is_long_worm(ptr) (((ptr)->mflags6 & M6_LONG_WORM))
#define is_long_worm_with_tail(ptr) (((ptr)->mflags6 & M6_TAILED_LONG_WORM))
#define is_long_worm_tail(ptr) (((ptr)->mflags6 & M6_LONG_WORM_TAIL))
#define is_purple_worm(ptr) (((ptr)->mflags6 & M6_PURPLE_WORM))
#define is_covetous(ptr) (((ptr)->mflags3 & M3_COVETOUS))
#define wants_bell(ptr) (((ptr)->mflags3 & M3_WANTSBELL))
#define infravision(ptr) (((ptr)->mflags3 & M3_INFRAVISION))
#define infravisible(ptr) (((ptr)->mflags3 & M3_INFRAVISIBLE))
#define is_displacer(ptr) (((ptr)->mflags3 & M3_DISPLACES) != 0L)
#define is_incorporeal(ptr) (((ptr)->mflags3 & M3_INCORPOREAL) != 0L) 
#define has_mflag_is_nonliving(ptr) (((ptr)->mflags3 & M3_NONLIVING) != 0L) /* do not use this normally; to make clearly different from is_not_living */
#define is_multiweaponmonster(ptr) (((ptr)->mflags3 & M3_MULTIWEAPON) != 0L)
#define leaves_corpses_randomly(ptr) (((ptr)->mflags3 & M3_RANDOM_CORPSE) != 0L)
#define corpse_crumbles_to_dust(ptr) (((ptr)->mflags3 & M3_CORPSE_CRUMBLES_TO_DUST) != 0L)
#define is_speaking(ptr) (((ptr)->mflags3 & M3_SPEAKING) != 0L)
#define is_chatty(ptr) (((ptr)->mflags3 & M3_CHATTY) != 0L)
#define is_constrictor(ptr) (((ptr)->mflags3 & M3_CONSTRICTOR) != 0L)
#define hug_throttles(ptr) (((ptr)->mflags3 & M3_STRANGLES_NECK_BY_HAND) != 0L)
#define hug_requires_two_previous_attacks(ptr) (((ptr)->mflags3 & M3_HUG_HITS_IF_TWO_FIRST_ATTACKS_SUCCEEDED) != 0L)
#define knows_pits_and_holes(ptr) (((ptr)->mflags3 & M3_KNOWS_PITS_AND_HOLES) != 0)
#define knows_traps(ptr) (((ptr)->mflags3 & M3_KNOWS_TRAPS) != 0)
/* no corpse (ie, blank scrolls) if killed by fire */
#define completelyburns(ptr) (((ptr)->mflags3 & M3_BURNS_COMPLETELY) != 0)

#define is_brave(ptr) (((ptr)->mflags4 & M4_BRAVE) != 0L)
#define is_fearless(ptr) (((ptr)->mflags4 & M4_FEARLESS) != 0L)
#define has_bloodlust(ptr) (((ptr)->mflags4 & M4_BLOODLUST) != 0L)
#define does_disregard_own_health(ptr) (is_brave(ptr) || mindless(ptr))
#define does_disregard_enemy_strength(ptr) (is_fearless(ptr) || mindless(ptr))
#define mon_has_bloodlust(mtmp) (has_bloodlust((mtmp)->data) || (mtmp)->hasbloodlust)
#define mon_disregards_own_health(mtmp) (does_disregard_own_health((mtmp)->data) || (mtmp)->disregards_own_health)
#define mon_disregards_enemy_strength(mtmp) (does_disregard_enemy_strength((mtmp)->data) || (mtmp)->disregards_enemy_strength)
#define is_protector(mtmp) ((mtmp)->isprotector)
#define does_split_upon_hit(ptr) (((ptr)->mflags4 & M4_SPLITS_UPON_HIT) != 0L)
#define is_vegetarian_food(ptr) (((ptr)->mflags4 & M4_VEGETARIAN_FOOD) != 0L)
#define is_vegan_food(ptr) (((ptr)->mflags4 & M4_VEGAN_FOOD) != 0L)
#define ignores_Elbereth(ptr) (((ptr)->mflags4 & M4_IGNORES_ELBERETH) != 0)
#define slurps_items(ptr) (((ptr)->mflags4 & M4_SLURPS_ITEMS) != 0)
#define is_stony(ptr) (((ptr)->mflags4 & M4_STONY) != 0)
#define is_wooden(ptr) (((ptr)->mflags4 & M4_WOODEN) != 0)
#define flaming(ptr) (((ptr)->mflags4 & M4_FLAMING) != 0L)
#define is_whirly(ptr) (((ptr)->mflags4 & M4_WHIRLY) != 0L)
#define is_bat(ptr) (((ptr)->mflags4 & M4_BAT) != 0L)
#define is_bird(ptr) (((ptr)->mflags4 & M4_BIRD) != 0L)
#define likes_lava(ptr) (((ptr)->mflags4 & M4_LIKES_LAVA) != 0)
#define likes_fire(ptr) (((ptr)->mflags4 & M4_LIKES_FIRE) != 0)
#define smells_buried_searchable(ptr) (((ptr)->mflags4 & M4_SMELLS_BURIED_SEARCHABLE) != 0)
#define has_vorpal_vulnerability(ptr) (((ptr)->mflags4 & M4_VORPAL_VULNERABILITY) != 0)
#define is_watery(ptr) (((ptr)->mflags4 & M4_WATERY) != 0)
#define is_always_faithful(ptr) (((ptr)->mflags4 & M4_FAITHFUL) != 0)
#define is_bear(ptr) (((ptr)->mflags4 & M4_BEAR) != 0L)
#define is_mercenary(ptr) (((ptr)->mflags4 & M4_MERC) != 0L)
#define is_lord(ptr) (((ptr)->mflags4 & M4_LORD) != 0L)
#define is_prince(ptr) (((ptr)->mflags4 & M4_PRINCE) != 0L)
#define is_ndemon(ptr) \
    (is_demon(ptr) && !is_lord(ptr) && !is_prince(ptr))
#define is_dlord(ptr) (is_demon(ptr) && is_lord(ptr))
#define is_dprince(ptr) (is_demon(ptr) && is_prince(ptr))
#define is_packmule(ptr) (((ptr)->mflags4 & M4_PACKMULE) != 0L)
#define is_iron(ptr) (((ptr)->mflags4 & M4_IRON) != 0)
#define is_noniron_metal(ptr) (((ptr)->mflags4 & M4_METAL) != 0)
#define is_metal(ptr) (is_iron(ptr) || is_noniron_metal(ptr))

#define innate_eschew_cursed(ptr) (((ptr)->mflags5 & M5_ESCHEWS_CURSED_ITEMS) != 0)
#define innate_eschew_blessed(ptr) (((ptr)->mflags5 & M5_ESCHEWS_BLESSED_ITEMS) != 0)
#define innate_eschew_silver(ptr) (((ptr)->mflags5 & M5_ESCHEWS_SILVER_ITEMS) != 0)
#define hates_light(ptr) (((ptr)->mflags5 & M5_HATES_LIGHT) != 0)
#define is_special_silver_hater(ptr) (((ptr)->mflags5 & M5_HATES_SILVER) != 0)
#define makes_sound_randomly(ptr) (((ptr)->mflags5 & M5_MAKES_SOUND_RANDOMLY) != 0)
#define is_semi_transparent(ptr) (((ptr)->mflags5 & M5_SEMI_TRANSPARENT) != 0)
#define is_radially_transparent(ptr) (((ptr)->mflags5 & M5_RADIAL_TRANSPARENCY) != 0)
#define is_glass_transparent(ptr) (((ptr)->mflags5 & M5_GLASS_TRANSPARENT) != 0)

#define has_mflag_is_non_eater(ptr) (((ptr)->mflags6 & M6_NON_EATER) != 0L)
#define is_corpse_eater(ptr) (((ptr)->mflags6 & M6_CORPSE_EATER) != 0L)
#define is_quantum_mechanic(ptr) (((ptr)->mflags6 & M6_QUANTUM_MECHANIC) != 0)
#define is_hell_hound(ptr) (((ptr)->mflags6 & M6_HELL_HOUND) != 0L)
#define is_boss_monster(ptr) (((ptr)->mflags6 & M6_BOSS_MONSTER) != 0L)
#define may_start_with_saddle(ptr) (((ptr)->mflags6 & M6_MAY_START_WITH_SADDLE) != 0L)
#define is_shade(ptr) (((ptr)->mflags6 & M6_SHADE) != 0)
#define is_teleport_heal_caster(ptr) (((ptr)->mflags6 & M6_TELEPORT_HEAL_TACTICS) != 0)
#define has_monster_type_nontinnable_corpse(ptr) (((ptr)->mflags6 & M6_NON_TINNABLE) != 0)
#define has_monster_type_nonedible_corpse(ptr) (((ptr)->mflags6 & M6_NON_EDIBLE) != 0)
#define revives_upon_meddling(ptr) (((ptr)->mflags6 & M6_REVIVES_UPON_MEDDLING) != 0)
#define lithovore(ptr) (((ptr)->mflags6 & M6_LITHOVORE) != 0L)

#define is_archaeologist(ptr) (((ptr)->mflags7 & M7_ARCHAEOLOGIST) != 0L)
#define is_barbarian(ptr) (((ptr)->mflags7 & M7_BARBARIAN) != 0L)
#define is_caveman(ptr) (((ptr)->mflags7 & M7_CAVEMAN) != 0L)
#define is_healer(ptr) (((ptr)->mflags7 & M7_HEALER) != 0L)
#define is_knight(ptr) (((ptr)->mflags7 & M7_KNIGHT) != 0L)
#define is_monk(ptr) (((ptr)->mflags7 & M7_MONK) != 0L)
#define is_priest(ptr) (((ptr)->mflags7 & M7_PRIEST) != 0L)
#define is_rogue(ptr) (((ptr)->mflags7 & M7_ROGUE) != 0L)
#define is_ranger(ptr) (((ptr)->mflags7 & M7_RANGER) != 0L)
#define is_samurai(ptr) (((ptr)->mflags7 & M7_SAMURAI) != 0L)
#define is_tourist(ptr) (((ptr)->mflags7 & M7_TOURIST) != 0L)
#define is_valkyrie(ptr) (((ptr)->mflags7 & M7_VALKYRIE) != 0L)
#define is_wizard(ptr) (((ptr)->mflags7 & M7_WIZARD) != 0L)

/* combinations */
#define is_not_living(ptr) \
    (is_undead(ptr) || has_mflag_is_nonliving(ptr))
#define is_living(ptr) !is_not_living(ptr)
#define slimeproof(ptr) \
    ((ptr) == &mons[PM_GREEN_SLIME] || flaming(ptr) || is_incorporeal(ptr))

#define is_non_eater(ptr) \
    ((is_not_living(ptr) || is_angel(ptr) || is_demon(ptr) || has_mflag_is_non_eater(ptr) || (!carnivorous(ptr) && !herbivorous(ptr) && !metallivorous(ptr) && !lithovore(ptr))) && !is_corpse_eater(ptr))

#define mon_is_literate(m) (is_speaking((m)->data) && !mindless((m)->data) && haseyes((m)->data))

/* Resistances and properties */
#define has_innate(ptr, bit) \
    (((ptr)->mresists & (bit)) != 0)
#define has_innate2(ptr, bit) \
    (((ptr)->mresists2 & (bit)) != 0)
#define has_property(mon, propindex) \
    ((mon)->mprops[(propindex)] != 0)
#define has_innate_or_property(mon, propindex)\
    (has_property(mon, propindex) || has_innate((mon)->data, prop_to_innate(propindex)) || has_innate2((mon)->data, prop_to_innate2(propindex)))


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

#define has_innate_levitation_control(ptr) \
    has_innate(ptr, MR_LEVITATION_CONTROL)

#define has_innate_blind_telepathy(ptr)   \
    has_innate(ptr, MR_BLIND_TELEPATHY)

#define has_innate_telepathy(ptr) \
    has_innate(ptr, MR_TELEPATHY)

#define has_innate_energy_regeneration(ptr) \
    has_innate2(ptr, MR2_ENERGY_REGENERATION)

#define has_innate_sick_resistance(ptr) \
    has_innate(ptr, MR_SICK)

/* fixed ability */
#define has_fixed_ability(mon) \
    has_property(mon, FIXED_ABIL)

#define has_brain_protection(mon) \
    has_property(mon, BRAIN_PROTECTION)

#define has_unchanging(mon) \
    has_property(mon, UNCHANGING)

/* mind shielding */
#define has_mind_shielding(mon) \
    has_property(mon, MIND_SHIELDING)

#define mon_has_no_apparent_mind(mon) \
    (has_mind_shielding(mon) || mindless((mon)->data))

/* slow, stoning, and sliming */
#define has_slowed(mon) \
    has_property(mon, SLOWED)

#define has_slimed(mon) \
    has_property(mon, SLIMED)

#define is_turning_into_slime(mon) \
    (has_slimed(mon))

#define has_stoned(mon) \
    has_property(mon, STONED)

#define has_petrification_resistance(mon) \
    has_innate_or_property(mon, STONE_RESISTANCE)

#define is_stoning(mon) \
    (has_property(mon, STONED) && !has_petrification_resistance(mon))

#define is_slow(mon) \
    (has_slowed(mon) || is_turning_into_slime(mon) || is_stoning(mon))


/* strangulation and suffocation */
#define has_magical_breathing(mon) \
    (has_property(mon, MAGICAL_BREATHING))

#define is_breathless(mon) \
    (has_magical_breathing(mon) || has_innate_breathless((mon)->data))

#define mon_survives_without_air(mon)  (is_breathless(mon))

#define has_strangled(mon) \
    has_property(mon, STRANGLED)

#define is_being_strangled(mon) \
    (has_strangled(mon))

#define has_airless_environment(mon) \
    has_property(mon, AIRLESS_ENVIRONMENT)

#define is_suffocating(mon) \
    (has_airless_environment(mon) && !has_magical_breathing(mon))


/* magic-use related properties */
#define has_cancelled(mon) \
    has_property(mon, CANCELLED)

#define is_cancelled(mon) \
    has_cancelled(mon)

/* This is really a delay until one can be cancelled again */
#define has_cancellation_resistance(mon) \
    has_property(mon, CANCELLATION_RESISTANCE)

#define has_three_fourths_magic_resistance(mon) \
    has_property(mon, THREE_FOURTHS_MAGIC_RESISTANCE)

#define has_half_magic_resistance(mon) \
    has_property(mon, HALVED_MAGIC_RESISTANCE)

#define has_one_fourth_magic_resistance(mon) \
    has_property(mon, ONE_FOURTH_MAGIC_RESISTANCE)

#define has_no_magic_resistance(mon) \
    has_property(mon, NO_MAGIC_RESISTANCE)

#define has_summon_forbidden(mon) \
    has_property(mon, SUMMON_FORBIDDEN)

#define has_silenced(mon) \
    has_property(mon, SILENCED)

#define is_silenced(mon) \
    has_silenced(mon)

/* half damage */
#define has_half_physical_damage(mon) \
    has_property(mon, HALF_PHYSICAL_DAMAGE)

#define has_half_spell_damage(mon) \
    has_property(mon, HALF_SPELL_DAMAGE)

#define has_half_physical_damage_against_undead_and_demons(mon) \
    has_property(mon, HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS)


/* paralysis, sleeping, and can move */
#define has_paralyzed(mon) \
    has_property(mon, PARALYZED)

#define has_undead_immobility(mon) \
    has_property(mon, UNDEAD_IMMOBILITY)

#define has_free_action(mon) \
    has_innate_or_property(mon, FREE_ACTION)

#define has_sleeping(mon) \
    has_property(mon, SLEEPING)

#define is_sleeping(mon) \
    (has_sleeping(mon) || (mon)->msleeping)


/* stun and confusion */
#define has_stunned(mon) \
    has_property(mon, STUNNED)

#define is_stunned(mon) \
    (has_stunned(mon))

#define has_stun_resistance(mon) \
    has_property(mon, STUN_RESISTANCE)

#define has_confused(mon) \
    has_property(mon, CONFUSION)

#define is_confused(mon) \
    (has_confused(mon))


/* blindness and can see (meaning can detect the player at the first place) */
#define has_blinded(mon) \
    has_property(mon, BLINDED)

#define has_blindfolded(mon) \
    has_property(mon, BLINDFOLDED)

#define is_blinded(mon) \
    (has_blinded(mon) || has_blindfolded(mon))

#define mon_can_see(mon) \
    (!is_blinded(mon))

#define is_deaf(mon) \
    (has_property(mon, DEAF))

/* hallucination */
#define has_hallucination(mon) \
    has_property(mon, HALLUC)

#define has_hallucination_resistance(mon) \
    has_innate_or_property(mon, HALLUC_RES)

#define is_hallucinating(mon) \
    (has_hallucination(mon) && !has_hallucination_resistance(mon))


/* sickness */
#define has_sick(mon) \
    has_property(mon, SICK)

#define has_sickness_resistance(mon) \
    has_innate_or_property(mon, SICK_RESISTANCE)

#define is_sick(mon) \
    (has_sick(mon) && !has_sickness_resistance(mon) && !is_undead((mon)->data) && !is_demon((mon)->data) && !is_vampshifter(mon))

#define has_food_poisoned(mon) \
    has_property(mon, FOOD_POISONED)

#define is_food_poisoned(mon) \
    (has_food_poisoned(mon) && !has_sickness_resistance(mon) && !is_undead((mon)->data) && !is_demon((mon)->data) && !is_vampshifter(mon))

#define has_vomiting(mon) \
    has_property(mon, VOMITING)

#define has_mummy_rot(mon) \
    has_property(mon, MUMMY_ROT)

#define is_mummy_rotted(mon) \
    (has_mummy_rot(mon) && !has_sickness_resistance(mon) && !is_undead((mon)->data) && !is_demon((mon)->data) && !is_vampshifter(mon))


/* charmed and tame */
#define has_charmed(mon) \
    has_property(mon, CHARMED)

#define has_innate_charm_resistance(ptr) \
    has_innate(ptr, MR_CHARM)

#define has_charm_resistance(mon) \
    (has_innate_charm_resistance((mon)->data) || has_property(mon, CHARM_RESISTANCE))

#define has_undead_control(mon) \
    has_property(mon, UNDEAD_CONTROL)

#define is_charmed(mon) \
    ((has_charmed(mon) && !has_charm_resistance(mon) && !is_undead((mon)->data) && !is_vampshifter(mon) && !mon_has_no_apparent_mind(mon)) \
     || (has_undead_control(mon) && (is_undead((mon)->data) || is_vampshifter(mon))) \
    )

#define is_tame(mon) \
    ((is_charmed(mon) || (mon)->mtame) && (mon)->mextra && (mon)->mextra->edog) /* Note: currently a monster cannot be tame without an edog */

#define is_peaceful(mon) \
    (is_charmed(mon) || (mon)->mpeaceful)

#define is_mon_protecting(mtmp) (is_protector(mtmp) && is_peaceful(mtmp))

/* fearful and fleeing */
#define has_fearful(mon) \
    has_property(mon, FEARFUL)

#define has_fear_resistance(mon) \
    (has_innate_or_property(mon, FEAR_RESISTANCE))

#define is_fearful(mon) \
    (has_fearful(mon) && !has_fear_resistance(mon) && !is_undead((mon)->data) && !mon_has_no_apparent_mind(mon) && !is_vampshifter(mon))

#define is_fleeing(mon) \
    (is_fearful(mon) || (mon)->mflee)


/* flying and levitation */
#define has_levitation(mon) \
    (has_property(mon, LEVITATION) || is_floater(mon->data))

#define has_levitation_control(mon) \
    (has_innate_levitation_control((mon)->data) || has_property(mon, LEVITATION_CONTROL))

#define has_flying(mon) \
    (has_innate_or_property(mon, FLYING) || is_flyer((mon)->data))

#define has_blocks_levitation(mon) \
    has_property(mon, BLOCKS_LEVITATION)

#define has_blocks_flying(mon) \
    has_property(mon, BLOCKS_FLYING)

#define is_flying(mon) \
    (has_flying(mon) && !has_blocks_flying(mon))

#define is_levitating(mon) \
    (has_levitation(mon) && !is_flying(mon) && !has_blocks_levitation(mon))

#define mon_can_reach_floor(mon) \
    (!is_levitating(mon) || Is_airlevel(&u.uz)|| Is_waterlevel(&u.uz))

#define has_swimming(mon) \
    (has_innate_or_property(mon, SWIMMING) || is_swimmer((mon)->data))

#define has_passes_walls(mon) \
    (has_innate_or_property(mon, PASSES_WALLS) || passes_walls(mon->data))

#define does_pass_walls(mon) \
    (has_passes_walls(mon))

/* invisibility */
#define has_blocks_invisibility(mon) \
    has_property(mon, BLOCKS_INVISIBILITY)

#define has_invisibility(mon) \
    ((has_innate_invisibility(mon->data) || has_property(mon, INVISIBILITY)))

#define is_invisible(mon) \
    (has_invisibility(mon) && !has_blocks_invisibility(mon))


/* speed */
#define has_fast(mon) \
    has_property(mon, FAST)

#define has_very_fast(mon) \
    has_property(mon, VERY_FAST)

#define has_ultra_fast(mon) \
    has_property(mon, ULTRA_FAST)

#define has_super_fast(mon) \
    has_property(mon, SUPER_FAST)

#define has_lightning_fast(mon) \
    has_property(mon, LIGHTNING_FAST)

#define is_lightning_fast(mon) \
    (has_lightning_fast(mon) && !is_slow(mon))

#define is_super_fast(mon) \
    (has_super_fast(mon) && !is_slow(mon) && !is_lightning_fast(mon))

#define is_ultra_fast(mon) \
    (has_ultra_fast(mon) && !is_slow(mon) && !is_super_fast(mon) && !is_lightning_fast(mon))

#define is_very_fast(mon) \
    (has_very_fast(mon) && !is_slow(mon) && !is_ultra_fast(mon) && !is_super_fast(mon) && !is_lightning_fast(mon))

#define is_fast(mon) \
    (has_fast(mon) && !is_slow(mon) && !is_very_fast(mon) && !is_ultra_fast(mon) && !is_super_fast(mon) && !is_lightning_fast(mon))





/* other properties */
#define has_heroism(mon) \
    has_property(mon, HEROISM)

#define has_super_heroism(mon) \
    has_property(mon, SUPER_HEROISM)

#define is_heroic(mon) \
    has_heroism(mon)

#define is_super_heroic(mon) \
    has_super_heroism(mon)


#define has_rapid_regeneration(mon) \
    has_property(mon, RAPID_REGENERATION)

#define has_rapider_regeneration(mon) \
    has_property(mon, RAPIDER_REGENERATION)

#define has_rapidest_regeneration(mon) \
    has_property(mon, RAPIDEST_REGENERATION)

#define has_rapid_energy_regeneration(mon) \
    has_property(mon, RAPID_ENERGY_REGENERATION)

#define has_rapider_energy_regeneration(mon) \
    has_property(mon, RAPIDER_ENERGY_REGENERATION)

#define has_rapidest_energy_regeneration(mon) \
    has_property(mon, RAPIDEST_ENERGY_REGENERATION)

#define has_divine_regeneration(mon) \
    has_property(mon, DIVINE_REGENERATION)

#define has_melee_life_leech(mon) \
    has_property(mon, MELEE_LIFE_LEECH)

#define has_crazed(mon) \
    has_property(mon, CRAZED)

#define is_crazed(mon) \
    (has_crazed(mon) && !has_mind_shielding(mon))

#define has_telepathy(mon)   \
     (has_innate_telepathy((mon)->data) || has_property(mon, TELEPAT))

#define has_blind_telepathy(mon)   \
     (has_innate_blind_telepathy((mon)->data) || has_property(mon, BLIND_TELEPATHY))

#define has_drain_resistance(mon) \
    (has_innate_or_property(mon, DRAIN_RESISTANCE))

#define has_death_resistance(mon) \
    (has_innate_or_property(mon, DEATH_RESISTANCE))

#define has_fire_vulnerability(mon) \
    (has_innate((mon)->data, MR_FIRE_VULNERABLITY) || has_property(mon, FIRE_VULNERABILITY))

#define has_cold_vulnerability(mon) \
    (has_innate((mon)->data, MR_COLD_VULNERABLITY) || has_property(mon, COLD_VULNERABILITY))

#define has_elec_vulnerability(mon) \
    (has_innate((mon)->data, MR_SHOCK_VULNERABLITY) || has_property(mon, SHOCK_VULNERABILITY))

#define has_magm_vulnerability(mon) \
    (has_innate((mon)->data, MR_MAGIC_MISSILE_VULNERABLITY) || has_property(mon, MAGIC_MISSILE_VULNERABILITY))

#define has_invulnerable(mon) \
    (has_property(mon, INVULNERABLE))

#define has_double_physical_damage(mon) \
    (has_property(mon, DOUBLE_PHYSICAL_DAMAGE))

#define has_double_spell_damage(mon) \
    (has_property(mon, DOUBLE_SPELL_DAMAGE))

#define has_titan_strength(mon) \
    has_property(mon, TITAN_STRENGTH)

#define has_divine_endurance(mon) \
    has_property(mon, DIVINE_ENDURANCE)

#define has_divine_intellect(mon) \
    has_property(mon, DIVINE_INTELLECT)

#define has_divine_wisdom(mon) \
    has_property(mon, DIVINE_WISDOM)

#define has_divine_dexterity(mon) \
    has_property(mon, DIVINE_DEXTERITY)

#define has_divine_charisma(mon) \
    has_property(mon, DIVINE_CHARISMA)

#define has_wounded_legs(mon) \
    has_property(mon, WOUNDED_LEGS)

#define has_bisection_resistance(mon) \
    (has_property(mon, BISECTION_RESISTANCE))

#define has_slime_resistance(mon) \
    (has_property(mon, SLIME_RESISTANCE))

/* permonst resistances */
#define pm_resists_disint(ptr) \
    (has_innate(ptr, MR_DISINT) || is_incorporeal(ptr))

#define pm_immune_to_fire(ptr) \
    (has_innate(ptr, MR_FIRE))

#define pm_immune_to_cold(ptr) \
    (has_innate(ptr, MR_COLD) || is_undead(ptr))

#define pm_immune_to_elec(ptr) \
    (has_innate(ptr, MR_ELEC))

#define pm_resists_fire(ptr) \
    (has_innate2(ptr, MR2_FIRE_RESISTANCE))

#define pm_resists_cold(ptr) \
    (has_innate2(ptr, MR2_COLD_RESISTANCE) || is_undead(ptr))

#define pm_resists_elec(ptr) \
    (has_innate2(ptr, MR2_SHOCK_RESISTANCE))

/* resistances at the time of acquisition */
#define is_mon_immune_to_fire(mon) \
    ( pm_immune_to_fire((mon)->data) || has_property(mon, FIRE_IMMUNITY))
#define is_mon_immune_to_cold(mon) \
    (pm_immune_to_cold((mon)->data) || has_property(mon, COLD_IMMUNITY))
#define is_mon_immune_to_elec(mon) \
    (pm_immune_to_elec((mon)->data) || has_property(mon, SHOCK_IMMUNITY))
#define is_mon_immune_to_magic_missile(mon) \
    (has_innate((mon)->data, MR_MAGIC_MISSILE) || has_property(mon, MAGIC_MISSILE_IMMUNITY))
#define is_mon_immune_to_acid(mon) \
    (has_innate((mon)->data, MR_ACID) || has_property(mon, ACID_IMMUNITY) || is_incorporeal((mon)->data))

#define mon_resists_fire_weakly(mon) \
    ( pm_resists_fire((mon)->data) || has_property(mon, FIRE_RESISTANCE))
#define mon_resists_cold_weakly(mon) \
    (pm_resists_cold((mon)->data) || has_property(mon, COLD_RESISTANCE) || is_vampshifter(mon))
#define mon_resists_elec_weakly(mon) \
    (pm_resists_elec((mon)->data) || has_property(mon, SHOCK_RESISTANCE))
#define mon_resists_magic_missile_weakly(mon) \
    (has_innate2((mon)->data, MR2_MAGIC_MISSILE_RESISTANCE) || has_property(mon, MAGIC_MISSILE_RESISTANCE))
#define mon_resists_acid_weakly(mon) \
    (has_innate((mon)->data, MR2_ACID_RESISTANCE) || has_property(mon, ACID_RESISTANCE))

#define mon_resists_fire_strongly(mon) \
    (has_property(mon, IMPROVED_FIRE_RESISTANCE))
#define mon_resists_cold_strongly(mon) \
    (has_property(mon, IMPROVED_COLD_RESISTANCE))
#define mon_resists_elec_strongly(mon) \
    (has_property(mon, IMPROVED_SHOCK_RESISTANCE))
#define mon_resists_magic_missile_strongly(mon) \
    (has_property(mon, IMPROVED_MAGIC_MISSILE_RESISTANCE))
#define mon_resists_acid_strongly(mon) \
    (has_property(mon, IMPROVED_ACID_RESISTANCE))


#define resists_disint(mon) \
    (pm_resists_disint((mon)->data) || has_property(mon, DISINTEGRATION_RESISTANCE))

#define resists_sleep(mon) \
    (has_innate((mon)->data, MR_SLEEP) || has_property(mon, SLEEP_RESISTANCE) || is_undead((mon)->data) || is_vampshifter(mon))
#define resists_death(mon) \
    (has_innate((mon)->data, MR_DEATH) || has_property(mon, DEATH_RESISTANCE) || is_not_living((mon)->data) || is_demon((mon)->data) || is_vampshifter(mon))
#define resists_lycanthropy(mon) \
    (has_innate((mon)->data, MR_LYCANTHROPY) || has_property(mon, LYCANTHROPY_RESISTANCE) || is_not_living((mon)->data) || is_demon((mon)->data) || is_elemental((mon)->data) || is_incorporeal((mon)->data) || unsolid((mon)->data) || is_vampshifter(mon))
#define resists_poison(mon) \
    (has_innate((mon)->data, MR_POISON) || has_property(mon, POISON_RESISTANCE) || is_not_living((mon)->data) || is_elemental((mon)->data) || is_incorporeal((mon)->data) || unsolid((mon)->data) || is_vampshifter(mon))
#define resists_ston(mon) \
    (has_innate((mon)->data, MR_STONE) || has_property(mon, STONE_RESISTANCE) || is_incorporeal((mon)->data))
#define resists_magic(mon) \
    (has_innate((mon)->data, MR_MAGIC) || has_property(mon, ANTIMAGIC))
#define resists_charm(mon) \
    (has_innate((mon)->data, MR_CHARM) || has_property(mon, CHARM_RESISTANCE) || is_undead((mon)->data) || mon_has_no_apparent_mind(mon))
#define resists_fear(mon) \
    (has_innate((mon)->data, MR_FEAR) || has_property(mon, FEAR_RESISTANCE) || is_undead((mon)->data) || mon_has_no_apparent_mind(mon) || is_vampshifter(mon))
#define is_reflecting(mon) \
    (has_innate((mon)->data, MR_REFLECTING) || has_property(mon, REFLECTING))
#define resists_drain(mon) \
    (has_innate((mon)->data, MR_DRAIN) || has_property(mon, DRAIN_RESISTANCE) ||  is_not_living((mon)->data) || is_were((mon)->data) || \
     is_demon((mon)->data) || is_vampshifter(mon) || ((mon) == &youmonst && u.ulycn >= LOW_PM))
#define resists_flash(mon) \
    (has_innate((mon)->data, MR_FLASH) || has_property(mon, FLASH_RESISTANCE) || is_blinded(mon) || !haseyes((mon)->data) )
#define resists_sickness(mon) \
    (has_innate((mon)->data, MR_SICK) || has_property(mon, SICK_RESISTANCE) ||  is_not_living((mon)->data) || is_demon((mon)->data) || is_elemental((mon)->data) || is_incorporeal((mon)->data) || unsolid((mon)->data) || is_vampshifter(mon) )
#define resists_paralysis(mon) \
    (has_innate((mon)->data, MR_FREE_ACTION) || has_property(mon, FREE_ACTION) ||  is_not_living((mon)->data) || is_vampshifter(mon))
#define resists_stun(mon) \
    (has_innate((mon)->data, MR_STUN) || has_property(mon, STUN_RESISTANCE) ||  mindless((mon)->data) || is_not_living((mon)->data) || is_vampshifter(mon))

/* other "resists" definitions */
#define resists_bisection(mon) \
    (has_bisection_resistance(mon) || is_incorporeal((mon)->data) || amorphous((mon)->data))

#define resists_slime(mon) \
    (slimeproof((mon)->data) || has_slime_resistance(mon))

/* more on paralysis */
#define is_paralyzed(mon) \
    ((has_paralyzed(mon) && !resists_paralysis(mon)) || \
     ((is_undead(mon->data) || is_vampshifter(mon)) && has_undead_immobility(mon) && !(has_innate((mon)->data, MR_FREE_ACTION) || has_property(mon, FREE_ACTION))) \
    )

#define mon_can_move(mon) \
    ((mon)->mcanmove && !is_sleeping(mon) && !is_paralyzed(mon))


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
#define conveys_level(ptr) \
    (((ptr)->mconveys & MC_LEVEL_GAIN) != 0)
#define conveys_invisibility(ptr) \
    (((ptr)->mconveys & MC_INVISIBILITY) != 0)
#define conveys_see_invisible(ptr) \
    (((ptr)->mconveys & MC_SEE_INVISIBLE) != 0)


#define has_see_invisible(mon) \
    (has_innate_see_invisible((mon)->data) || has_property(mon, SEE_INVISIBLE))
#define has_regeneration(mon) \
    (has_innate_regeneration((mon)->data) || has_property(mon, REGENERATION))
#define has_energy_regeneration(mon) \
    (has_innate_energy_regeneration((mon)->data) || has_property(mon, ENERGY_REGENERATION))
#define mon_spec_cooldown_divisor(mon) \
    (has_property(mon, RAPIDEST_ENERGY_REGENERATION) ? 5 : has_property(mon, RAPIDER_ENERGY_REGENERATION) ? 4 : has_property(mon, RAPID_ENERGY_REGENERATION) ? 3 : has_energy_regeneration(mon) ? 2 : 1)
#define has_teleportation(mon) \
    (has_innate_teleportation((mon)->data) || has_property(mon, TELEPORT))
#define has_teleport_control(mon) \
    (has_innate_teleport_control((mon)->data) || has_property(mon, TELEPORT_CONTROL))
#define has_blind_telepathy(mon) \
    (has_innate_blind_telepathy((mon)->data) || has_property(mon, BLIND_TELEPATHY))
#define has_telepathy(mon) \
    (has_innate_telepathy((mon)->data) || has_property(mon, TELEPAT))
#define has_detect_monsters(mon) \
    (has_property(mon, DETECT_MONSTERS))

#define has_stunning_corpse(ptr) (((ptr)->mconveys & MC_STUNNING_CORPSE) != 0L)
#define has_acidic_corpse(ptr) (((ptr)->mconveys & MC_ACIDIC_CORPSE) != 0L)
#define has_poisonous_corpse(ptr) (((ptr)->mconveys & MC_POISONOUS_CORPSE) != 0L)
#define has_hallucinating_corpse(ptr) (((ptr)->mconveys & MC_HALLUCINATING_CORPSE) != 0L)
#define has_sickening_corpse(ptr) (((ptr)->mconveys & MC_SICKENING_CORPSE) != 0L)
#define has_mummy_rotted_corpse(ptr) (((ptr)->mconveys & MC_MUMMY_ROTTED_CORPSE) != 0L)

#define is_armed(ptr) attacktype(ptr, AT_WEAP)


#define is_mplayer(ptr) \
    (((ptr) >= &mons[PM_ARCHAEOLOGIST]) && ((ptr) <= &mons[PM_WIZARD]))
#define is_watch(ptr) \
    ((ptr) == &mons[PM_WATCHMAN] || (ptr) == &mons[PM_WATCH_CAPTAIN])
#define is_rider(ptr)                                      \
    ((ptr) == &mons[PM_DEATH] || (ptr) == &mons[PM_FAMINE] \
     || (ptr) == &mons[PM_PESTILENCE])
#define is_rider_or_tarrasque(ptr)                                      \
    ((ptr) == &mons[PM_TARRASQUE] || is_rider(ptr))
#define is_placeholder(ptr)                             \
    ((ptr) == &mons[PM_ORC] || (ptr) == &mons[PM_GIANT] \
     || (ptr) == &mons[PM_ELF] || (ptr) == &mons[PM_HUMAN])
/* return TRUE if the monster tends to revive */
#define is_reviver(ptr) (((ptr)->mflags3 & M3_REVIVES_FROM_DEAD) != 0)
#define is_reviver_corpse(mnum) ((mons[(mnum)].mflags3 & M3_REVIVES_FROM_DEAD) != 0)
#define does_regenerate_bodyparts(ptr) (((ptr)->mflags3 & M3_REGENERATES_LOST_BODY_PARTS) != 0)
//(is_rider(ptr) || (ptr) == &mons[PM_PHOENIX] || (ptr)->mlet == S_TROLL)
/* monsters whose corpses and statues need special handling;
   note that high priests and the Wizard of Yendor are flagged
   as unique even though they really aren't; that's ok here */
#define unique_corpstat(ptr) (((ptr)->geno & G_UNIQ) != 0)
#define pm_has_female_name(ptr) ((ptr)->mfemalename != 0)

#define emitted_light_range(ptr) ((ptr)->lightrange)
#define touch_petrifies(ptr) (((ptr)->mflags1 & M1_TOUCH_PETRIFIES) != 0)
#define flesh_petrifies(ptr) (touch_petrifies(ptr) || (ptr) == &mons[PM_MEDUSA])
#define is_vampire(ptr) ((ptr)->mlet == S_VAMPIRE)

#define mon_ambient_sound(ptr) get_monster_ambient_sound_id((ptr)->soundset)//((ptr)->soundset <= MONSTER_SOUNDSET_NONE ? GHSOUND_NONE : monster_soundsets[(ptr)->soundset].sounds[MONSTER_SOUND_TYPE_AMBIENT].ghsound < MAX_GHSOUNDS ? monster_soundsets[(ptr)->soundset].sounds[MONSTER_SOUND_TYPE_AMBIENT].ghsound : GHSOUND_NONE)
#define mon_ambient_volume(ptr) get_monster_ambient_sound_volume((ptr)->soundset) //((ptr)->soundset <= MONSTER_SOUNDSET_NONE ? 0 : monster_soundsets[(ptr)->soundset].sounds[MONSTER_SOUND_TYPE_AMBIENT].volume)
#define mon_ambient_subtype(ptr) ((ptr)->soundset <= MONSTER_SOUNDSET_NONE ? SOUNDSOURCE_AMBIENT_GENERAL : monster_soundsets[(ptr)->soundset].ambient_subtype)

/* cursed items are good for some evil creatures */
#define cursed_items_are_positive(ptr) \
    (is_undead(ptr) || is_demon(ptr))

/* Used for conduct with corpses, tins, and digestion attacks */
/* G_NOCORPSE monsters might still be swallowed as a purple worm */
/* Maybe someday this could be in mflags... */
#define incorporeal_food(ptr) is_incorporeal(ptr)

#define vegan(ptr)                                                 \
    (is_vegan_food(ptr) || incorporeal_food(ptr))

#define vegetarian(ptr) \
    (vegan(ptr) || is_vegetarian_food(ptr))

#define nonrotting_corpse_ptr(ptr) (((ptr)->mflags4 & M4_NONROTTING_CORPSE) != 0)
#define nonrotting_corpse(mnum) nonrotting_corpse_ptr(&mons[mnum])
#define has_pitwalk(ptr) (((ptr)->mflags4 & M4_PITWALK) != 0)
#define can_speak_language(ptr) ((ptr)->msound >= MS_LAUGH || (ptr)->msound == MS_ORC || is_speaking(ptr))
#define call_mon_tame(mon)  (is_domestic((mon)->data) || ((is_animal((mon)->data) || !humanoid((mon)->data)) && !mindless((mon)->data) && !is_speaking((mon)->data) && is_living((mon)->data) && !is_demon((mon)->data)))

/* Overall resistances */
#define resists_drli(mon) resists_drain(mon)

#define resists_blnd(mon) \
    (((mon) == &youmonst && (Blind || Unaware || Flash_resistance)) || (is_blinded(mon) \
    || !haseyes((mon)->data) || is_sleeping(mon) || resists_flash(mon)))

#define is_immune_to_life_leech(ptr) \
    (is_rider(ptr) || is_not_living(ptr))
#define resists_life_leech(mon) \
    (is_immune_to_life_leech((mon)->data) || is_vampshifter(mon))

/* Other */
#define is_mon_talkative(mon) (is_speaking((mon)->data) && (is_peaceful(mon) || is_chatty((mon)->data)))

/* monkeys are tameable via bananas but not pacifiable via food,
   otherwise their theft attack could be nullified too easily;
   dogs and cats can be tamed by anything they like to eat and are
   pacified by any other food;
   horses can be tamed by always-veggy food or lichen corpses but
   not tamed or pacified by other corpses or tins of veggy critters */


#endif /* MONDATA_H */
