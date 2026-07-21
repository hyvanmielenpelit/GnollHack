/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    extern.h    $NHDT-Date: 1557088399 2019/05/05 20:33:19 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.703 $ */
/* Copyright (c) Steve Creps, 1988.                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef EXTERN_H
#define EXTERN_H

#include "general.h"
#include "layer.h"
#include "lev.h"
#include "prop.h"
#include "soundset.h"
#include "tiledata.h"
#include "you.h"

#define E extern

/* ### alloc.c ### */

#if 0
E int64_t *alloc(size_t);
#endif
E char *fmt_ptr(const genericptr);

/* This next pre-processor directive covers almost the entire file,
 * interrupted only occasionally to pick up specific functions as needed. */
#if !defined(MAKEDEFS_C) && !defined(LEV_LEX_C)

/* ### allmain.c ### */

E void moveloop(uchar);
E void stop_occupation(void);
E void display_gamewindows(void);
E void newgame(void);
E void choose_game_difficulty(void);
E void set_mouse_buttons(void);
E const char* get_game_difficulty_text(int);
E const char* get_game_difficulty_symbol(int);
E const char* get_game_mode_text(boolean);
E const char* get_game_mode_text_core(boolean, boolean, boolean, boolean, boolean, boolean, boolean);
E const char* get_game_mode_description(void);
E const char* get_game_mode_description_core(boolean, boolean, boolean, boolean);
E void show_gui_tips(void);
E void welcome(boolean);
E time_t get_realtime(void);
E int argcheck(int, char **, enum earlyarg);
E void reset_monster_origin_coordinates(struct monst*);
E void reset_all_monster_origin_coordinates(void);
E void reset_object_origin_coordinates(struct obj*);
E void reset_all_object_origin_coordinates(void);
E void lock_thread_lock(void);
E void unlock_thread_lock(void);
E void reset_allmain(void);
E double calculate_hp_regeneration(boolean*);
E double calculate_mana_regeneration(boolean*);

/* ### animation.c ### */

E void init_animations(void);
E short get_player_replacement(enum action_tile_types, int, int, int, int, int);
E short get_player_animation(enum action_tile_types, int, int, int, int, int);
E short get_player_enlargement(enum action_tile_types, int, int, int, int, int);
E int get_player_action_glyph_offset(enum action_tile_types);
E int get_monster_action_glyph_offset(enum action_tile_types, int);
E int maybe_get_replaced_glyph(int, int, int, struct replacement_info);
E int maybe_get_animated_tile(int, int, enum animation_play_types, int64_t, int*, int*, char*, enum autodraw_types*);
E int get_tile_animation_index_from_glyph(int);
E int get_replacement_base_tile(short);
E int get_animation_base_tile(short);
E int get_enlargement_base_tile(short, short);
E struct replacement_info data_to_replacement_info(int, int, struct obj*, struct monst*, uint64_t, uint64_t, uint64_t, uchar, short);
E int get_animation_frame_with_tile(int, int);
E void play_special_effect_with_details_at(int, int, int, int, enum layer_types, short, int, int, int, boolean);
E void play_special_effect_at(enum special_effect_types, int, int, int, boolean);
E void special_effect_wait_until_action(int);
E void special_effect_wait_until_end(int);
E int get_u_move_speed(boolean);
E void reduce_counters(int);
E void reduce_counters_intervals(int);
E void stop_animations(void);
E boolean glyph_is_specific_cmap_or_its_variation(int, int);
E boolean no_wall_end_autodraw(int, int);

/* ### apply.c ### */

E int doapply(void);
E int doputitemsin(void);
E int dotakeitemsout(void);
E int dobreak(void);
E int dorub(void);
E int dojump(void);
E int jump(int);
E int number_leashed(void);
E void o_unleash(struct obj *);
E void m_unleash(struct monst *, boolean);
E void unleash_all(void);
E boolean next_to_u(void);
E struct obj *get_mleash(struct monst *);
E const char *beautiful(void);
E void check_leash(xchar, xchar);
E boolean um_dist(xchar, xchar, xchar);
E boolean snuff_candle(struct obj *);
E boolean snuff_torch(struct obj*);
E boolean snuff_lit(struct obj *);
E boolean catch_lit(struct obj *);
E int use_unicorn_horn(struct obj *, boolean);
E int use_golf_club(struct obj*);
E boolean tinnable(struct obj *);
E void reset_trapset(void);
E int fig_transform(ANY_P *, int64_t);
E int unfixable_trouble_count(boolean);
E boolean turn_undead_success_effect(struct monst*, int, int, int);
E int uthitm(struct monst*, struct obj*, struct monst*);
E int uthito(struct obj*, struct obj*, struct monst*);
E int uthitt(struct trap*, struct obj*, struct monst*);
E int endlessarrows(struct obj*, int, int);
E struct obj* elemental_enchant_quan(struct obj*, int, uchar);
E void use_magic_whistle(struct obj*);
E int count_other_containers(struct obj*, struct obj*, struct obj**, boolean);
E struct obj* select_other_container(struct obj*, struct obj*, boolean);
E void get_pole_type_weapon_min_max_distances(struct obj*, struct monst*, int*, int*);
//E boolean can_pole_hit_in_melee(struct obj*, struct monst*);
E int use_grail(struct obj*, boolean);
E int use_salve(struct obj*, boolean);
E int floorapply(void);
E int use_lever(struct trap*);
E void setapplyclasses(char*);
E void setbreakclasses(char*);
E void use_candelabrum(struct obj*);
E int use_candle(struct obj**);
E int use_torch(struct obj**);
E void use_lamp(struct obj*);
E void light_cocktail(struct obj**);
E int use_pole2(struct obj*, coord*);
E int dopolearm(void);
E int dopickaxe(void);
E void reset_apply(void);

/* ### artifact.c ### */

E void init_artifacts(void);
E void save_artifacts(int);
E void reset_artifacts(void);
E void restore_artifacts(int);
E const char *artiname(short);
E struct obj *mk_artifact(struct obj *, aligntyp, uchar);
E const char *artifact_name(const char *, short *);
E boolean exist_artifact(int, const char *);
E void artifact_exists(struct obj *, const char *, boolean);
E int get_artifact_id(int, const char*);
E int nartifact_exist(void);
E boolean arti_immune(struct obj *, int);
E boolean artifact_has_flag(struct obj *, uint64_t);
E boolean artifact_confers_monster_power(struct monst*, struct obj*, int);
E boolean mythic_confers_monster_power(struct monst*, struct obj*, int);
E boolean material_confers_monster_power(struct monst*, struct obj*, int);
E boolean confers_luck(struct obj *);
E boolean confers_unluck(struct obj*);
E boolean artifact_confers_luck(struct obj*);
E boolean artifact_confers_unluck(struct obj*);
E boolean arti_reflects(struct obj *);
E boolean shade_glare(struct obj *, struct monst*);
E boolean restrict_name(struct obj *, const char *);
E void set_artifact_intrinsic(struct obj *, int64_t);
E int touch_artifact(struct obj *, struct monst *);
E int spec_abon(struct obj *, struct monst *);
E double spec_dbon(struct obj *, struct monst *, struct monst*, double);
E void discover_artifact(short);
E boolean undiscovered_artifact(short);
E int disp_artifact_discoveries(winid);
E int artifact_hit(struct monst *, struct monst *, struct obj *, double *, boolean*, int);
E int pseudo_artifact_hit(struct monst*, struct monst*, struct obj*, int, int, int, short*);
E int doinvoke(void);
E boolean finesse_ahriman(struct obj *);
E void arti_speak(struct obj *);
E boolean artifact_light(struct obj *);
E uint64 spec_m2(struct obj *);
E boolean artifact_has_invprop(struct obj *, uchar);
E int64_t arti_cost(struct obj *);
E struct obj *what_gives(int, boolean);
E const char *glow_color(int);
E const char *glow_verb(int, boolean);
E void Sting_effects(struct obj*, int);
E boolean item_has_specific_monster_warning(struct obj*);
E int retouch_object(struct obj **, boolean);
E void retouch_equipment(int);
E void mkot_trap_warn(void);
E boolean is_magic_key(struct monst *, struct obj *);
E struct obj *has_magic_key(struct monst *);
E boolean carried_base_item_is_giving_monster_power(struct monst*, struct obj*, int, boolean);
E boolean worn_base_item_is_giving_monster_power(struct monst*, struct obj*, int, boolean);
E boolean item_is_giving_monster_power(struct monst*, struct obj*, int, boolean);
E boolean item_is_giving_power(struct obj*, int);
E boolean item_is_giving_known_power(struct obj*, int);
E boolean carried_artifact_is_giving_monster_power(struct monst*, struct obj*, int);
E uint64_t prop_to_spfx(int);
E int spfx_to_prop(uint64_t);
E uchar prop_to_adtyp(int);
E int adtyp_to_prop(uchar);
E struct obj* what_gives_monster(struct monst*, int);
E boolean is_immune(struct monst*, int);
E int create_portal(void);
E const char* get_artifact_invoke_name(int);
E const char* get_artifact_invoke_description(int);
E boolean is_artifact_applicable_as_axe(struct obj*);
E int artifact_to_otyp(int);
E void artifact_taken_away(int);
E void save_initial_artifact_values(void);


/* ### attrib.c ### */

E uchar adjattrib(int, int, int);
E uchar m_adjattrib(struct monst*, int, int, boolean);
E void gainstr(struct obj *, int, boolean);
E void losestr(int);
E void poisontell(int, boolean);
E void poisoned(const char *, int, const char *, int, boolean, int, struct monst*);
E void extra_enchantment_damage(const char*, uchar, const char*, boolean, struct monst*);
E void change_luck(int, boolean);
E int stone_luck(boolean);
E boolean object_uses_spellbook_wand_flags_and_properties(struct obj*);
E boolean inappropriate_character_type(struct obj*);
E boolean inappropriate_monster_character_type(struct monst*, struct obj*);
E boolean inappropriate_exceptionality(struct monst*, struct obj*);
E void update_extrinsics(void);
E void set_moreluck(void);
E void restore_attrib(void);
E void exercise(int, boolean);
E void exerchk(void);
E void init_attr(int);
E void redist_attr(void);
E void adjabil(int, int);
E int newhp(void);
E int hpmaxadjustment(boolean);
E int m_hpmaxadjustment(struct monst*, boolean);
E void updatemaxhp(void);
E void update_mon_maxhp(struct monst*);
E void updateabon(void);
E void update_mon_abon(struct monst*);
E schar acurr(int);
E schar m_acurr(struct monst*, int);
E schar acurrstr(void);
E schar m_acurrstr(struct monst*);
E schar acurrstr_base(int);
E boolean extremeattr(int);
E void adjalign(int);
E int is_innate(int);
E char *from_what(int);
E void uchangealign(int, int);
E const struct innate* race_abil(int);
E const struct innate* role_abil(int);
E boolean is_obj_worn(struct obj*);

/* ### ball.c ### */

E void ballrelease(boolean);
E void ballfall(void);
E void placebc(void);
E void unplacebc(void);
E void set_bc(int);
E void move_bc(int, int, xchar, xchar, xchar, xchar);
E boolean drag_ball(xchar, xchar, int *, xchar *, xchar *,
                            xchar *, xchar *, boolean *, boolean);
E void drop_ball(xchar, xchar);
E void drag_down(void);
E void bc_sanity_check(void);

/* ### bones.c ### */

E void sanitize_name(char *);
E void drop_upon_death(struct monst *, struct obj *, int, int);
E boolean can_make_bones(void);
E void savebones(int, time_t, struct obj *);
E int getbones(void);

/* ### botl.c ### */

E void reset_blstats(void);
E char* get_strength_string(int);
E const char* rank(void);
E char *do_statusline1(void);
E void check_gold_symbol(void);
E char *do_statusline2(void);
E char* do_statusline3(void);
E void bot(void);
E void timebot(void);
E int xlev_to_rank(int);
E const char *rank_of(int, short, boolean);
E int title_to_mon(const char *, int *, size_t*);
E void max_rank_sz(void);
E int64_t botl_score(void);
E int describe_level(char *);
E int describe_mode(char*);
E void status_initialize(boolean);
E void status_finish(void);
E int stat_cap_indx(void);
E int stat_hunger_indx(void);
E const char *bl_idx_to_fldname(int);
#ifdef STATUS_HILITES
E void status_eval_next_unhilite(void);
E void reset_status_hilites(void);
E boolean parse_status_hl1(char *op, boolean);
E void status_notify_windowport(boolean);
E void clear_status_hilites(void);
E int count_status_hilites(void);
E boolean status_hilite_menu(void);
E void botl_save_hilites(void);
E void botl_restore_hilites(void);
#endif
E const char* get_condition_name(uint64_t);
E uint64_t get_u_condition_bits(void);
E uint64_t get_m_status_bits(struct monst*, boolean, boolean, boolean, boolean);
E uint64_t get_m_condition_bits(struct monst*);
E void get_m_buff_bits(struct monst*, uint64_t*, boolean);
E void status_reassess(void);
E char* botl_realtime(void);
E int64_t get_current_game_duration(void);
E int64_t calculate_current_game_duration(struct u_realtime);
E size_t print_conditions(char*);
E void compose_partystatline(char*, char*, char*, char*, char*, size_t);
E char* format_duration_with_units(int64_t);


/* ### cmd.c ### */

E char randomkey(void);
E void random_response(char *, int);
E int rnd_extcmd_idx(void);
E int dodeletesavedgame(void);
E int doconduct(void);
E int domonsterability(void);
E int domonability(void);
E int doability(void);
E int dospellmainmenu(void);
E int docommandmenu(void);
E char cmd_unctrl(char);
E char nondir_cmd_from_func(int (*)(void));
E char cmd_from_func(int (*)(void));
E uchar cmd_from_txt(const char*);
E int ext_cmd_from_txt(const char*);
E boolean redraw_cmd(char);
#ifdef USE_TRAMPOLI
E int doextcmd(void);
E int domonability(void);
E int doprev_message(void);
E int timed_occupation(void);
E int doattributes(void);
E int dopolyformstatistics(void);
E int doviewpetstatistics(void);
E int wiz_detect(void);
E int wiz_files(void);
E int wiz_genesis(void);
E int wiz_identify(void);
E int wiz_level_tele(void);
E int wiz_map(void);
E int wiz_where(void);
E int wiz_wish(void);
#endif /* USE_TRAMPOLI */
E void reset_occupations(void);
E void set_occupation(int (*)(void), const char *, int, int, enum object_soundset_types, enum object_occupation_types, enum occupation_sound_types, int);
E char pgetchar(void);
E void pushch(char);
E void savech(char);
E const char *key2extcmddesc(uchar);
E boolean bind_specialkey(char, const char *);
E char txt2key(char *);
E void parseautocomplete(char *, boolean);
E void reset_commands(boolean);
E void rhack(char *);
E void reduce_last_item_show_duration(void);
E int doextlist(void);
E int extcmd_via_menu(void);
E int enter_explore_mode(void);
E void enlightenment(int, int);
E void youhiding(boolean, int);
E void show_conduct(int);
E boolean bind_key(uchar, const char *);
E void dokeylist(void);
E int xytod(schar, schar);
E void dtoxy(coord *, int);
E int movecmd(char);
E int dxdy_moveok(void);
E int getdir(const char *);
E int getdir_ex(const char*, boolean);
E void confdir(void);
E const char *directionname(int);
E int isok(int, int);
E int get_adjacent_loc(const char *, const char *, xchar, xchar, coord *);
E const char *click_to_cmd(int, int, int);
E char get_count(char *, char, int64_t, int64_t *, boolean);
#ifdef HANGUPHANDLING
E void hangup(int);
E void end_of_input(void);
#endif
E char readchar(void);
E void sanity_check(void);
E char* key2txt(uchar, char *);
E char yn_function(const char*, const char*, char, const char*);
E char yn_function_core(int, int, int, int, const char*, const char *, const char *, char, const char*, const char*, uint64_t);
E char yn_function_mon(struct monst*, const char*, const char*, char, const char*);
E char yn_function_end(const char*, const char*, char, const char*);
E char yn_query_mon(struct monst*, const char*);
E char ynq_mon(struct monst*, const char*);
E char ynaq_mon(struct monst*, const char*);
E boolean paranoid_query(boolean, const char *);
E boolean paranoid_query_ex(int, int, boolean, const char*, const char*);
E boolean object_stats_known(struct obj*);
E int dozoomin(void);
E int dozoomout(void);
E int dozoomnormal(void);
E int dozoommini(void);
E int dozoomhalf(void);
E void zoomtoscale(double);
E void update_bindings_list(void);
E void create_context_menu(enum create_context_menu_types);
E void reset_found_this_turn(void);
E void clear_found_this_turn_at(int, int);
E int doviewpet(void);
E int dofavorite(void);
E int dounfavorite(void);
E int dosetquickengraveitem(void);
E int dounsetquickengraveitem(void);
E int dosetquickpickaxeitem(void);
E int dounsetquickpickaxeitem(void);
E int dosetquickbag(void);
E int dounsetquickbag(void);
E struct monst* spotted_linedup_monster_in_way(int, int, int, int);
E int do_gamelog(void);

/* ### dbridge.c ### */

E boolean is_pool(int, int);
E boolean is_lava(int, int);
E boolean is_pool_or_lava(int, int);
E boolean is_ice(int, int);
E boolean is_moat(int, int);
E schar db_under_typ(int);
E int is_drawbridge_wall(int, int);
E boolean is_db_wall(int, int);
E boolean find_drawbridge(int *, int *);
E boolean create_drawbridge(int, int, int, boolean);
E void open_drawbridge(int, int, boolean);
E void maybe_close_drawbridge(int, int, boolean);
E void close_drawbridge(int, int, boolean);
E void destroy_drawbridge(int, int, boolean);
E void reset_drawbridge(void);

/* ### decl.c ### */

E void decl_init(void);

/* ### detect.c ### */

E boolean trapped_chest_at(int, int, int);
E boolean trapped_door_at(int, int, int);
E struct obj *o_in(struct obj *, char);
E struct obj *o_material(struct obj *, unsigned);
E int gold_detect(struct obj *);
E int food_detect(struct obj *);
E int object_detect(struct obj *, int);
E int monster_detect(struct obj *, int);
E int trap_detect(struct obj *);
E const char *level_distance(d_level *);
E void use_crystal_ball(struct obj **);
E void do_mapping(void);
E void do_vicinity_map(struct obj *);
E void cvt_sdoor_to_door(int, int);
E void cvt_sdoor_to_door_with_animation(int, int);
E void cvt_scorr_to_corr_with_animation(int, int);
#ifdef USE_TRAMPOLI
E void findone(int, int, genericptr_t);
E void openone(int, int, genericptr_t);
#endif
E int findit(void);
E int openit(void);
E boolean detecting(void (*)(int, int, genericptr));
E void find_trap(struct trap *);
E void warnreveal(void);
E int dosearch0(int);
E int dosearch(void);
E void sokoban_detect(void);
#if defined(DUMPLOG) || defined(DUMPHTML)
E void dump_map(void);
#endif
E void reveal_terrain(int, int);
E int reveal_terrain_getglyph(int, int, int, boolean, int, int);

/* ### dig.c ### */

E int dodig(void);
E int dig_typ(struct obj *, xchar, xchar);
E boolean is_digging(void);
#ifdef USE_TRAMPOLI
E int dig(void);
#endif
E int holetime(void);
E boolean dig_check(struct monst *, boolean, int, int);
E void digactualhole(int, int, struct monst *, int);
E boolean dighole(boolean, boolean, coord *);
E int use_pick_axe(struct obj *);
E int use_pick_axe2(struct obj *);
E boolean mdig_tunnel(struct monst *);
E void draft_message(boolean);
E void watch_dig(struct monst *, xchar, xchar, boolean);
E void zap_try_destroy_tree(int, int);
E void zap_dig(struct obj*);
E void zap_evaporation(struct obj*);
E struct obj *bury_an_obj(struct obj *, boolean *);
E void bury_objs(int, int);
E int unearth_objs(struct monst*, int, int, boolean, boolean);
E int rot_organic(ANY_P *, int64_t);
E int rot_corpse(ANY_P *, int64_t);
E struct obj *buried_ball(coord *);
E void buried_ball_to_punishment(void);
E void buried_ball_to_freedom(void);
E schar fillholetyp(int, int, boolean);
E void liquid_flow(xchar, xchar, schar, struct trap *, const char *);
E boolean conjoined_pits(struct trap *, struct trap *, boolean);
E void uncatch_tree_objects(int, int);
E void reset_dig(void);

#if 0
E void bury_monst(struct monst *);
E void bury_you(void);
E void unearth_you(void);
E void escape_tomb(void);
E void bury_obj(struct obj *);
#endif

/* ### display.c ### */

E void magic_map_background(xchar, xchar, int);
E void map_background(xchar, xchar, int);
E void map_trap(struct trap *, int);
E void map_object(struct obj *, int);
E void map_object_for_detection(struct obj*, int);
E void map_object_no_chain_check(struct obj*, int);
E void map_object_no_chain_check_choose_ascii(struct obj*, int, boolean);
E void map_object_core(struct obj*, int, boolean, boolean, boolean);
E void map_invisible(xchar, xchar);
E boolean unmap_invisible(int, int);
E boolean unmap_invisible_with_animation(int, int, int);
E void unmap_object(int, int);
E void clear_monster_layerinfo(struct layer_info*);
E void map_location(int, int, int);
E void feel_newsym(xchar, xchar);
E void feel_location(xchar, xchar);
E void newsym(int, int);
E void newsym_with_flags(int, int, uint64_t);
E void newsym_with_extra_info_and_flags(int, int, uint64_t, uint64_t, int, int, uint64_t);
E void newsym_force(int, int);
E void force_redraw_at(int, int);
E void shieldeff(xchar, xchar);
E void talkeff(xchar, xchar);
E void tmp_at(int, int);
E void tmp_at_with_missile_flags(int, int, uint64_t, uchar, short);
E void tmp_at_with_obj(int, int, struct obj*, uint64_t, uchar, short);
E void swallowed(int);
E void under_ground(int);
E void under_water(int);
E void see_monsters(void);
E void set_mimic_blocking(void);
E void see_objects(void);
E void see_traps(void);
E void curs_on_u(void);
E int doredraw(void);
E void docrt(void);
E void clear_memory_object_detection_marks(void);
E void redraw_map(void);
E void show_glyph(int, int, int);
E void clear_current_and_show_glyph(int, int, int);
E void maybe_clear_and_show_glyph(int, int, int, boolean);
E void show_gui_glyph_on_layer(int, int, int, int, enum layer_types);
E void show_glyph_on_layer(int, int, int, enum layer_types);
E void show_glyph_ascii(int, int, int);
E void show_glyph_on_layer_and_ascii(int, int, int, enum layer_types);
E void show_gui_glyph_on_layer_and_ascii(int, int, int, int, enum layer_types);
E void clear_glyph_buffer_at(int, int);
E void show_monster_glyph_with_extra_info(int, int, int, struct monst*, uint64_t, uint64_t, int, int);
E void show_monster_glyph_with_extra_info_choose_ascii(int, int, int, struct monst*, int, int, uint64_t, uint64_t, int, int, boolean);
E void clear_monster_extra_info(int, int);
E void clear_monster_layer_at(int, int);
E void clear_monster_layer_memory_at(int, int);
E void show_extra_info(int, int, uint64_t, uint64_t, int, int);
E void show_missile_info(int, int, uchar, uchar, short, uchar, uchar, uchar, uchar, uchar, uchar, uint64_t, short, xchar, xchar);
E uint64_t get_missile_flags(struct obj*, boolean);
E void show_leash_info(int, int, xchar, xchar, xchar, xchar);
E void clear_layer_info(struct layer_info*);
E void clear_all_glyphs_at(int, int);
E void clear_hero_memory_at(int, int);
E void clear_object_glyphs_at(int, int);
E void change_layer_damage_displayed(int, int, int);
E void change_layer_hit_tile(int, int, int);
E void add_glyph_buffer_layer_flags(int, int, uint64_t, uint64_t);
E void remove_glyph_buffer_layer_flags(int, int, uint64_t, uint64_t);
E void set_glyph_buffer_object_height(int, int, short);
E void set_glyph_buffer_feature_doodad_height(int, int, schar);
E void set_glyph_buffer_oid(int, int, int);
E void add_or_remove_glyph_to_layer(int, int, int, boolean);
E void add_glyph_to_layer(int, int, int);
E void remove_current_glyph_from_layer(int, int);
E void clear_glyph_buffer(void);
E void row_refresh(int, int, int);
E void cls(void);
E void show_memory_everywhere(void);
E void show_detection_everywhere(void);
E void show_memory_and_detection_everywhere(void);
E void flush_screen(int);
E int back_to_glyph(xchar, xchar);
E int back_to_broken_glyph(xchar, xchar);
E int zapdir_to_glyph(int, int, int);
E int zapbounce_to_glyph(int, int, int, int, int);
E int zap_glyph_to_corner_glyph(int, uint64_t, int);
E int dir_to_beam_index(int, int);
E int glyph_at(xchar, xchar);
E struct layer_info layers_at(xchar, xchar);
E void set_wall_state(void);
E void unset_seenv(struct rm *, int, int, int, int);
E int warning_of(struct monst *);
E int get_current_cmap_type_index(void);
E int get_missile_index(int, int);
E void display_self_with_extra_info_choose_ascii(uint64_t, uint64_t, int, int, boolean);
E int get_location_light_range(xchar, xchar);
E int get_location_light_sidedness(xchar, xchar);
E void u_shieldeff(void);
E void m_shieldeff(struct monst*);
E boolean use_extra_special_staircase(void);
E int wall_angle(struct rm*);
E short get_obj_height(struct obj*);
E boolean generic_has_floor_tile(struct obj*);
E void update_tile_gui_info(boolean, struct monst*, int, int, uint64_t, uint64_t);
E void refresh_u_tile_gui_info(boolean);
E void refresh_m_tile_gui_info(struct monst*, boolean);
E void set_obj_glyph(struct obj*);
E int get_seen_monster_glyph(struct monst*);
E void reset_display(void);
E const char* get_decoration_description(int, int);
E const char* get_carpet_description(int, int);

/* ### do.c ### */

#ifdef USE_TRAMPOLI
E int drop(struct obj *);
E int wipeoff(void);
#endif
E int dodrop(void);
E int doitemdescriptions(void);
E int docharacterstatistics(void);
E const char* get_damage_type_text(short);
E const char* get_defense_type_text(int);
E const char* get_monster_size_text(int);
E const char* get_attack_type_text(int);
E boolean boulder_hits_pool(struct obj *, int, int, boolean);
E boolean flooreffects(struct obj *, int, int, const char *);
E void doaltarobj(struct obj *);
E boolean canletgo(struct obj *, const char *);
E int drop(struct obj*);
E boolean dropx(struct obj *);
E boolean dropxf(struct obj*);
E boolean dropy(struct obj *);
E boolean dropyf(struct obj*);
E boolean dropz(struct obj *, boolean);
E void obj_no_longer_held(struct obj *);
E int doddrop(void);
E int dodropmany(void);
E int doautostash(void);
E int autobag(struct obj*);
E int dodown(void);
E int doup(void);
#ifdef INSURANCE
E void save_currentstate(void);
#endif
E void u_collide_m(struct monst *);
E void goto_level(d_level *, uchar, boolean, boolean, xchar);
E void schedule_goto(d_level *, uchar, boolean, boolean, boolean, int64_t,
                             const char *, const char *);
E void deferred_goto(void);
E void revival_at_altar(char*);
E void revival_popup_message(char*);
E boolean revive_corpse(struct obj *);
E boolean animate_corpse(struct obj*, int);
E int revive_mon(ANY_P *, int64_t);
E int donull(void);
E int dowipe(void);
E void set_wounded_legs(int64_t, int);
E void heal_legs(int);
E boolean floorexamine(void);
E int itemdescription(struct obj*);
E int itemdescription_core(struct obj*, int, struct item_description_stats*);
E int corpsedescription(struct obj*);
E void printweight(char*, int, boolean, boolean, boolean);
E void printdice(char*, int, int, int);
E int monsterdescription(struct monst*);
E int monsterdescription_core(struct monst*, struct permonst*);
E int dotogglehpbars(void);
E int dotogglegrid(void);
E int dotogglemonstertargeting(void);
E int dotoggleumark(void);
E int dotogglebufftimers(void);
E void delete_location(xchar, xchar);
E void delete_decoration(xchar, xchar);
E void full_location_transform(xchar, xchar, int, int, int, unsigned short, schar, schar, uchar, schar, schar, schar, uchar, int, int, int, int, boolean, boolean, short, short, boolean);
E void full_initial_location_transform(xchar, xchar, int, unsigned short, schar, schar, uchar, schar, schar, schar, uchar, int, int, boolean, boolean, short, short, boolean);
E void create_simple_location(xchar, xchar, int, int, int, unsigned short, int, int, int, int, boolean);
E void create_simple_initial_location(xchar, xchar, int, unsigned short, int, int, boolean);
E void create_simple_location_with_carpet(xchar, xchar, int, int, int, unsigned short, schar, schar, uchar, int, int, int, int, boolean);
E void create_location_with_current_floor(xchar, xchar, int, int, int, unsigned short, int, boolean);
E void create_initial_location_with_current_floor(xchar, xchar, int, unsigned short, int, boolean);
E void create_current_floor_location(xchar, xchar, unsigned short, int, boolean);
E void create_basic_floor_location(xchar, xchar, int, int, unsigned short, boolean);
E void transform_location_type(xchar, xchar, int, int);
E void transform_location_type_and_flags(xchar, xchar, int, int, unsigned short);
E void transform_location_type_and_flags_and_set_broken(xchar, xchar, int, int, unsigned short, int);
E const char* get_obj_subtype_name(struct obj*);
E const char* get_otyp_subtype_name(int);
E struct extended_create_window_info extended_create_window_info_from_obj(struct obj*);
E struct extended_create_window_info extended_create_window_info_from_mon(struct monst*);
E struct extended_create_window_info extended_create_window_info_from_mon_with_flags(struct monst*, uint64_t);
E void hint_via_pline(const char*);
E void standard_hint(const char*, boolean*);
E void reviver_hint(struct monst*);
E void brain_hint(struct monst*);
E void grab_hint(struct monst*);
E void pray_hint(const char*, const char*, boolean*);
E void death_hint(void);
E void check_mobbed_hint(void);
E void check_closed_for_inventory_hint(void);
E void item_destruction_hint(int, boolean);
E void heal_ailments_upon_revival(void);
E void convert_dice_to_ranges(char*);
#if !defined (GNH_MOBILE) && defined (DEBUG)
E void write_spells(void);
E void write_monsters(void);
E void write_items(void);
#endif

/* ### do_name.c ### */

E char *coord_desc(int, int, char *, char);
E boolean getpos_menu(coord *, int);
E int getpos(coord *, boolean, const char *, enum game_cursor_types);
E void getpos_sethilite(void (*f)(int), int (*d)(int,int));
E void new_mname(struct monst *, size_t);
E void free_mname(struct monst *);
E void new_umname(struct monst*, size_t);
E void free_umname(struct monst*);
E void new_oname(struct obj *, size_t);
E void free_oname(struct obj *);
E const char *safe_oname(struct obj *);
E void new_uoname(struct obj*, size_t);
E void free_uoname(struct obj*);
E const char* safe_uoname(struct obj*);
E struct monst *christen_monst(struct monst *, const char *);
E struct monst* u_name_monst(struct monst*, const char*);
E struct obj *oname(struct obj *, const char *);
E struct obj* uoname(struct obj*, const char*);
E boolean objtyp_is_callable(int);
E int docallcmd(void);
E int doname_specific_object(void);
E int doname_type_of_object(void);
E void docall(struct obj *, const char*);
E const char *rndghostname(void);
E char *x_monnam(struct monst *, int, const char *, int, boolean);
E char *l_monnam(struct monst *);
E char *mon_nam(struct monst *);
E char *noit_mon_nam(struct monst *);
E char* noittame_mon_nam(struct monst*);
E char *Monnam(struct monst *);
E char *noit_Monnam(struct monst *);
E char* noittame_Monnam(struct monst*);
E char *noname_monnam(struct monst *, int);
E char *m_monnam(struct monst *);
E char *y_monnam(struct monst *);
E char* Ymonnam(struct monst*);
E char *Adjmonnam(struct monst *, const char *);
E char *Amonnam(struct monst *);
E char *a_monnam(struct monst *);
E char *distant_monnam(struct monst *, int, char *);
E char *name_possessive_ex(const char *, const char *, const char *);
E char *rndmonnam(char *);
E const char *hcolor(const char *);
E const char* hcolor_multi(const char*, int*, int);
E const char* hcolor_multi_buf0(const char*);
E const char* hcolor_multi_buf1(const char*);
E const char* hcolor_multi_buf2(const char*);
E const char* hcolor_multi_buf3(const char*);
E const char* hcolor_multi_buf4(const char*);
E int color_name_to_nhcolor(const char*);
E const char *rndcolor(void);
E const char *hliquid(const char *);
E const char *roguename(void);
E struct obj *realloc_obj(struct obj *, int, genericptr_t, int, const char *);
E char *coyotename(struct monst *, char *);
E char *rndorcname(char *);
E struct monst *christen_orc(struct monst *, const char *, const char *);
E const char *noveltitle(short *, uint64_t, uint64_t);
E const char* manualtitle(short*, uint64_t, uint64_t);
E const char *lookup_novel(const char *, short*);
E const char* lookup_manual(const char*, short*);
E char* randomize_dwarf_name(char*);
E char* randomize_elf_name(char*);
E char* randomize_gnome_name(char*);
E char* randomize_hobbit_name(char*);
E char* randomize_gnoll_name(char*);
E char* randomize_flind_name(char*);
E char* randomize_male_human_name(char*);
E char* randomize_female_human_name(char*);
E char* randomize_demon_name(char*);
E char* randomize_undead_spellcaster_name(char*);
E char* randomize_angel_name(char*);
E char* randomize_modron_name(char*);
#ifdef ANDROID
E void and_getlin_log(const char*, char*);
#endif
E void read_manual(struct obj*);
E void reset_doname(void);

/* ### do_wear.c ### */

#ifdef USE_TRAMPOLI
E int Armor_on(void);
E int Boots_on(void);
E int Gloves_on(void);
E int Helmet_on(void);
E int select_off(struct obj *);
E int select_on(struct obj*);
E int take_off(void);
#endif
E void off_msg(struct obj *);
E void off_msg_with_flags(struct obj*, unsigned);
E void set_wear(struct obj *);
E boolean donning(struct obj *);
E boolean doffing(struct obj *);
E void cancel_doff(struct obj *, int64_t);
E void cancel_don(void);
E int stop_donning(struct obj *);
E int Armor_off(void);
E int Armor_gone(void);
E int Helmet_off(void);
E int Gloves_off(void);
E int Boots_on(void);
E int Boots_off(void);
E int Cloak_off(void);
E int Shield_off(void);
E int Shirt_off(void);
E int Robe_off(void);
E int Bracers_off(void);
E int MiscellaneousItem_off(struct obj *);
E void Amulet_off(void);
E void Ring_on(struct obj *);
E void Ring_off(struct obj *);
E void Ring_gone(struct obj *);
E void Blindf_on(struct obj *);
E void Blindf_off(struct obj *);
E int dotakeoff(void);
E int doremring(void);
E int cursed(struct obj *, boolean);
E int armoroff(struct obj *);
E int canwearobj(struct obj *, int64_t *, boolean, boolean, int*);
E int dowear(void);
E int doputon(void);
E int ddowear(void);
E void find_ac(void);
E void find_mc(void);
E void glibr(void);
E struct obj *some_armor(struct monst *);
E struct obj *stuck_ring(struct obj *, int);
E struct obj *unchanger(void);
E void reset_remarm(void);
E int doddoremarm(void);
E int destroy_arm(struct obj *);
E void adj_abon(struct obj *, schar);
E boolean inaccessible_equipment(struct obj *, const char *, boolean, boolean);
E int get_role_AC_bonus(void);
E int get_role_MC_bonus(void);
E int armor_or_accessory_off(struct obj*);
E void reset_dowear(void);
E void check_wielded_cockatrice(boolean, boolean, boolean);
E void mon_item_change_sex_and_useup(struct monst*, struct obj*, boolean);

/* ### dog.c ### */

E void newedog(struct monst *);
E void free_edog(struct monst *);
E void initedog(struct monst *, boolean);
E struct monst *make_familiar(struct obj *, xchar, xchar, boolean);
E struct monst *makedog(void);
E void update_mlstmv(void);
E void arrival_from_mydogs_and_migrating_mons(void);
E void mon_arrive(struct monst *, boolean);
E void mon_catchup_elapsed_time(struct monst *, int64_t);
E void move_monsters_to_mydogs(boolean, boolean);
E void migrate_to_level(struct monst *, xchar, xchar, coord *);
E int dogfood(struct monst *, struct obj *);
E boolean tamedog(struct monst *, struct obj *, uchar, int, unsigned short, boolean, boolean, const char*);
E void abuse_dog(struct monst *);
E void wary_dog(struct monst *, boolean);
E void break_charm(struct monst*, boolean);
E void reset_dogs(void);

/* ### dogmove.c ### */

E struct obj *droppables(struct monst *);
E struct obj* m_has_wearable_armor_or_accessory(struct monst*);
E struct obj* m_has_worn_armor_or_accessory(struct monst*);
E int dog_nutrition(struct monst *, struct obj *);
E int dog_eat(struct monst *, struct obj *, int, int, boolean);
E int dog_move(struct monst *, int);
#ifdef USE_TRAMPOLI
E void wantdoor(int, int, genericptr_t);
#endif
E void finish_meating(struct monst *);
E boolean could_reach_item(struct monst*, xchar, xchar);
E void m_gainstr(struct monst*, struct obj*, int, boolean);
E void dog_food_after_effect(struct monst*, struct obj*, boolean);
E boolean dog_wants_to_eat(struct monst*);
E void reset_dogmove(void);

/* ### dokick.c ### */

E boolean maybe_kick_monster(struct monst*, xchar, xchar);
E boolean ghitm(struct monst *, struct obj *, uchar *);
E void container_impact_dmg(struct obj *, xchar, xchar);
E int dokick(void);
E int dokick_indir(boolean);
#ifdef ANDROID
E void autokick(void);
#endif
E boolean ship_object(struct obj *, xchar, xchar, boolean);
E void obj_delivery(boolean);
E void deliver_obj_to_mon(struct monst *mtmp, int, uint64_t);
E schar down_gate(xchar, xchar);
E void impact_drop(struct obj *, xchar, xchar, xchar, boolean);
E int kick_object(xchar, xchar, char *, boolean);
E void reset_kick(void);

/* ### dothrow.c ### */

E int dothrow(void);
E int dofire(void);
E void endmultishot(boolean);
E boolean hitfloor(struct obj *, boolean);
E void hurtle(int, int, int, boolean);
E void mhurtle(struct monst *, int, int, int);
E void throwit(struct obj *, int64_t);
E int omon_adj(struct monst *, struct obj *, boolean);
E int thitmonst(struct monst *, struct obj *, boolean, uchar *);
E boolean hero_breaks(struct obj *, xchar, xchar, boolean);
E boolean breaks(struct obj *, xchar, xchar);
E void release_camera_demon(struct obj *, xchar, xchar);
E boolean breakobj(struct obj *, xchar, xchar, boolean, boolean);
E boolean breaktest(struct obj *);
E boolean walk_path(coord *, coord *,
                            boolean (*)(genericptr, int, int), genericptr_t);
E boolean hurtle_jump(genericptr_t, int, int);
E boolean hurtle_step(genericptr_t, int, int);

E struct multishot_result get_multishot_stats(struct monst*, struct obj*, struct obj*, uchar);
E void check_shop_obj(struct obj*, xchar, xchar, boolean, boolean);
E void reset_throw(void);

/* ### do_wear.c ### */
E void learnring(struct obj*, boolean);



/* ### drawing.c ### */
#endif /* !MAKEDEFS_C && !LEV_LEX_C */
E int def_char_to_objclass(char);
E int def_char_to_monclass(char);
#if !defined(MAKEDEFS_C) && !defined(LEV_LEX_C)
E void switch_symbols(int);
E void assign_graphics(int);
E void init_r_symbols(void);
E void init_symbols(void);
E void update_bouldersym(void);
E void init_showsyms(void);
E void init_l_symbols(void);
E void clear_symsetentry(int, boolean);
E void update_l_symset(struct symparse *, nhsym);
E void update_r_symset(struct symparse *, nhsym);
E boolean cursed_object_at(int, int);
E const char* get_cmap_tilename(int);

/* ### dungeon.c ### */

E void save_dungeon(int, boolean, boolean);
E void reset_dungeon(void);
E void restore_dungeon(int);
E void insert_branch(branch *, boolean);
E void init_dungeons(void);
E s_level *find_level(const char *);
E s_level *Is_special(d_level *);
E branch *Is_branchlev(d_level *);
E branch* get_current_branch(d_level*);
E boolean builds_up(d_level *);
E xchar ledger_no(d_level *);
E xchar maxledgerno(void);
E schar depth(d_level *);
E xchar dunlev(d_level *);
E xchar dunlevs_in_dungeon(d_level *);
E xchar ledger_to_dnum(xchar);
E xchar ledger_to_dlev(xchar);
E xchar deepest_lev_reached(boolean);
E boolean on_level(d_level *, d_level *);
E void next_level(boolean);
E void prev_level(boolean);
E void u_on_newpos(int, int);
E void u_on_rndspot(int);
E void u_on_sstairs(int);
E void u_on_upstairs(void);
E void u_on_dnstairs(void);
E boolean On_stairs(xchar, xchar);
E void get_level(d_level *, int);
E void get_level_in_dungeon(d_level*, int, xchar);
E boolean Is_botlevel(d_level *);
E boolean Can_fall_thru(d_level *);
E boolean Can_dig_down(d_level *);
E boolean Can_rise_up(int, int, d_level *);
E boolean has_ceiling(d_level *);
E boolean In_quest(d_level *);
E boolean In_mines(d_level *);
E boolean In_modron_level(d_level*);
E boolean In_bovine_level(d_level*);
E boolean In_large_circular_dgn_level(d_level*);
E branch *dungeon_branch(const char *);
E boolean at_dgn_entrance(const char *);
E boolean In_hell(d_level *);
E boolean In_V_tower(d_level *);
E boolean On_W_tower_level(d_level *);
E boolean In_W_tower(int, int, d_level *);
E void find_hell(d_level *);
E void goto_hell(boolean, boolean);
E void assign_level(d_level *, d_level *);
E void assign_rnd_level(d_level *, d_level *, int);
E int induced_align(int);
E boolean Invocation_lev(d_level *);
E xchar level_difficulty(void);
E schar lev_by_name(const char *);
E schar print_dungeon(boolean, schar *, xchar *);
E char *get_annotation(d_level *);
E int donamelevel(void);
E int dooverview(void);
E void show_overview(int, int);
E void forget_mapseen(int);
E void rm_mapseen(int);
E void init_mapseen(d_level *);
E void recalc_mapseen(void);
E void mapseen_temple(struct monst *);
E void room_discovered(int);
E void recbranch_mapseen(d_level *, d_level *);
E void overview_stats(winid, const char *, int64_t *, size_t *);
E void remdun_mapseen(int);
E const char *endgamelevelname(char *, int);
E mapseen* find_mapseen(d_level*);
E void check_special_level_naming_by_mon(struct monst*);
E void set_special_level_seen(d_level*, boolean);

/* ### eat.c ### */

#ifdef USE_TRAMPOLI
E int eatmdone(void);
E int eatfood(void);
E int opentin(void);
E int unfaint(void);
#endif
E void eatmupdate(void);
E boolean is_edible(struct obj *);
E const char* is_nonedible_corpse_material(struct permonst*, struct obj*);
E void init_uhunger(void);
E int Hear_again(void);
E void reset_eat(void);
E boolean is_obj_vegan_food(struct obj*);
E int check_vegan_food(struct obj*);
E int doeat(void);
E int use_tin_opener(struct obj *);
E double calchungry(boolean*);
E void gethungry(void);
E void morehungry(int);
E void lesshungry(int);
E boolean is_fainted(void);
E void reset_faint(void);
E void violated_vegetarian(void);
E void update_hunger_status(boolean);
E struct obj *floorfood(const char *, int);
E void vomit(void);
E int eaten_stat(int, struct obj *);
E void food_disappears(struct obj *);
E void food_substitution(struct obj *, struct obj *);
E void eating_conducts(struct permonst *);
E int eat_brains(struct monst *, struct monst *, boolean, double *);
E void fix_petrification(void);
E void consume_oeaten(struct obj *, int);
E boolean maybe_finished_meal(boolean);
E void set_tin_variety(struct obj *, int);
E int tin_variety_txt(char *, int *);
E void tin_details(struct obj *, int, char *);
E boolean Popeye(int);
E unsigned obj_nutrition(struct obj*);
E unsigned mon_obj_nutrition_value(struct obj*, struct monst*);
E unsigned mon_obj_oeaten_value(struct obj*, struct monst*);
E unsigned mon_nutrition_factor(struct obj*, struct monst*, boolean);
E int adjust_nutrition_base_value(int, struct obj*, struct monst*);
E void display_nutrition_floating_text(int, int, int);
E void display_famine_floating_text(int, int, int);
E int64_t get_rotted_status(struct obj*);
E void reset_hunger_status(void);
E boolean intrinsic_possible(int, struct permonst*);
E int magic_nutrition(struct obj*);

/* ### encounter.c ### */

E void encounter_init(void);
E void randomize_encounter(int, int);
E void create_encounter(int, int, int, int);
#if !defined(GNH_MOBILE) && defined(DEBUG)
E int wiz_save_encounters(void);
#endif

/* ### end.c ### */

E void done1(int);
E int done2(void);
#ifdef USE_TRAMPOLI
E void done_intr(int);
#endif
E void get_killer_name_and_format(struct monst*, char*, int*);
E void done_in_by(struct monst *, int);
#endif /* !MAKEDEFS_C && !LEV_LEX_C */
E void set_panic_handling(int, boolean);
E void panic(const char *, ...) PRINTF_F(1, 2) NORETURN;
#if !defined(MAKEDEFS_C) && !defined(LEV_LEX_C)
E void done(int);
E void container_contents(struct obj *, boolean, boolean, boolean, int, boolean);
E void magic_chest_contents(boolean, boolean, boolean, int, boolean);
#ifdef VMS
E void nh_terminate(int);
E void nh_bail(int, const char*, boolean);
#else
E void nh_terminate(int) NORETURN;
E void nh_bail(int, const char*, boolean) NORETURN;
#endif
E int dovanquished(void);
E int dokilledmonsters(void);
E int dogenocidedmonsters(void);
E int num_genocides(void);
E void delayed_killer(int, int, const char *, int);
E struct kinfo *find_delayed_killer(int);
E void dealloc_killer(struct kinfo *);
E void save_killers(int, int);
E void reset_killers(void);
E void restore_killers(int);
E char *build_english_list(char *);
#if defined(PANICTRACE) && !defined(NO_SIGNAL)
E void panictrace_setsignals(boolean);
#endif
#ifdef ANDROID
void and_you_die();
#endif
E int64_t get_current_game_score(void);
E int64_t get_conduct_score_upon_ascension(void);
E int wiz_dumplog(void);
E int dosnapshot(void);
E void print_selfies(winid, int);
E void print_knight_slayings(winid, int);
E void reset_game(void);
E void reset_gamestate_ex(void);
E void tally_realtime(void);
E struct item_score_count_result count_powerful_melee_weapon_score(struct obj*);
E struct item_score_count_result count_powerful_ranged_weapon_score(struct obj*);
E struct item_score_count_result count_powerful_Japanese_item_score(struct obj*);
E struct item_score_count_result count_powerful_valkyrie_item_score(struct obj*);
E struct amulet_count_result count_amulets(struct obj*);
E struct item_score_count_result count_artifacts(struct obj*);
E struct item_score_count_result count_historic_statues(struct obj*);
E struct item_score_count_result count_sarcophaguses(struct obj*);
E struct item_score_count_result count_mummy_wrappings(struct obj*);
E struct item_score_count_result count_valuable_art_objects(struct obj*);
E int64_t count_archaeologist_item_score(struct obj*);
E void recalculate_knight_slaying_score(void);

/* ### engrave.c ### */

E char *random_engraving(char *);
E void wipeout_text(char *, int, unsigned);
E boolean can_reach_floor(boolean);
E void cant_reach_floor(int, int, boolean, boolean);
E const char *surface(int, int);
E const char *ceiling(int, int);
E struct engr *engr_at(xchar, xchar);
E int sengr_at(const char *, xchar, xchar, boolean);
E void u_wipe_engr(int);
E void wipe_engr_at(xchar, xchar, xchar, boolean);
E void read_engr_at(int, int);
E void make_engr_at(int, int, const char *, int64_t, xchar, unsigned short);
E void del_engr_at(int, int);
E int freehand(void);
E int doengrave(void);
E int doengravequick(void);
E void sanitize_engravings(void);
E void save_engravings(int, int);
E void reset_engravings(void);
E void rest_engravings(int);
E void engr_stats(const char *, char *, int64_t *, size_t *);
E void del_engr(struct engr *);
E void rloc_engr(struct engr *);
E void make_grave(int, int, const char *, boolean);
E void make_signpost(int, int, const char*, boolean);
E size_t count_engravings(void);
E coord* get_engraving_coords(size_t*);
E void unsee_engravings(void);
E void forget_engravings(void);

/* ### exper.c ### */

E int64_t newuexp(int);
E int newpw(void);
E int enmaxadjustment(void);
E void updatemaxen(void);
E int experience(struct monst *, int);
E void more_experienced(int, int);
E int64_t game_score_difficulty_adjustment(int64_t);

E void losexp(const char *);
E void newexplevel(void);
E void pluslvl(boolean);
E int64_t rndexp(boolean);

/* ### explode.c ### */

E void explode(int, int, int, struct monst*, int, int, int, int, char, int);
E int64_t scatter(int, int, int, unsigned int, struct obj *);
E void splatter_burning_oil(int, int, boolean);
E void explode_oil(struct obj *, int, int);
E void play_explosion_animation_at(int, int, enum explosion_types);
E void explosion_wait_until_action(void);
E void explosion_wait_until_end(void);

/* ### extralev.c ### */

E void makeroguerooms(void);
E void corr(int, int);
E void makerogueghost(void);

/* ### files.c ### */

E char *fname_encode(const char *, char, char *, char *, int);
E char *fname_decode(char, char *, char *, int);
E const char *fqname(const char *, int, int);
E FILE *fopen_datafile(const char *, const char *, int);
#ifdef MFLOPPY
E void set_lock_and_bones(void);
#endif
E void set_levelfile_name(char *, int);
E int create_levelfile(int, char *);
E int open_levelfile(int, char *);
E void delete_levelfile(int);
E void delete_excess_levelfiles(void);
E void clearlocks(void);
E int create_bonesfile(d_level *, char **, char *);
#ifdef MFLOPPY
E void cancel_bonesfile(void);
#endif
E void commit_bonesfile(d_level *);
E int open_bonesfile(d_level *, char **);
E int delete_bonesfile(d_level *);
E void compress_bonesfile(void);
E void set_savefile_name(boolean);
#ifdef INSURANCE
E void save_savefile_name(int);
#endif
#ifndef MICRO
E void set_error_savefile(void);
E void set_imported_savefile(void);
#endif
E int create_savefile(void);
E int open_savefile(void);
E int open_savefilepath(const char*);
E int delete_savefile(void);
E int ask_delete_invalid_savefile(const char*, boolean);
E int query_about_corrupted_savefile(void);
E int open_and_validate_saved_game(boolean, boolean*);
E void mode_message(void);
E void create_gamestate_levelfile(void);
E int load_saved_game(int);
E void nh_compress(const char *);
E void nh_uncompress(const char *);
E boolean lock_file(const char *, int, int);
E void unlock_file(const char *);
#ifdef USER_SOUNDS
E boolean can_read_file(const char *);
#endif
E void config_error_init(boolean, const char *, boolean);
E void config_erradd(const char *);
E int config_error_done(void);
E boolean read_config_file(const char *, int);
E void check_recordfile(const char *);
E void read_wizkit(void);
E int read_sym_file(int);
E int parse_sym_line(char *, int);
E void paniclog(const char *, const char *);
E void testinglog(const char *, const char *, const char *);
E int validate_prefix_locations(char *);
#ifdef SELECTSAVED
E char *plname_from_file(const char *, struct save_game_stats*);
E struct save_game_data newsavegamedata(char*, char*, struct save_game_stats, boolean, boolean, boolean);
#endif
E struct save_game_data *get_saved_games(void);
E void free_saved_games(struct save_game_data *);
E boolean check_saved_game_exists(void);
E int make_tmp_backup_savefile_from_uncompressed_savefile(const char*);
E int move_tmp_backup_savefile_to_actual_backup_savefile(void);
E int restore_backup_savefile(boolean);
E int delete_backup_savefile(void);
E int delete_tmp_backup_savefile(void);
E boolean check_has_backup_savefile(void);
E int delete_savefile_if_exists(void);
E int delete_error_savefile(void);
E int delete_running_files(void);
E boolean check_existing_save_file(void);
E boolean check_existing_error_save_file(void);

#ifdef SELF_RECOVER
E boolean recover_savefile(void);
#endif
#ifdef SYSCF_FILE
E void assure_syscf_file(void);
#endif
E int nhclose(int);
#ifdef HOLD_LOCKFILE_OPEN
E void really_close(void);
#endif
#ifdef DEBUG
E boolean debugcore(const char *, boolean);
#endif
E boolean read_tribute(const char *, const char *, int,
                               char *, int, unsigned);
E boolean Death_quote(char *, int);
E void list_files(void);
E void reset_save_file_name(boolean);
E void reset_files(void);
E void reset_gamelog(void);
E void show_gamelog(int);
E void gamelog_add(int64_t, int64_t, const char*);
E void livelog_printf(unsigned int, const char*, ...) PRINTF_F(2, 3);
E void livelog_dump_url(unsigned int);
E void post_to_forum_printf(unsigned int, const char*, ...) PRINTF_F(2, 3);
E void post_to_forum_rt_printf(unsigned int, struct u_realtime, const char*, ...) PRINTF_F(3, 4);
#ifdef WHEREIS_FILE
E void touch_whereis(void);
E void delete_whereis(void);
E void signal_whereis(int);
#endif
#ifdef EXTRAINFO_FN
E void mk_dgl_extrainfo(void);
#endif
#ifdef WISH_TRACKER
E void trackwish(char*);
#endif
E int check_current_fd(const char*);
E void maybe_report_file_descriptors(const char*, int);
#if defined(UNIX) && defined(GNH_MOBILE)
E char* gnh_lsof(void);
E boolean increase_file_descriptor_limit_to_at_least(uint64_t);
E int get_file_descriptor_limit(boolean);
#endif
E void make_dumplog_dir(void);

/* ### fountain.c ### */

E void floating_above(const char *);
E void dogushforth(int);
#ifdef USE_TRAMPOLI
E void gush(int, int, genericptr_t);
#endif
E void dryup(xchar, xchar, boolean);
E void drinkfountain(void);
E void dipfountain(struct obj *);
E void breaksink(int, int);
E void drinksink(void);
E const char* fountain_type_text(int);
E const char* get_fountain_name(int, int);
E void init_fountains(void);

/* ### gnhmain.c ### */

#if defined(GNH_MOBILE)
#ifdef PORT_HELP
E void port_help(void);
#endif
E boolean authorize_wizard_mode(void);
#endif /* GNH_MOBILE */

/* ### gnhunix.c and gnhwin.c ### */

#if defined(GNH_MOBILE)
E void getlock(void);
E void check_crash(void);
E void thread_lock_init(void);
E void thread_lock_destroy(void);
E void thread_lock_lock(void);
E void thread_lock_unlock(void);
#endif /* GNH_MOBILE */

/* ### hack.c ### */

E boolean is_valid_travelpt(int,int);
E anything *uint_to_any(unsigned);
E anything *long_to_any(long);
E anything *monst_to_any(struct monst *);
E anything *obj_to_any(struct obj *);
E anything* trap_to_any(struct trap*);
E anything* coord_to_any(coord);
E anything* region_to_any(struct nhregion*);
E anything* xy_to_any(xchar, xchar);
E boolean revive_nasty(int, int, const char *);
E void movobj(struct obj *, xchar, xchar);
E boolean may_dig(xchar, xchar);
E boolean may_passwall(xchar, xchar);
E boolean bad_rock(struct permonst *, xchar, xchar);
E int cant_squeeze_thru(struct monst *);
E boolean invocation_pos(xchar, xchar);
E boolean test_move(int, int, int, int, int);
#ifdef DEBUG
E int wiz_debug_cmd_traveldisplay(void);
#endif
E boolean u_rooted(void);
E void domove(void);
E boolean overexertion(void);
E void invocation_message(void);
E void switch_terrain(void);
E boolean pooleffects(boolean);
E void spoteffects(boolean);
E char *in_rooms(xchar, xchar, int);
E boolean in_town(int, int);
E void check_special_room(boolean);
E int dopickup(void);
E int doput2bag(void);
E void lookaround(void);
E boolean crawl_destination(int, int);
E int monster_nearby(void);
E void nomul(int);
E void unmul(const char*);
E void unmul_ex(int, int, const char *);
E void you_die(const char*, int);
E void kill_player(const char*, int);
E void losehp(double, const char *, int);
E void losehp_core(double, const char*, int, boolean);
E int weight_cap(void);
E int inv_weight(void);
E int near_capacity(void);
E int calc_capacity(int);
E int max_capacity(void);
E boolean check_capacity(const char *);
E int inv_cnt(boolean);
E int inv_cnt_ex(boolean, boolean);
E int64_t money_cnt(struct obj *);
E struct extended_menu_info obj_to_extended_menu_info(struct obj*);
E struct extended_menu_info menu_heading_info(void);
E struct extended_menu_info active_menu_info(void);
E struct extended_menu_info menu_group_heading_info(char);
E struct extended_menu_info menu_special_mark_info(char);
E const char* get_cmap_or_cmap_variation_glyph_explanation(int);
E void clear_run_and_travel(void);
E void mark_spotted_monsters_in_run(void);
E void reset_hack(void);

/* ### hacklib.c ### */

E boolean digit(char);
E boolean letter(char);
E char highc(char);
E char lowc(char);
E char *lcase(char *);
E char *ucase(char *);
E char *upstart(char *);
E char *mungspaces(char *);
E char *trimspaces(char *);
E char *strip_newline(char *);
E char *stripchars(char *, const char *, const char *);
E char *eos(char *);
E boolean str_end_is(const char *, const char *);
E char *strkitten(char *, char);
E void copynchars(char *, const char *, int);
E char chrcasecpy(int, int);
E char *strcasecpy(char *, const char *);
E char* strcpy_capitalized_for_title(char*, const char*);
E char *s_suffix(const char *);
E char *ing_suffix(const char *);
E char *xcrypt(const char *, char *);
E boolean onlyspace(const char *);
E char *tabexpand(char *);
E char *visctrl(char);
E char *strsubst(char *, const char *, const char *);
E int strNsubst(char *, const char *, const char *, int);
E const char *ordin(int);
E char *sitoa(int);
E int sgn(int);
E int rounddiv(int, int);
E int dist2(int, int, int, int);
E int isqrt(int);
E int distmin(int, int, int, int);
E boolean online2(int, int, int, int);
E boolean pmatch(const char *, const char *);
E boolean pmatchi(const char *, const char *);
E boolean pmatchz(const char *, const char *);
#ifndef STRNCMPI
E int strncmpi(const char *, const char *, size_t);
#endif
#ifndef STRSTRI
E char *strstri(const char *, const char *);
#endif
E boolean
fuzzymatch(const char *, const char *, const char *, boolean);
E void init_random(int (*fn)(int));
E void reseed_random(int (*fn)(int));
E time_t getnow(void);
E time_t convert2UTC(time_t);
E time_t getUTC(void);
E int getyear(void);
#if 0
E char *yymmdd(time_t);
#endif
E void print_realtime(char*, int64_t);
E int64_t yyyymmdd(time_t);
E int64_t hhmmss(time_t);
E char *yyyymmddhhmmss(time_t);
E time_t time_from_yyyymmddhhmmss(char *);
E int phase_of_the_moon(void);
E boolean friday_13th(void);
E int night(void);
E int midnight(void);
E void strbuf_init(strbuf_t *);
E void strbuf_append(strbuf_t *, const char *);
E void strbuf_reserve(strbuf_t *, size_t);
E void strbuf_empty(strbuf_t *);
E void strbuf_nl_to_crlf(strbuf_t *);
E void adjusted_delay_output(void);
E void copyUTF8toCP437(char*, size_t, char*, size_t);
E void convertUTF8toCP437(char*, size_t);
E char unicode_to_CP437(nhsym);
E boolean use_utf8_encoding(void);
E int converted_nh_poskey(int*, int*, int*);
E int tty_arrow_key_support_enabled(void);
E void write_nhsym_utf8(char**, nhsym, boolean, boolean);
E void write_text2buf_utf8(char*, size_t, const char*);
E void write_CP437_to_buf_unicode(char*, size_t, const char*);
E void write_gui_debuglog(const char*);
E int hl_attridx_to_attrmask(int);
E int hl_attrmask_to_attridx(int);
E int hl_attridx_to_atr(int);
E int hl_attrmask_to_atr(int);

/* ### invent.c ### */

E Loot *sortloot(struct obj **, unsigned, boolean,
                         boolean (*)(OBJ_P));
E void unsortloot(Loot **);
E void assigninvlet(struct obj *);
E struct obj *merge_choice(struct obj *, struct obj *);
E boolean merged(struct obj **, struct obj **);
#ifdef USE_TRAMPOLI
E int ckunpaid(struct obj *);
#endif
E void addinv_core1(struct obj *);
E void addinv_core2(struct obj *);
E void update_all_character_properties(struct obj*, boolean);
E struct obj *addinv(struct obj *);
E struct obj *hold_another_object(struct obj *, const char *, const char *, const char *, boolean);
E void useupall(struct obj *);
E void useup(struct obj *);
E void consume_obj_charge(struct obj *, boolean);
E void freeinv_core(struct obj *);
E void freeinv(struct obj *);
E void delallobj(int, int);
E void delobj(struct obj *);
E void delobj_with_flags(struct obj*, uint64_t);
E struct obj *sobj_at(int, int, int);
E struct obj* noncursed_sobj_at(int, int, int);
E struct obj* any_obj_at(int, int, int);
E struct obj* otyp_in_objchn(int, struct obj*);
E struct obj *nxtobj(struct obj *, int, boolean);
E struct obj *carrying(int);
E struct obj* carrying_class(char);
E struct obj* carrying_leashed_leash(void);
E struct obj* carrying_fitting_unlocking_tool_for_box(struct obj*);
E struct obj* carrying_fitting_unlocking_tool_for_door(struct rm*);
E boolean have_lizard(void);
E struct obj* u_have_novel(void);
E struct obj* o_on(unsigned int, struct obj*);
E struct obj* o_on_memory(unsigned int, struct obj*);
E struct obj* m_on_memory(unsigned int, struct obj*);
E struct obj* o_on_open_inventory(unsigned int);
E void reduce_item_cooldown(struct obj*);
E boolean obj_here(struct obj *, int, int);
E boolean wearing_armor(void);
E boolean is_worn(struct obj *);
E boolean is_worn_correctly(struct obj*);
E boolean is_wearable(struct obj*);
E struct obj *g_at(int, int);
E boolean splittable(struct obj *);
E struct obj *getobj(const char *, const char *, int, const char*);
E struct obj* getobj_ex(const char*, const char*, int, boolean, const char*, boolean (*)(struct obj*), int64_t, unsigned int);
void construct_getobj_letters(const char*, const char*, boolean(*)(struct obj*), char*, char*, char*, size_t, size_t, size_t, int*, xchar*, char**, xchar*, boolean*, boolean*, boolean*, boolean*, struct obj*);

E boolean acceptable_getobj_obj(struct obj*, const char*, const char*);
E int ggetobj(const char *, int (*)(OBJ_P), int,
                      boolean, unsigned *, int, boolean);
E int askchain(struct obj **, const char *, int, int (*)(OBJ_P),
                       int (*)(OBJ_P), int, const char *);
E void fully_identify_obj(struct obj *);
E int identify(struct obj *);
E int count_unidentified(struct obj *, boolean(*)(OBJ_P), boolean);
E int count_unknown(struct obj*, boolean(*)(OBJ_P), boolean);
E int count_objects_in_class(struct obj*, char, boolean(*)(OBJ_P), boolean);
E int identify_pack(int, boolean);
E void learn_unseen_invent(void);
E void prinv(const char *, struct obj *, int64_t);
E void prinvc(const char*, struct obj*, int64_t);
E void prinv_ex(const char*, struct obj*, int64_t, int, int, int, int, boolean, boolean);
E char *xprname(struct obj *, const char *, char, boolean, int64_t, int64_t);
E int ddoinv(void);
E int doseeworn(void);
E int dolastpickeditem(void);
E int display_item_command_menu_by_invlet(char, int64_t, boolean*);
E int display_item_command_menu(struct obj*, int64_t, boolean*);
E char display_inventory(const char *, boolean, int, boolean);
E char display_inventory_with_header(const char*, boolean, int64_t*, int, boolean, boolean, boolean);
E int display_binventory(int, int, boolean);
E struct obj *display_cinventory(struct obj *);
E struct obj *display_minventory(struct monst *, int, char *);
E int dotypeinv(void);
E const char *dfeature_at(int, int);
E const char* adjusted_dfeature_at(int, int);
int here_window_display_style(const char*, struct engr*, struct obj*);
E const char* get_floor_doodad_explanation_at(int, int);
E int look_here(int, boolean, boolean);
E int dolook(void);
E boolean will_feel_cockatrice(struct obj *, boolean);
E void feel_cockatrice(struct obj *, boolean);
E void stackobj(struct obj *);
E boolean mergable(struct obj *, struct obj *);
E int doprgold(void);
E int doprwep(void);
E int doprarm(void);
E int doprring(void);
E int dopramulet(void);
E int doprtool(void);
E int doprinuse(void);
E void useupf(struct obj *, int64_t);
E void useupf_with_flags(struct obj*, int64_t, uint64_t);
E char *let_to_name(char, uchar, boolean);
E const char* get_class_name(char);
E void free_invbuf(void);
E void reassign(void);
E int doorganize(void);
E void free_pickinv_cache(void);
E int count_unpaid(struct obj *, boolean(*)(OBJ_P), boolean);
E int count_objects(struct obj*, boolean(*)(OBJ_P), boolean);
E int count_buc(struct obj *, int, boolean (*)(OBJ_P), boolean);
E void tally_BUCX(struct obj *, boolean,
                          int *, int *, int *, int *, int *, int *);
E int64_t count_contents(struct obj *, boolean, boolean, boolean);
E int64_t count_contained_contents(struct obj*, boolean, boolean, boolean);
E void carry_obj_effects(struct obj *);
E const char *currency(int64_t);
E void silly_thing(const char *, struct obj *);
E void print_things_here_to_window(void);
E void update_here_window(void);
E void reset_inventory(void);
E struct obj* contains_otyp(struct obj*, int);
E void invocation_ritual_quest_update(boolean);
E void play_hint_sound_with_delay(boolean);

/* ### ioctl.c ### */

#if (defined(UNIX) || defined(__BEOS__))
E void getwindowsz(void);
E void getioctls(void);
E void setioctls(void);
#ifdef SUSPEND
E int dosuspend(void);
#endif /* SUSPEND */
#endif /* UNIX || __BEOS__ */


/* ### light.c ### */

E void new_light_source(xchar, xchar, int, int, ANY_P *, int);
E void del_light_source(int, ANY_P *);
E void do_light_sources(char **);
E struct monst* find_mid_ew(unsigned);
E struct monst *find_mid(unsigned, unsigned);
E void save_light_sources(int, int, int);
E void reset_light_sources(void);
E void restore_light_sources(int);
E void light_stats(const char *, char *, int64_t *, size_t *);
E void relink_light_sources(boolean);
E void light_sources_sanity_check(void);
E boolean any_light_source(void);
E void snuff_light_source(int, int);
E boolean obj_sheds_light(struct obj *);
E boolean obj_is_burning(struct obj *);
E void obj_split_light_source(struct obj *, struct obj *);
E void obj_merge_light_sources(struct obj *, struct obj *);
E void obj_adjust_light_radius(struct obj *, int);
E int candle_light_range(struct obj *);
E int artifact_light_range(struct obj*);
E int current_arti_light_radius(struct obj *);
E const char *arti_light_description(struct obj *);
E int wiz_light_sources(void);
E int obj_light_radius(struct obj*);
E int64_t obj_light_maximum_burn_time(struct obj*);
E int64_t obj_light_burn_time_left(struct obj*);

/* ### lock.c ### */

#ifdef USE_TRAMPOLI
E int forcelock(void);
E int picklock(void);
#endif
E boolean picking_lock(int *, int *);
E boolean picking_at(int, int);
E void breakchestlock(struct obj *, boolean);
E void reset_pick(void);
E void maybe_reset_pick(struct obj *);
E int pick_lock(struct obj *);
E int pick_lock_core(struct obj*, int, int, boolean);
#ifdef ANDROID
E boolean can_try_force(void);
#endif
E int doforce(void);
E boolean boxlock(struct obj *, struct obj *);
E boolean doorlock(struct obj *, int, int);
E int doopen(void);
E boolean stumble_on_door_mimic(int, int);
E int doopen_indir(int, int);
E int doclose(void);
#ifdef ANDROID
E void lock_mouse_cursor(boolean);
#endif
E boolean key_fits_the_box_lock(struct obj*, struct obj*);
E boolean key_fits_the_door_lock(struct obj*, struct rm*);

#ifdef MAC
/* These declarations are here because the main code calls them. */

/* ### macfile.c ### */

E int maccreat(const char *, int64_t);
E int macopen(const char *, int, int64_t);
E int macclose(int);
E int macread(int, void *, unsigned);
E int macwrite(int, void *, unsigned);
E int64_t macseek(int, int64_t, short);
E int macunlink(const char *);

/* ### macmain.c ### */

E boolean authorize_wizard_mode(void);

/* ### macsnd.c ### */

E void mac_speaker(struct obj *, char *);

/* ### macunix.c ### */

E void regularize(char *);
E void getlock(void);

/* ### macwin.c ### */

E void lock_mouse_cursor(Boolean);
E int SanePositions(void);

/* ### mttymain.c ### */

E void getreturn(char *);
E void msmsg(const char *, ...);
E void gettty(void);
E void setftty(void);
E void settty(const char *);
E int tgetch(void);
E void cmov(int x, int y);
E void nocmov(int x, int y);

#endif /* MAC */

/* ### mail.c ### */

#ifdef MAIL
#ifdef UNIX
E void free_maildata(void);
E void getmailstatus(void);
E void ck_server_admin_msg(void);
#endif
E void ckmailstatus(void);
E void readmail(struct obj *);
#endif /* MAIL */

/* ### makemon.c ### */

E void dealloc_monst(struct monst *);
E boolean is_home_elemental(struct permonst *);
E struct monst* clone_mon(struct monst*, xchar, xchar, boolean);
E double monhp_per_lvl(struct monst *);
E int monbasehp_per_lvl(struct monst*);
E double monhpadj_per_lvl(struct monst*);
E void newmonhp(struct monst *, int, int, uint64_t);
E int monhp_difficulty_adjustment(int, int);
E struct mextra *newmextra(void);
E void copy_mextra(struct monst *, struct monst *);
E void dealloc_mextra(struct monst *);
E struct monst *makemon(struct permonst *, int, int, uint64_t);
E struct monst* makemon2(struct permonst*, int, int, uint64_t, uint64_t);
E struct monst* makemon_ex(struct permonst*, int, int, uint64_t, uint64_t, unsigned short, int, int);
E struct monst* makemon_limited(struct permonst*, int, int, uint64_t, uint64_t, unsigned short, unsigned short, int, int, int, aligntyp);
E boolean create_critters(int, struct permonst *, boolean);
E struct permonst *rndmonst(void);
E struct permonst* rndmonst_core(int, int);
E struct permonst* rndmonst_for_polymon(struct monst*);
E void reset_rndmonst(int);
E void get_generated_monster_minmax_levels(int, int*, int*, int, int);
E struct permonst *mkclass(char);
E struct permonst *mkclass_aligned(char, aligntyp, uint64_t);
E struct permonst* mkclass_core(char, int,int, int, aligntyp, int, uint64_t);
E int mkclass_poly(int);
E int adj_lev(struct permonst *, int);
E boolean is_mon_high_level(struct monst*);
E struct permonst *grow_up(struct monst *, struct monst *);
E void change_mon_ability_scores(struct monst*, int, int);
E struct obj* mongets(struct monst *, int);
E struct obj* mongets_with_material(struct monst*, int, uchar);
E struct obj* mongets_spellbook(struct monst*);
E int mongets_return_enchantment(struct monst*, int);
E struct obj* mongets_noinit_item(struct monst*, int,  int);
E int64_t mongetsgold(struct monst*, int64_t);
E int golemhp(int);
E boolean peace_minded(struct permonst *, boolean);
E void set_mhostility(struct monst *);
E void newmcorpsenm(struct monst *);
E void freemcorpsenm(struct monst *);
E void set_mimic_sym(struct monst *);
E void set_rakshasa_appearance(struct monst *);
E int mbirth_limit(int);
E void mimic_hit_msg(struct monst *, short);
E void mkmonmoney(struct monst *, int64_t);
E int bagotricks(struct obj *, boolean, int *);
E boolean propagate(int, boolean, boolean);
E boolean usmellmon(struct permonst *);
E boolean wrong_elem_type(struct permonst*);
E void m_initthrow(struct monst*, int, int, int, boolean, int, int, int);
E boolean randomize_monster_gender(struct permonst*);
E void makemon_animation_wait_until_end(void);
E struct monst* make_level_monster(int, int, uint64_t, uint64_t);
E struct monst* make_level_monster_anywhere(void);
E void reset_makemon(void);
E void newmmonst(struct monst*);
E void free_mmonst(struct monst*);
E void newmobj(struct monst*);
E void free_mobj(struct monst*);
E struct monst* get_mmonst(struct monst*, boolean);
E void save_mmonst(struct monst*, struct monst*);
E struct obj* get_mobj(struct monst*, boolean);
E void save_mobj(struct monst*, struct obj*);
E void set_mimic_new_mobj(struct monst*, int);
E void set_mimic_existing_mobj(struct monst*, struct obj*);

/* ### mapglyph.c ### */

E int mapglyph(struct layer_info, nhsym *, int *, uint64_t*, int, int);
E char *encglyph(int);
E char *decode_mixed(char *, const char *);
E void genl_putmixed_ex(winid, const char *, int, int, int);
E int zap_glyph_to_cmap(int);

/* ### mcastu.c ### */

E int castmu(struct monst *, struct attack *, boolean, boolean);
E int buzzmu(struct monst *, struct attack *);

/* ### mhitm.c ### */

E int fightm(struct monst *, boolean);
E int mattackm(struct monst *, struct monst *);
E boolean engulf_target(struct monst *, struct monst *);
E int mdisplacem(struct monst *, struct monst *, boolean);
E void paralyze_monst(struct monst *, int, boolean);
E int sleep_monst(struct monst *, struct obj *, struct monst*, int, int, int);
E void slept_monst(struct monst *);
E void xdrainenergym(struct monst *, boolean);
E int64_t attk_protection(int);
E void rustm(struct monst *, struct obj *);
E void reset_mhitm(void);

/* ### mhitu.c ### */

E boolean u_slip_free(struct monst*, struct attack*);
E boolean check_stuck_and_slip(struct monst*);
E const char *mpoisons_subj(struct monst *, struct attack *);
//E void u_slow_down(void);
E struct monst *cloneu(void);
E void expels(struct monst *, struct permonst *, boolean);
E struct attack *getmattk(struct monst *, struct monst *,
                                  int, int *, struct attack *);
E int mattacku(struct monst *);
E boolean check_magic_cancellation_success(struct monst*, int);
E boolean check_ability_resistance_success(struct monst*, int, int);
E int magic_negation(struct monst *);
E int magic_negation_percentage(int);
E boolean gulp_blnd_check(void);
E int gazemu(struct monst *, struct attack *);
E void mdamageu(struct monst *, double, boolean);
E void mdamageu_with_hit_tile(struct monst*, double, boolean, enum hit_tile_types);
E int could_seduce(struct monst *, struct monst *, struct attack *);
E int doseduce(struct monst *);
E void update_m_facing(struct monst*, int, boolean);
E int get_pm_attack_index(struct permonst*, struct attack*);
E enum hit_tile_types get_hit_tile_by_adtyp(int);
boolean should_display_m_action_tile(struct monst*, enum action_tile_types action);
E void reset_mhitu(void);

/* ### minion.c ### */

E void newemin(struct monst *);
E void free_emin(struct monst *);
E int monster_census(boolean);
E int msummon(struct monst *);
E int yeenaghu_gnoll_summon(struct monst*);
E int yeenaghu_ghoul_summon(struct monst*);
E int yacc_bison_summon(void);
E int orcus_undead_summon(void);
E void summon_minion(aligntyp, boolean);
E int demon_talk(struct monst *, boolean*);
E int64_t bribe(struct monst *);
E int dprince(aligntyp, boolean, boolean);
E int dlord(aligntyp, boolean, boolean);
E int llord(boolean, boolean);
E int ndemon(aligntyp, boolean, boolean);
E int lminion(boolean, boolean);
E void lose_guardian_angel(struct monst *);
E void gain_guardian_angel(boolean);

/* ### mklev.c ### */

#ifdef USE_TRAMPOLI
E int do_comp(genericptr_t, genericptr_t);
#endif
E void sort_rooms(void);
E void add_room(int, int, int, int, boolean, schar, boolean, int, int, int, int, short);
E void add_subroom(struct mkroom *, int, int, int, int, boolean,
                           schar, boolean, int, int, int, int, short);
E void makecorridors(void);
E void add_door(int, int, struct mkroom *);
E void mklev(void);
#ifdef SPECIALIZATION
E void topologize(struct mkroom *, boolean);
#else
E void topologize(struct mkroom *);
#endif
E void place_branch(branch *, xchar, xchar);
E boolean occupied(xchar, xchar);
E int okdoor(xchar, xchar);
E void dodoor(int, int, struct mkroom *);
E struct trap* mktrap(int, int, struct mkroom *, coord *);
E void mkmodronportal(int, coord*, coord*, uint64_t);
E void mkstairs(xchar, xchar, char, struct mkroom *, int);
E void mkinvokearea(void);
E void mineralize(int, int, int, int, boolean);
E void maybe_create_location_light_source(xchar, xchar);
E void maybe_create_location_sound_source(xchar, xchar);
E void maybe_create_location_light_and_sound_sources(xchar, xchar);
E void create_level_light_sources(void);
E void create_level_sound_sources(void);
E void define_level_location_floor_types(boolean, boolean, boolean, boolean);

/* ### mkmap.c ### */

E void flood_fill_rm(int, int, int, boolean, boolean);
E void remove_rooms(int, int, int, int);
E void wallify_entire_map(void);
/* E void mkmap(lev_init *); -- need sp_lev.h for lev_init */

/* ### mkmaze.c ### */

E void maze_init(void);
E void wallification(int, int, int, int);
E void fix_wall_spines(int, int, int, int);
E void walkfrom(int, int, schar);
E void makemaz(const char *);
E void mazexy(coord *);
E void bound_digging(void);
E void mkportal(xchar, xchar, xchar, xchar, uchar, uint64_t, boolean);
E boolean bad_location(xchar, xchar, xchar, xchar,
                               xchar, xchar);
E void place_lregion(xchar, xchar, xchar, xchar, xchar,
                             xchar, xchar, xchar, xchar, d_level *);
E void fixup_special(void);
E void fumaroles(void);
E void movebubbles(void);
E void water_friction(void);
E void save_waterlevel(int, int);
E void reset_waterlevel(void);
E void restore_waterlevel(int);
E const char *waterbody_name(xchar, xchar);

/* ### mkobj.c ### */

E struct oextra *newoextra(void);
E void copy_oextra(struct obj *, struct obj *);
E void dealloc_oextra(struct obj *);
E void newomonst(struct obj *);
E void free_omonst(struct obj *);
E void newomid(struct obj *);
E void free_omid(struct obj *);
E void newolong(struct obj *);
E void free_olong(struct obj *);
E void new_omailcmd(struct obj *, const char *);
E void free_omailcmd(struct obj *);
E struct obj *mkobj_at(char, int, int, boolean);
E struct obj* mkobj_at_with_flags(char, int, int, boolean, boolean, uchar, int64_t, int64_t, uint64_t);
E struct obj *mksobj_at(int, int, int, boolean, boolean);
E struct obj* mksobj_found_at(int, int, int, boolean, boolean);
E struct obj* mksobj_at_with_flags(int, int, int, boolean, boolean, int, struct monst*, uchar, int64_t, int64_t, uint64_t);
E struct obj *mksobj_migr_to_species(int, unsigned, boolean, boolean);
E struct obj *mkobj(char, boolean, int);
E struct obj* mkobj_with_flags(char, boolean, boolean, int, struct monst*, uchar, int64_t, int64_t, uint64_t);
E int random_objectid_from_class(char, struct monst*, uint64_t);
E int random_spellbook_objectid(struct monst*, uint64_t);
E int rndmonnum(void);
E boolean bogon_is_pname(char);
E struct obj *splitobj(struct obj *, int64_t);
E struct obj *unsplitobj(struct obj *);
E void clear_splitobjs(void);
E void replace_object(struct obj *, struct obj *);
E struct obj *unknwn_contnr_contents(struct obj *);
E void bill_dummy_object(struct obj *);
E struct obj* memory_dummy_object(struct obj*);
E void add_memory_object_contents(struct obj*, struct obj*);
E void add_to_memoryobjs(struct obj*);
E void clear_memoryobjs(void);
E void clear_hero_object_memory_at(int, int);
E void costly_alteration(struct obj *, int);
E struct obj *mksobj(int, boolean, boolean, int);
E struct obj* mksobj_ex(int, boolean, boolean, int, struct monst*, uchar);
E struct obj* mksobj_with_flags(int, boolean, boolean, int, struct monst*, uchar, int64_t, int64_t, uint64_t);
E int bcsign(struct obj *);
E int weight(struct obj *);
E int64_t get_random_gold_amount(void);
E void set_random_gold_amount(struct obj*);
E struct obj *mkgold(int64_t, int, int);
E struct obj *mkcorpstat(int, struct monst *, struct permonst *, int,
                                 int, unsigned);
E int corpse_revive_type(struct obj *);
E struct obj *obj_attach_mid(struct obj *, unsigned);
E struct monst *get_mtraits(struct obj *, boolean);
E struct obj *mk_tt_object(int, int, int);
E struct obj *mk_named_object(int, struct permonst *, int, int, const char *);
E struct obj *rnd_treefruit_at(int, int);
E int64_t get_multigen_quan(uchar);
E short get_init_charge(uchar);
E short get_obj_init_charge(struct obj*);
E short get_max_charge(uchar);
E const char* get_recharge_text(enum recharging_types);
E short get_obj_max_charge(struct obj*);
E int get_init_enchantment(int);
E int get_obj_init_enchantment(struct obj*);
E int get_max_enchantment(int);
E int get_obj_max_enchantment(struct obj*);
E void set_corpsenm(struct obj *, int);
E void start_corpse_timeout(struct obj *);
E void bless(struct obj *);
E void unbless(struct obj *);
E void curse(struct obj *);
E void uncurse(struct obj *);
E void blessorcurse(struct obj *, int);
E boolean is_flammable(struct obj *);
E boolean is_rottable(struct obj *);
E boolean is_polymorphable(struct obj*);
E void place_object(struct obj *, int, int);
E void remove_object(struct obj *);
E void place_memory_object(struct obj*, int, int);
E void remove_memory_object(struct obj*);
E void update_last_memoryobj(void);
E void discard_minvent(struct monst *);
E void obj_extract_self(struct obj *);
E void extract_nobj(struct obj *, struct obj **);
E void extract_nexthere(struct obj *, struct obj **);
E boolean add_to_minv(struct monst *, struct obj *);
E struct obj *add_to_container(struct obj *, struct obj *);
E struct obj* add_to_magic_chest(struct obj*);
E void add_to_migration(struct obj *);
E void add_to_buried(struct obj *);
E void dealloc_obj(struct obj *);
E void obj_ice_effects(int, int, boolean);
E int64_t peek_at_iced_corpse_age(struct obj *);
E int hornoplenty(struct obj *, boolean);
E void obj_sanity_check(void);
E struct obj *obj_nexto(struct obj *);
E struct obj *obj_nexto_xy(struct obj *, int, int, boolean);
E struct obj *obj_absorb(struct obj **, struct obj **);
E struct obj *obj_meld(struct obj **, struct obj **);
E void pudding_merge_message(struct obj *, struct obj *);
E unsigned int get_item_base_weight(struct obj*);

/* ### mkroom.c ### */

E int make_room(int);
E void fill_zoo(struct mkroom *);
E struct permonst *antholemon(void);
E boolean nexttodoor(int, int);
E boolean nexttotree(int, int);
E boolean has_dnstairs(struct mkroom *);
E boolean has_upstairs(struct mkroom *);
E int somex(struct mkroom *);
E int somey(struct mkroom *);
E boolean inside_room(struct mkroom *, xchar, xchar);
E struct mkroom* which_room(xchar, xchar);
E struct mkroom* which_room_by_list(struct mkroom**, int, xchar, xchar);
E boolean somexy(struct mkroom *, coord *);
E boolean somexy_within_distance(struct mkroom*, coord, int, coord*);
E void mkundead(coord *, boolean, int);
E struct permonst *courtmon(void);
E void save_rooms(int);
E void rest_rooms(int);
E void reset_rooms(void);
E struct mkroom *search_special(schar);
E int cmap_to_type(int);
E int mknpcroom(int);
E void set_room_tileset(struct mkroom*);

/* ### mon.c ### */

E void mon_sanity_check(void);
E int undead_to_corpse(int);
E int genus(int, int);
E int pm_to_cham(int);
E int minliquid(struct monst *);
E int movemon(void);
E int meatmetal(struct monst *);
E int meatrock(struct monst*);
E int meatobj(struct monst *);
E void mpickgold(struct monst *);
E boolean mpickstuff(struct monst *, const char *);
E int curr_mon_load(struct monst *);
E int max_mon_load(struct monst *);
E int can_carry(struct monst *, struct obj *);
E int can_carry_core(struct monst*, struct obj*, boolean);
E int mfndpos(struct monst *, coord *, int64_t *, int64_t);
E int mfndpos_xy(struct monst*, xchar, xchar, coord*, int64_t*, int64_t);
E boolean monnear(struct monst *, int, int);
E boolean mon_somewhat_near(struct monst*, int, int);
E void dmonsfree(void);
E int mcalcmove(struct monst *, boolean);
E void update_monster_timeouts(void);
E void replmon(struct monst *, struct monst *);
E void relmon(struct monst *, struct monst **);
E struct obj *mlifesaver(struct monst *);
E boolean corpse_chance(struct monst *, struct monst *, boolean);
E void mondead(struct monst *);
E void mondead_with_flags(struct monst*, uint64_t);
E void mondied(struct monst *);
E void mongone(struct monst *);
E void monstone(struct monst *);
E void monkilled(struct monst *, const char *, int, int);
E void unstuck(struct monst *);
E void killed(struct monst *);
E void killed_by_stoning(struct monst*);
E void killed_by_disintegration(struct monst*);
E void xkilled(struct monst *, int);
E void mon_to_stone(struct monst *);
E void m_into_limbo(struct monst *, boolean);
E void mnexto(struct monst *);
E void mnexto2(struct monst*, boolean);
E void maybe_mnexto(struct monst *);
E int mnearto(struct monst *, xchar, xchar, boolean, boolean);
E void m_respond(struct monst *);
E void setmangry(struct monst *, boolean);
E void wakeup(struct monst *, boolean);
E void wake_nearby(void);
E void wake_nearto(int, int, int);
E void seemimic(struct monst *);
E void rescham(void);
E void restartcham(void);
E void restore_cham(struct monst *);
E boolean hideunder(struct monst *);
E void hide_monst(struct monst *);
E void mon_animal_list(boolean);
E boolean validvamp(struct monst *, int *, int);
E int select_newcham_form(struct monst *);
E void mgender_from_permonst(struct monst *, struct permonst *);
E int newcham(struct monst *, struct permonst *, unsigned short, boolean, boolean);
E int can_be_hatched(int);
E int egg_type_from_parent(int, boolean);
E boolean dead_species(int, boolean);
E void kill_genocided_monsters(void);
E void golemeffects(struct monst *, int, double);
E boolean angry_guards(boolean);
E void pacify_guards(void);
E void decide_to_shapeshift(struct monst *, int);
E boolean vamp_stone(struct monst *);
E void m_detach(struct monst*, struct permonst*, boolean);
E int randomtruegem(void);
E int random_reagent_otyp(boolean, boolean, int);
E int randomore(boolean);
E const char* pm_monster_name(struct permonst*, uchar);
E const char* mon_monster_name(struct monst*);
E const char* corpse_monster_name(struct obj*);
E const char* pm_female_name(struct permonst*);
E const char* pm_common_name(struct permonst*);
E const char* mon_common_name(struct monst*);
E const char* corpse_common_name(struct obj*);
E const char* pm_plural_name(struct permonst*, uchar);
E const char* pm_general_name(struct permonst*, uchar);
E boolean is_female_corpse_or_statue(struct obj*);
E boolean is_corpse_or_statue_facing_right(struct obj*);
E int count_mon_runeswords(struct monst*);
E int revert_mon_polymorph(struct monst*, boolean, boolean, boolean);
E void reset_mon(void);

/* ### mondata.c ### */

E void set_mon_data(struct monst *, struct permonst *, unsigned short);
E struct attack *attacktype_fordmg(struct permonst *, int, int);
E int does_passive_impact_obj(struct permonst*, struct obj*);
E boolean attacktype(struct permonst *, int);
E boolean noattacks(struct permonst *);
E boolean poly_when_stoned(struct permonst *);
E boolean can_blnd(struct monst *, struct monst *, uchar, struct obj *);
E boolean ranged_attk(struct permonst *);
E boolean hates_silver(struct permonst *);
E boolean mon_hates_silver(struct monst *);
E boolean hates_blessed(struct permonst*);
E boolean mon_hates_blessed(struct monst*);
E boolean hates_cursed(struct permonst*);
E boolean mon_hates_cursed(struct monst*);
E boolean eschews_cursed(struct permonst*);
E boolean mon_eschews_cursed(struct monst*);
E boolean eschews_blessed(struct permonst*);
E boolean mon_eschews_blessed(struct monst*);
E boolean mon_hates_light(struct monst*);
E boolean mon_eschews_light(struct monst*);
E boolean eschews_silver(struct permonst*);
E boolean mon_eschews_silver(struct monst*);
E boolean passes_bars(struct permonst *);
E boolean can_blow(struct monst *);
E boolean can_chant(struct monst *);
E boolean can_be_strangled(struct monst *);
E boolean can_track(struct permonst *);
E boolean breakarm(struct permonst *);
E boolean sliparm(struct permonst *);
E boolean sticks(struct permonst *);
E boolean cantvomit(struct permonst *);
E int num_horns(struct permonst *);
/* E boolean canseemon(struct monst *); */
E struct attack *dmgtype_fromattack(struct permonst *, int, int);
E boolean dmgtype(struct permonst *, int);
E int max_passive_dmg(struct monst *, struct monst *);
E boolean same_race(struct permonst *, struct permonst *);
E int monsndx(struct permonst *);
E int name_to_mon(const char *);
E int name_to_mon_ex(const char*, int*);
E int name_to_monclass(const char *, int *);
E int gender(struct monst *);
E int pronoun_gender(struct monst *, boolean);
E boolean levl_follower(struct monst *);
E int little_to_big(int);
E int big_to_little(int);
E boolean big_little_match(int, int);
E const char *locomotion(const struct permonst *, const char *);
E const char *stagger(const struct permonst *, const char *);
E const char *on_fire(struct permonst *, struct attack *);
E const struct permonst *raceptr(struct monst *);
E boolean olfaction(struct permonst *);
E uint64_t prop_to_innate(int);
E uint64_t prop_to_innate2(int);
E uint64_t prop_to_conveyed(int);
E int innate_to_prop(uint64_t);
E int innate2_to_prop(uint64_t);
E int conveyed_to_prop(uint64_t);
E const char* get_mflag_description(uint64_t, boolean, uchar);
E int get_worm_tail_mnum(struct permonst*);
boolean befriend_with_obj(struct permonst*, struct obj*);


/* ### monmove.c ### */

E boolean check_mon_wants_to_attack_target(struct monst*, struct monst*);
E boolean itsstuck(struct monst *);
E boolean mb_trapped(struct monst *);
E boolean monhaskey(struct monst *, boolean);
E void monster_regeneration_and_timeout(struct monst *, boolean);
E int dochugw(struct monst *);
E boolean onscary(int, int, struct monst *);
E boolean onnopickup(int, int, struct monst*);
E void monflee(struct monst *, int, boolean, boolean);
E void make_mon_fearful(struct monst*, int);
E void mon_yells(struct monst *, const char *, const char *, const char*, boolean, boolean);
E void check_mon_talk(struct monst*);
E int dochug(struct monst *);
E boolean m_digweapon_check(struct monst *, xchar, xchar);
E int m_move(struct monst *, int);
E void dissolve_bars(int, int);
E boolean closed_door(int, int);
E boolean accessible(int, int);
E void set_apparxy(struct monst *);
E boolean can_ooze(struct monst *);
E boolean can_fog(struct monst *);
E boolean should_displace(struct monst *, coord *, int64_t *, int, xchar, xchar);
E boolean undesirable_disp(struct monst *, xchar, xchar);
E boolean m_findtravelpath(struct monst*, int, xchar*, xchar*, int64_t);
E boolean m_test_move(struct monst*, xchar, xchar, xchar, xchar, int, int64_t);
E void check_boss_fight(struct monst*);
E void check_seen_bosses(void);

/* ### monst.c ### */

E void monst_init(void);

/* ### monstr.c ### */

E void monstr_init(void);

/* ### mplayer.c ### */

E struct monst *mk_mplayer(struct permonst *, xchar, xchar, boolean);
E void create_mplayers(int, boolean);
E void mplayer_talk(struct monst *);

#if defined(MICRO) || defined(WIN32)

/* ### msdos.c,os2.c,tos.c,winnt.c ### */

#ifndef WIN32
E int tgetch(void);
#endif
#ifndef TOS
E char switchar(void);
#endif
#ifndef __GO32__
E int64_t freediskspace(char *);
#ifdef MSDOS
E int findfirst_file(char *);
E int findnext_file(void);
E int64_t filesize_nh(char *);
#else
E int findfirst(const char *);
E int findnext(void);
E int64_t filesize(const char *);
#endif /* MSDOS */
E char *foundfile_buffer(void);
#endif /* __GO32__ */
E void chdrive(char *);
#ifndef TOS
E void disable_ctrlP(void);
E void enable_ctrlP(void);
#endif
#if defined(MICRO) && !defined(WINNT)
E void get_scr_size(void);
#ifndef TOS
E void gotoxy(int, int);
#endif
#endif
#ifdef TOS
E int _copyfile(char *, char *);
E int kbhit(void);
E void set_colors(void);
E void restore_colors(void);
#if defined(SUSPEND) && !defined(GNH_MOBILE)
E int dosuspend(void);
#endif
#endif /* TOS */
#ifdef WIN32
E char *get_username(size_t *);
E void nt_regularize(char *);
E int (*nt_kbhit)(void);
E void Delay(int);
#endif /* WIN32 */
#endif /* MICRO || WIN32 */

/* ### mthrowu.c ### */

E int thitu(int, int, struct obj **, const char *, struct monst*, const char*);
E int ohitmon(struct monst *, struct obj *, int, boolean);
E void thrwmu(struct monst *);
E int spitmu(struct monst *, struct attack *);
E int breamu(struct monst *, struct attack *);
E int eyesmu(struct monst*, struct attack*);
E boolean linedup(xchar, xchar, xchar, xchar, int, boolean, boolean, boolean, int, boolean, int);
E boolean lined_up(struct monst *, boolean, int, boolean, int);
E struct obj *m_carrying(struct monst *, int);
E struct obj* m_carrying_with_best_bounded_exceptionality(struct monst*, int, int, int);
E struct obj* m_carrying_with_best_exceptionality(struct monst*, int);
E int thrwmm(struct monst *, struct monst *);
E int spitmm(struct monst *, struct attack *, struct monst *);
E int breamm(struct monst *, struct attack *, struct monst *);
E int eyesmm(struct monst*, struct attack*, struct monst*);
E int buzzmm(struct monst*, struct attack*, struct monst*);
E void m_useupall(struct monst *, struct obj *);
E void m_useup(struct monst *, struct obj *);
E void m_throw(struct monst *, int, int, int, int, int, struct obj *);
E void hit_bars(struct obj **, int, int, int, int,
                        boolean, boolean);
E boolean hits_bars(struct obj **, int, int, int, int, int, int);
E const char* get_eyestalk_ray_name(int);
E const char* get_breath_weapon_name(int);
E void set_m_ray_spell_stats(struct monst*, struct attack*, struct monst*, int*, int*, int*, int*);
E void reset_mthrowu(void);

/* ### muse.c ### */

E boolean set_defensive_potion(struct monst*, struct obj*);
E boolean set_defensive_unicorn_horn(struct monst*, struct obj*);
E void clear_defensive(void);
E boolean find_defensive(struct monst *);
E int use_defensive(struct monst *);
E int rnd_defensive_item(struct monst *);
E boolean find_offensive(struct monst *);
#ifdef USE_TRAMPOLI
E int mbhitm(struct monst *, struct obj *, struct monst*);
#endif
E int use_offensive(struct monst *);
E int rnd_offensive_item(struct monst *);
E boolean set_misc_potion(struct monst *, struct obj *);
E boolean find_misc(struct monst*);
E int use_misc(struct monst *);
E int rnd_misc_item(struct monst *);
E boolean searches_for_item(struct monst *, struct obj *);
E boolean mon_reflects(struct monst *, const char *);
E boolean ureflects(const char *, const char *);
E void mcureblindness(struct monst *, boolean);
E void mcuresickness(struct monst*, boolean);
E void mcurehallucination(struct monst*, boolean);
E void mcurestun(struct monst*, boolean);
E void mcureconfusion(struct monst*, boolean);
E boolean munstone(struct monst *, boolean);
E boolean munslime(struct monst *, boolean);
E void mmake_wish(struct monst*);

/* ### music.c ### */

E void awaken_monsters(int, boolean);
E void awaken_soldiers(struct monst *, boolean);
E void do_earthquake(int);
E int do_play_instrument(struct obj *);
E const char* generic_lvl_desc(void);

/* ### nhlan.c ### */
#ifdef LAN_FEATURES
E void init_lan_features(void);
E char *lan_username(void);
#endif

/* ### nhregex.c ### */
E struct nhregex *regex_init(void);
E boolean regex_compile(const char *, struct nhregex *);
E const char *regex_error_desc(struct nhregex *);
E boolean regex_match(const char *, struct nhregex *);
E void regex_free(struct nhregex *);

/* ### npc.c ### */
E void forget_npc_entry(struct monst*);
E int npc_move(struct monst*);
E char npc_room_occupied(char*);
E struct monst* findnpc(char);
E boolean in_his_npc_room(struct monst*);
E void in_npc_room(int, boolean);
E void npcini(d_level*, struct mkroom*, int, int, uchar, int);
E void newenpc(struct monst*);
E void free_enpc(struct monst*);
E void clearnpcs(void);
E void restnpc(struct monst*, boolean);
E schar min_npc_appearance_depth(void);

/* ### nttty.c ### */

#ifdef WIN32
E void get_scr_size(void);
E int nttty_kbhit(void);
E void nttty_open(int);
E void nttty_rubout(void);
E int tgetch(void);
E int ntposkey(int *, int *, int *);
E void set_output_mode(int);
E void synch_cursor(void);
E void GnollHack_enter_nttty(void);
E void nttty_exit(void);
#endif

/* ### o_init.c ### */

E void save_initial_objects_values(void);
E void init_objects(void);
E void obj_shuffle_range(int, int *, int *);
E int find_skates(void);
E void oinit(void);
E void savenames(int, int);
E void reset_names(void);
E void restnames(int);
E void discover_object(int, boolean, boolean);
E void undiscover_object(int);
E int dodiscovered(void);
E int doclassdisco(void);
E void rename_disco(void);
E void exceptionality_checks(struct obj*);
E void randomize_mythic_quality(struct obj*, uchar, uchar*, uchar*);
E boolean is_mythic_affix_ok(uchar, uchar, struct obj*, uchar);
E double get_mythic_dmg_multiplier(struct obj*, struct monst*, struct monst*);
E int get_obj_exceptionality_ac_bonus(struct obj*);
E int get_obj_exceptionality_mc_bonus(struct obj*);
E int get_obj_material_and_exceptionality_ac_bonus(struct obj*);
E int get_obj_material_and_exceptionality_mc_bonus(struct obj*);
E int get_object_base_ac(struct obj*);
E int get_object_base_mc(struct obj*);
E uint64_t get_obj_oc_flags(struct obj*);
E uint64_t get_obj_oc_flags2(struct obj*);
E uint64_t get_obj_oc_flags3(struct obj*);
E uint64_t get_obj_oc_flags4(struct obj*);
E uint64_t get_obj_oc_flags5(struct obj*);
E uint64_t get_obj_oc_flags6(struct obj*);
E boolean can_wear_miscellaneous(struct permonst*, int);

/* ### objects.c ### */

E void objects_init(void);

/* ### matcomps.c ### */

E void matcomps_init(void);

/* ### objnam.c ### */

E char *obj_typename(int);
E char *simple_typename(int);
E boolean obj_is_pname(struct obj *);
E char *distant_name(struct obj *, char *(*)(OBJ_P));
E char *fruitname(boolean);
E struct fruit *fruit_from_indx(int);
E struct fruit *fruit_from_name(const char *, boolean, int *);
E void reorder_fruit(boolean);
E char *xname(struct obj *);
E char* xname_bare(struct obj*);
E char *mshot_xname(struct obj *);
E boolean the_unique_obj(struct obj *);
E boolean the_unique_pm(struct permonst *);
E boolean erosion_matters(struct obj *);
E char *doname(struct obj *);
E char* doname_with_flags(struct obj* obj, unsigned, char**, char**);
E char* doname_payobj(struct obj*);
E char* doname_in_text(struct obj*);
E char* doname_in_text_with_price_and_weight_last(struct obj*);
E char* doname_in_text_with_price_and_weight_last_and_comparison(struct obj*, char**, char**);
E char *doname_vague_quan(struct obj *);
E char* doname_with_weight_first_true(struct obj*);
E char* doname_with_weight_last_true(struct obj*);
E char* doname_with_weight_first(struct obj*, boolean, boolean, unsigned);
E char* doname_with_weight_last(struct obj*, boolean, boolean, unsigned);
E char* doname_with_price(struct obj*);
E char* doname_with_price_quick(struct obj*);
E char* doname_with_price_and_weight_first(struct obj*, boolean, boolean);
E char* doname_with_price_and_weight_last(struct obj*, boolean, boolean);
E char* doname_with_price_and_comparison(struct obj*, boolean, char**, char**);
E char* doname_with_price_and_weight_first_and_comparison(struct obj*, boolean, boolean, char**, char**);
E char* doname_with_price_and_weight_last_and_comparison(struct obj*, boolean, boolean, char**, char**);
E boolean not_fully_identified(struct obj *);
E boolean is_obj_unknown(struct obj*);
E char *corpse_xname(struct obj *, const char *, unsigned);
E char *cxname(struct obj *);
E char* acxname(struct obj*);
E char* aqcxname(struct obj*);
E char* thecxname(struct obj*);
E char* prepend_quan(int64_t, const char*);
E char *cxname_singular(struct obj *);
E char *killer_xname(struct obj *);
E char* killer_xname_flags(struct obj*, unsigned);
E char *short_oname(struct obj *, char *(*)(OBJ_P), char *(*)(OBJ_P), size_t);
E const char *singular(struct obj *, char *(*)(OBJ_P));
E char *an(const char *);
E char *An(const char *);
E char* an_prefix(const char*);
E char* An_prefix(const char*);
E char *The(const char *);
E char *the(const char *);
E char *aobjnam(struct obj *, const char *);
E char *yobjnam(struct obj *, const char *);
E char *Yobjnam2(struct obj *, const char *);
E char *Tobjnam(struct obj *, const char *);
E char *otense(struct obj *, const char *);
E char *vtense(const char *, const char *);
E char *Doname2(struct obj *);
E char* Doname_payobj2(struct obj*);
E char *yname(struct obj *);
E char *Yname2(struct obj *);
E char *ysimple_name(struct obj *);
E char *Ysimple_name2(struct obj *);
E char *simpleonames(struct obj *);
E char* simpleonames_payobj(struct obj*);
E char *ansimpleoname(struct obj *);
E char* ansimpleoname_payobj(struct obj*);
E char *thesimpleoname(struct obj *);
E char* thesimpleoname_payobj(struct obj*);
E char* actualoname(struct obj*);
E char *bare_artifactname(struct obj *);
E char *makeplural(const char *);
E char *makesingular(const char *);
E struct obj *readobjnam(char *, struct obj *, boolean, boolean*);
E int rnd_class(int, int);
E const char *suit_simple_name(struct obj *);
E const char *cloak_simple_name(struct obj *);
E const char* robe_simple_name(struct obj*);
E const char* miscitem_simple_name(struct obj*);
E const char *helm_simple_name(struct obj *);
E const char* armor_class_simple_name(struct obj*);
E const char* otyp_armor_class_simple_name(int);
E const char *mimic_obj_name(struct monst *);
E char *safe_qbuf(char *, const char *, const char *, struct obj *,
                          char *(*)(OBJ_P), char *(*)(OBJ_P), const char *);
E int shiny_obj(char);
E void add_erosion_words(struct obj*, char*);
E const char* get_key_special_quality_description(struct obj*);
E const char* get_key_special_quality_description_by_otyp(int, int);
E const char* get_lock_description_by_otyp(int, int, boolean);
E boolean find_key_otyp_by_description(char*, int*, int*);
E void print_lock_with_buf(char*, int, int, boolean);
E char* nextobuf(void);
E char* next_offset_init_obuf(void);
E char* str_upper_start(const char*);
E void print_comparison_stats(struct obj*, char*, winid, int, int, uchar, boolean, char*, char*, char*);
E void concatenate_colored_text(const char*, const char*, int, int, char*, char*, char*);
E const char* Japanese_item_name(int i);
E boolean is_obj_semi_transparent(struct obj*);

/* ### options.c ### */

E void reglyph_darkroom(void);
E boolean match_optname(const char *, const char *, size_t, boolean);
E void process_options_file(void);
E void init_options(void);
E void finish_options(void);
E boolean parseoptions(char *, boolean, boolean);
E int doset(void);
E int dotoggledecorations(void);
E int dotogglepickup(void);
E void option_help(void);
E void next_opt(winid, const char *);
E int fruitadd(char *, struct fruit *);
E int choose_classes_menu(const char *, int, boolean,
                                  char *, char *);
E boolean parsebindings(char *);
E void oc_to_str(char *, char *);
E void add_menu_cmd_alias(char, char);
E char get_menu_cmd_key(char);
E char map_menu_cmd(char);
E void show_menu_controls(winid, boolean);
E void assign_warnings(nhsym*);
E char *nh_getenv(const char *);
E void set_duplicate_opt_detection(int);
E void set_wc_option_mod_status(uint64_t, int);
E void set_wc2_option_mod_status(uint64_t, int);
E void set_option_mod_status(const char *, int);
E int add_autopickup_exception(const char *);
E void free_autopickup_exceptions(void);
E int load_symset(const char *, int);
E void free_symsets(void);
E boolean parsesymbols(char *);
E struct symparse *match_sym(char *);
E void set_playmode(void);
E nhsym sym_val(const char *);
E int query_color(const char *);
E int query_attr(const char *);
E const char *clr2colorname(int);
E int clrnameptr2color(const char*);
E int match_str2clr(char *);
E int match_str2attr(const char *, boolean);
E boolean add_menu_coloring(char *);
E boolean get_menu_coloring(const char *, int *, int *);
E int get_menu_coloring_index(const char*);
E boolean free_menu_coloring_str(const char*);
E void free_menu_coloring(void);
E boolean menu_style_allows_menu_coloring(int);
E boolean msgtype_parse_add(char *);
E int msgtype_type(const char *, boolean);
E void hide_unhide_msgtypes(boolean, int);
E void msgtype_free(void);

/* ### pager.c ### */

E char *self_lookat(char *);
E void mhidden_description(struct monst *, boolean, char *);
E boolean object_from_map(int,int,int,struct obj **);
E int do_screen_description(coord, boolean, nhsym, char *,
                                    const char **, struct permonst **);
E int do_look(int, coord *);
E int dowhatis(void);
E int doquickwhatis(void);
E int doidtrap(void);
E int dowhatdoes(void);
E char *dowhatdoes_core(char, char *);
E int dohelp(void);
E int dohistory(void);

/* ### pcmain.c ### */

#if defined(MICRO) || defined(WIN32)
#ifdef CHDIR
E void chdirx(char *, boolean);
#endif /* CHDIR */
E boolean authorize_wizard_mode(void);
#endif /* MICRO || WIN32 */

/* ### pcsys.c ### */

#if defined(MICRO) || defined(WIN32)
E void flushout(void);
E int dosh(void);
#ifdef MFLOPPY
E void eraseall(const char *, const char *);
E void copybones(int);
E void playwoRAMdisk(void);
E int saveDiskPrompt(int);
E void gameDiskPrompt(void);
#endif
E void append_slash(char *);
E void getreturn(const char *);
#ifndef AMIGA
E void msmsg(const char *, ...);
#endif
E FILE *fopenp(const char *, const char *);
#elif defined(UNIX) && defined(NOCWD_ASSUMPTIONS)
E void append_slash(char*);
#endif /* MICRO || WIN32 */

/* ### pctty.c ### */

#if defined(MICRO) || defined(WIN32)
E void gettty(void);
E void settty(const char *);
E void setftty(void);
E void error(const char *, ...);
#if defined(TIMED_DELAY) && defined(_MSC_VER)
E void msleep(unsigned);
#endif
#endif /* MICRO || WIN32 */

/* ### pcunix.c ### */

#if defined(MICRO)
E void regularize(char *);
#endif /* MICRO */
#if defined(PC_LOCKING)
E void getlock(void);
#endif

/* ### pickup.c ### */

E int collect_obj_classes(char *, struct obj *, boolean,
                                  boolean FDECL((*), (OBJ_P)), int *);
E boolean rider_corpse_revival(struct obj *, boolean, boolean*);
E boolean menu_class_present(int);
E void add_valid_menu_class(int);
E boolean allow_all(struct obj *);
E boolean allow_all_but_coins(struct obj*);
E boolean allow_category(struct obj *);
E boolean allow_unicorn_horn(struct obj*);
E boolean is_potion_of_water(struct obj*);
E boolean is_scroll_of_remove_curse(struct obj*);
E boolean is_worn_by_type(struct obj *);
E boolean is_wearable_by_type(struct obj*);
E int ck_bag(struct obj *);
E int stash_obj_in_container(struct obj*, struct obj*);
#ifdef USE_TRAMPOLI
E int in_container(struct obj *);
E int out_container(struct obj *);
#endif
E int auto_bag_in(struct obj*, struct obj*, boolean);
E int count_bags_for_stashing(struct obj*, struct obj*, boolean, boolean);
E int pickup(int, boolean);
E int pickup_object(struct obj *, int64_t, boolean, boolean, uchar*);
E int query_category(const char *, struct obj *, int,
                             menu_item **, int, int);
E int query_objlist(const char *, struct obj **, int,
                            menu_item **, int, boolean (*)(OBJ_P), int);
E void add_weight_summary(winid, int, int);
E void add_weight_summary_putstr(winid, int, int);
E int enclevelmaximumweight(enum encumbrance_types);
E int enclevelminimumweight(enum encumbrance_types);
E struct obj *pick_obj(struct obj *);
E int encumber_msg(void);
E int container_at(int, int, boolean);
E int doloot(void);
E int dolootin(void);
E int dolootout(void);
E void observe_quantum_cat(struct obj *, boolean, boolean);
E boolean container_gone(int (*)(OBJ_P));
E boolean u_handsy(void);
E int use_container(struct obj **, int, boolean, int);
E int loot_mon(struct monst *, int *, boolean *, boolean);
E int dotip(void);
E boolean is_autopickup_exception(struct obj *, boolean);
E boolean autopick_testobj(struct obj *, boolean);
E boolean maybe_disturb_container_monster(struct obj* );
E int handle_knapsack_prefull(int, int, boolean*, struct obj**, struct obj*, int, int, uchar);
E int handle_knapsack_full(void);
E int dostash(void);
E int dostashfloor(void);
E boolean can_stash_objs(void);
E boolean can_floor_stash_objs(void);
E void set_current_container_to_dummyobj(void);
E void set_current_container_to_null(void);
E void reset_pickup(void);
E boolean loadstone_weight_shown_correctly(int);

/* ### pline.c ### */

#if defined(DUMPLOG) || defined(DUMPHTML)
E void dumplogmsg(const char *, const char*, const char*, int, int);
E void dumplogfreemessages(void);
#endif
E void pline(const char *, ...) PRINTF_F(1, 2);
E void custompline(unsigned, const char *, ...) PRINTF_F(2, 3);
E void custompline_ex(int, int, unsigned, const char*, ...) PRINTF_F(4, 5);
E void custompline_ex_prefix(int, int, const char*, int, int, const char*, int, int, unsigned, const char*, ...) PRINTF_F(10, 11);
E void pline_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void pline_multi_ex(int, int, const int*, const int*, const char*, ...) PRINTF_F(5, 6);
E void pline_multi_ex_popup(int, int, const int*, const int*, const char*, int, const char*, ...) PRINTF_F(7, 8);
E void pline_multi_ex_flags(int, int, const int*, const int*, unsigned, const char*, ...) PRINTF_F(6, 7);
E void Norep(const char *, ...) PRINTF_F(1, 2);
E void Norep_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void free_youbuf(void);
E void You(const char *, ...) PRINTF_F(1, 2);
E void Your(const char *, ...) PRINTF_F(1, 2);
E void You_feel(const char *, ...) PRINTF_F(1, 2);
E void You_cant(const char *, ...) PRINTF_F(1, 2);
E void You_hear(const char *, ...) PRINTF_F(1, 2);
E void You_see(const char *, ...) PRINTF_F(1, 2);
E void pline_The(const char *, ...) PRINTF_F(1, 2);
E void There(const char *, ...) PRINTF_F(1, 2);
E void You_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void You_multi_ex(int, int, const int*, const int*, const char*, ...) PRINTF_F(5, 6);
E void Your_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void Your_multi_ex(int, int, const int*, const int*, const char*, ...) PRINTF_F(5, 6);
E void You_feel_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void You_cant_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void You_hear_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void You_see_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void pline_The_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void pline_The_multi_ex(int, int, const int*, const int*, const char*, ...) PRINTF_F(5, 6);
E void There_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void There_multi_ex(int, int, const int*, const int*, const char*, ...) PRINTF_F(5, 6);
E void verbalize_ex(int, int, const char*, ...) PRINTF_F(3, 4);
E void verbalize(const char *, ...) PRINTF_F(1, 2);
E void raw_printf(const char *, ...) PRINTF_F(1, 2);
E void impossible(const char *, ...) PRINTF_F(1, 2);
E void silent_impossible(const char*, ...) PRINTF_F(1, 2);
E void debugprint(const char*, ...) PRINTF_F(1, 2);
E void config_error_add(const char *, ...) PRINTF_F(1, 2);
E void reset_pline(void);
E int* get_colorless_multicolor_buffer(void);
E void pline1_multi_ex(const char*, const char*, const char*, int, int);
E char* allocate_buffer_with_debug_buffers(const char*);
E const char* basefilename(const char*);

/* ### polyself.c ### */

E void set_uasmon(void);
E void float_vs_flight(void);
E void change_sex(void);
E void polyself(int);
E int polymon(int);
E int monster_attribute_maximum(struct permonst *, int);
E int monster_attribute_minimum(struct permonst*, int);
E void rehumanize(void);
E int dobreathe(void);
E int dosteedbreathe(void);
E int dosteedbreathemon(struct monst*);
E int dospit(void);
E int doremove(void);
E int dospinweb(void);
E int dosummon(void);
E int dogaze(void);
E int doeyestalk(void);
E int dohide(void);
E int dopoly(void);
E int domindblast(void);
E int dodryfountain(void);
E int douseunicornhorn(void);
E int doshriek(void);
E int dolayegg(void);
E void uunstick(void);
E void skinback(boolean);
E const char *mbodypart(struct monst *, int);
E const char *body_part(int);
E int poly_gender(void);
E void ugolemeffects(int, double);
E boolean ugenocided(void);
E const char *udeadinside(void);
E int get_ray_adtyp(uchar);
E int get_ray_adtyp_choose(uchar, const char*, struct monst*);
E void reset_polyself(void);

/* ### potion.c ### */

E void set_itimeout(int64_t *, int64_t);
E void incr_itimeout(int64_t *, int);
E void incr_itimeout_limited(int64_t*, int, int);
E int64_t itimeout_incr(int64_t, int);
E int64_t itimeout_incr_limited(int64_t, int, int);
E void make_confused(int64_t, boolean);
E void make_stunned(int64_t, boolean);
E void make_sick(int64_t, const char *, boolean, int);
E void make_food_poisoned(int64_t, const char*, boolean, int);
E void make_mummy_rotted(int64_t, const char*, boolean, int);
E void make_slimed(int64_t, const char*, int, const char*, int);
E void make_stoned(int64_t, const char *, int, const char *, int);
E void make_vomiting(int64_t, boolean);
E void make_blinded(int64_t, boolean);
E void toggle_blindness(void);
E boolean make_hallucinated(int64_t, boolean, int64_t);
E void make_deaf(int64_t, boolean);
E void self_invis_message(void);
E int dodrink(void);
E int dopotion(struct obj *);
E int peffects(struct obj *);
E void healup(int, int, boolean, boolean, boolean, boolean, boolean);
E void strange_feeling(struct obj *, const char *, boolean);
E void potionhit(struct monst *, struct obj **, int);
E void potionbreathe(struct obj *, const char*);
E int dodip(void);
E int refill_obj_with_oil(struct obj*, struct obj*);
E void mongrantswish(struct monst **);
E void djinni_from_bottle(struct obj *);
E void convert_magic_lamp_to_oil_lamp(struct obj*);
E struct monst *split_mon(struct monst *, struct monst *);
E const char *bottlename(void);
E void reset_potion(void);
E boolean H2Opotion_dip(struct obj*, struct obj*, boolean, const char*);

/* ### pray.c ### */

E boolean critically_low_hp(boolean);
E boolean stuck_in_wall(void);
#ifdef USE_TRAMPOLI
E int prayer_done(void);
#endif
E int dosacrifice(void);
E boolean can_pray(boolean);
E int dopray(void);
E const char *u_gname(void);
E int doturn(void);
E const char *a_gname(void);
E const char *a_gname_at(xchar x, xchar y);
E const char *align_gname(aligntyp);
E const char* align_gdesc(aligntyp);
E const char* align_ghisher(aligntyp);
E const char* u_ghisher(void);
E const char *halu_gname(aligntyp);
E const char *align_gtitle(aligntyp);
E boolean align_gfemale(aligntyp);
E void altar_wrath(int, int);
E int absolution_spell(void);
E int wiz_crown(void);
E int get_artifact_replacement_item_otyp(void);
E void removealtarsummons(void);

/* ### priest.c ### */

E int move_special(struct monst *, boolean, schar, boolean,
                           boolean, xchar, xchar, xchar, xchar);
E char temple_occupied(char *);
E boolean inhistemple(struct monst *);
E int pri_move(struct monst *);
E void priestini(d_level *, struct mkroom *, int, int, boolean, int);
E aligntyp mon_aligntyp(struct monst *);
E char *priestname(struct monst *, char *);
E boolean p_coaligned(struct monst *);
E struct monst *findpriest(char);
E void intemple(int);
E void forget_temple_entry(struct monst *);
E void priest_talk(struct monst *);
E struct monst *mk_roamer(struct permonst *, aligntyp, xchar,
                                  xchar, boolean);
E void reset_hostility(struct monst *);
E boolean in_your_sanctuary(struct monst *, xchar, xchar);
E void ghod_hitsu(struct monst *);
E void angry_priest(void);
E void clearpriests(void);
E void restpriest(struct monst *, boolean);
E void newepri(struct monst *);
E void free_epri(struct monst *);
E const char *align_str(aligntyp);
E char *piousness(boolean, const char *);
E void mstatusline(struct monst *);
E void print_mstatusline(char*, struct monst*, int, boolean);
E void ustatusline(void);
E char smithy_occupied(char*);
E struct monst* findsmith(char);
E boolean inhissmithy(struct monst*);
E void insmithy(int);
E void smithini(d_level*, struct mkroom*, int, int, uchar, int);
E void forget_smithy_entry(struct monst*);
E void newesmi(struct monst*);
E void free_esmi(struct monst*);
E void clearsmiths(void);
E void restsmith(struct monst*, boolean);
E int smith_move(struct monst*);

/* ### prop.c ### */

E void props_init(void);
E const char* get_property_name(int);
E struct propname get_property_name_ex(int);
E const char* get_status_name(struct monst*, int);

/* ### quest.c ### */

E void onquest(void);
E void nemdead(void);
E void artitouch(struct obj *);
E boolean ok_to_quest(void);
E boolean leader_speaks(struct monst *);
E boolean nemesis_speaks(struct monst*);
E boolean quest_chat(struct monst *, boolean);
E boolean quest_talk(struct monst *, boolean);
E void quest_stat_check(struct monst *);
E void finish_quest(struct obj *);
E void achievement_gained(const char*);
E boolean not_capable(void);
E int is_pure(boolean);

/* ### questpgr.c ### */

E void load_qtlist(void);
E void unload_qtlist(void);
E short quest_info(int);
E const char *ldrname(void);
E boolean is_quest_artifact(struct obj *);
E struct obj *find_quest_artifact(unsigned);
E void com_pager(struct monst*, int);
E void com_pager_ex(struct monst*, int, int, int, boolean);
E void qt_pager(struct monst*, int);
E void qt_pager_ex(struct monst*, int, int, int, boolean);
E struct permonst *qt_montype(void);
E void deliver_splev_message(void);
E void write_quest_texts(int);
E const char* neminame(void);

/* ### random.c ### */

#if defined(RANDOM) && !defined(__GO32__) /* djgpp has its own random */
E void srandom(unsigned);
E char *initstate(unsigned, char *, int);
E char *setstate(char *);
E int64_t random(void);
#endif /* RANDOM */

/* ### read.c ### */

E void learnscroll(struct obj *);
E char *tshirt_text(struct obj *, char *);
E int doread(void);
E boolean is_chargeable(struct obj *);
E void recharge(struct obj *, int, boolean, const char*, boolean);
E void forget_objects(int);
E void forget_levels(int);
E void forget_traps(void);
E void forget_map(int);
E int seffects(struct obj *, boolean*, struct monst*);
E void drop_boulder_on_player(boolean, boolean, boolean, boolean);
E boolean drop_boulder_on_monster(int, int, boolean, boolean);
E void wand_explode(struct obj *, int);
#ifdef USE_TRAMPOLI
E void set_lit(int, int, genericptr_t);
#endif
E void litroom(boolean, struct obj *);
E void do_genocide(int);
E void punish(struct obj *);
E void unpunish(void);
E boolean cant_revive(int *, boolean, struct obj *);
E boolean create_particular(void);
E int maybe_tame(struct monst*, struct obj*, struct monst*, const char*);
E int maybe_controlled(struct monst*, struct obj*, struct monst*, const char*);
E void read_the_ruling_ring(struct obj*);
E void strip_charges(struct obj*, boolean, boolean);
E void p_glow1(struct obj*, int, int, const char*, boolean);
E void p_glow2(struct obj*, const char*, int, int, const char*, boolean);
E void dragon_scales_to_scale_mail(struct obj*, boolean, boolean);
E boolean get_valid_targeted_position(int, int, int);
E int get_invalid_stinking_cloud_pos(int, int);
E boolean is_valid_stinking_cloud_pos(int, int, boolean);
E void display_stinking_cloud_positions(int);
E int bless_or_curse(struct obj*, struct monst*, boolean);
E int remove_curse(struct obj*, struct monst*, boolean);
E void enchant_ring(struct obj*, int, int, boolean);
E void enchant_armor(struct obj*, int, int, boolean);
E void reset_read(void);

/* ### rect.c ### */

E void init_rect(void);
E NhRect *get_rect(NhRect *);
E NhRect *rnd_rect(void);
E void remove_rect(NhRect *);
E void add_rect(NhRect *);
E void split_rects(NhRect *, NhRect *);

/* ## region.c ### */
E void clear_regions(void);
E void run_regions(void);
E boolean in_out_region(xchar, xchar);
E boolean m_in_out_region(struct monst *, xchar, xchar);
E void update_player_regions(void);
E void update_monster_region(struct monst *);
E NhRegion *visible_region_at(xchar, xchar);
E void show_region(NhRegion *, xchar, xchar);
E void save_regions(int, int);
E void rest_regions(int, boolean);
E void remove_expired_regions(boolean);
E void region_stats(const char *, char *, int64_t *, size_t *);
E NhRegion *create_gas_cloud(xchar, xchar, enum region_types, int, int, int, int);
E NhRegion* create_simple_permanent_region(xchar, xchar, xchar, xchar, enum region_types);
E boolean region_danger(void);
E void region_safety(void);
E struct nhregion* find_rid(int);
E int get_rid(struct nhregion*);
E void do_light_regions(char**);

/* ### restore.c ### */

E int dorestore(int, boolean);
E int dorestore0(int);
E void inven_inuse(boolean);
E void restcemetery(int, struct cemetery **);
E void trickery(char *);
E void getlev(int, int, xchar, boolean);
E boolean get_plname_from_file(int, char *, size_t);
E void get_save_game_stats_from_file(int, struct save_game_stats*);
#ifdef SELECTSAVED
E int restore_menu(winid);
E int select_saved_game(winid, uchar, struct save_game_data*);
#endif
E void minit(void);
E boolean lookup_id_mapping(unsigned, unsigned *);
E void mread(int, genericptr_t, size_t);
E int validate(int, const char *);
E void reset_restpref(void);
E void set_restpref(const char *);
E void set_savepref(const char *);
E void print_dgnlvl_buf(char*, char*, const char*, const char*, int, boolean*);
E void print_current_dgnlvl(char*);
E void print_character_description(char*, short, short, short, boolean, aligntyp, const char*);
E void print_location_description(char*, const char*, const char*, int, schar, const char*);
E void print_mode_duration_description(char*, schar, int64_t, boolean, boolean, boolean, boolean, boolean, boolean, const char*);
E void print_timestamp_description(char*, const char*, time_t, const char*);
E void reset_restore(void);

/* ### rip.c ### */

E void genl_outrip(winid, int, time_t);

/* ### rm.c ### */
E void init_rm(void);
E enum obj_material_types get_location_type_material(int, int);
E int get_initial_location_subtype(int);
E int get_initial_location_vartype(int, int);
E int get_initial_tree_subtype(schar);
E const char* get_door_name_at(xchar, xchar);
E const char* get_door_name_at_ptr(struct rm*);
E const char* get_short_door_name_at(xchar, xchar);
E const char* get_short_door_name_at_ptr(struct rm*);
E boolean m_can_destroy_door(struct monst*, struct rm*, boolean);
E boolean is_door_indestructible_at(xchar, xchar);
E boolean is_door_indestructible_at_ptr(struct rm*);
E boolean is_door_nonpassable_at(xchar, xchar);
E boolean is_door_nonpassable_at_ptr(struct rm*);
E boolean has_door_normal_lock_at(xchar, xchar);
E boolean has_door_normal_lock_at_ptr(struct rm*);
E boolean has_box_normal_lock(struct obj*);
E boolean is_door_diggable_at(xchar, xchar);
E boolean is_door_diggable_at_ptr(struct rm*);
E boolean is_door_kickable_at(xchar, xchar);
E boolean is_door_kickable_at_ptr(struct rm*);
E boolean is_door_destroyed_by_booby_trap_at(xchar, xchar);
E boolean is_door_destroyed_by_booby_trap_at_ptr(struct rm*);
E boolean is_door_destroyed_by_striking_at(xchar, xchar);
E boolean is_door_destroyed_by_striking_at_ptr(struct rm*);
E boolean is_door_destroyed_by_monsters_at(xchar, xchar);
E boolean is_door_destroyed_by_monsters_at_ptr(struct rm*);
E boolean door_blocks_vision_at(xchar, xchar);
E boolean door_blocks_vision_at_ptr(struct rm*);
E boolean is_door_normally_without_lock_at(xchar, xchar);
E boolean is_door_normally_without_lock_at_ptr(struct rm*);
E void set_initial_location_floortype(struct rm*, boolean, boolean, boolean, boolean);
E void initialize_location(struct rm*);
E void set_initial_location_type_at(xchar, xchar, int);
E void set_initial_location_type_ptr(struct rm*, int);

/* ### rnd.c ### */

#ifdef USE_ISAAC64
E void init_isaac64(uint64_t, int (*fn)(int));
E int64_t nhrand(void);
#endif
E int rn2(int);
E int rn2_on_display_rng(int);
E int rnl(int);
E int rnd(int);
E int d(int, int);
E int rne(int);
E int rnz(int);
E int rngh(int, int);
E int dd(int, int, int);

/* ### role.c ### */

E boolean validrole(int);
E boolean validrace(int, int);
E boolean validgend(int, int, int);
E boolean validalign(int, int, int);
E int randrole(boolean);
E int randrace(int);
E int randgend(int, int);
E int randalign(int, int);
E int str2role(const char *);
E int str2race(const char *);
E int str2gend(const char *);
E int str2align(const char *);
E boolean ok_role(int, int, int, int);
E int pick_role(int, int, int, int);
E boolean ok_race(int, int, int, int);
E int pick_race(int, int, int, int);
E boolean ok_gend(int, int, int, int);
E int pick_gend(int, int, int, int);
E boolean ok_align(int, int, int, int);
E int pick_align(int, int, int, int);
E void rigid_role_checks(void);
E boolean setrolefilter(const char *);
E boolean gotrolefilter(void);
E void clearrolefilter(void);
E char *build_plselection_prompt(char *, size_t, int, int, int, int, boolean);
E char *root_plselection_prompt(char *, size_t, int, int, int, int);
E void plnamesuffix(void);
E void role_selection_prolog(int, winid);
E void role_menu_extra(int, winid, boolean);
E void role_init(void);
E const char *Hello(struct monst *);
E const char *Goodbye(void);
E int u_to_glyph(void);
E int player_to_glyph_index(int, int, int, int, int);
E boolean player_has_action_tile(enum action_tile_types, int, int, int, int, int);
E uint64_t get_player_action_flags(enum action_tile_types, int, int, int, int, int);
E uint64_t u_action_flags(enum action_tile_types);
E uint64_t u_item_use_flags(void);
#if 0
E boolean player_has_attack_tile(int, int, int, int, int);
E boolean player_has_throw_tile(int, int, int, int, int);
E boolean player_has_fire_tile(int, int, int, int, int);
E boolean player_has_cast_tile(int, int, int, int, int);
E boolean player_has_special_attack_tile(int, int, int, int, int);
E boolean player_has_kick_tile(int, int, int, int, int);
E boolean player_has_item_use_tile(int, int, int, int, int);
E boolean player_has_door_use_tile(int, int, int, int, int);
E boolean player_has_death_tile(int, int, int, int, int);
#endif

E int glyph_to_player_mon(int);
E int attack_glyph_to_player_mon(int);
E int throw_glyph_to_player_mon(int);
E int fire_glyph_to_player_mon(int);
E int nodir_cast_glyph_to_player_mon(int);
E int dir_cast_glyph_to_player_mon(int);
E int special_attack_glyph_to_player_mon(int);
E int kick_glyph_to_player_mon(int);
E int item_use_glyph_to_player_mon(int);
E int door_use_glyph_to_player_mon(int);
E int death_glyph_to_player_mon(int);
E int passive_defense_glyph_to_player_mon(int);
E int special_attack2_glyph_to_player_mon(int);
E int special_attack3_glyph_to_player_mon(int);
E const char* get_advancement_description(int, int, int, boolean);
E void save_initial_urolerace_values(void);
E void reset_urolerace(void);

/* ### rumors.c ### */

E char *getrumor(int, char *, boolean);
E char *get_rnd_text(const char *, char *, int FDECL((*), (int)));
E void outrumor(struct monst*, struct obj*, int, int);
E void outoracle(struct monst*, struct obj*, boolean, int);
E void save_oracles(int, int);
E void reset_oracles(void);
E void restore_oracles(int);
E int doconsult(struct monst *);
E int do_oracle_identify(struct monst*);
E int do_oracle_enlightenment(struct monst*);
E void rumor_check(void);
E int get_number_of_oracle_major_consultations(void);

/* ### save.c ### */

E int dosave(void);
E int dosave0(boolean);
E boolean tricked_fileremoved(int, char *);
#ifdef INSURANCE
E void savestateinlock(void);
#endif
#ifdef MFLOPPY
E boolean savelev(int, xchar, int);
E boolean swapin_file(int);
E void co_false(void);
#else
E void savelev(int, xchar, int);
#endif
E genericptr_t mon_to_buffer(struct monst *, int *);
E void bufon(int);
E void bufoff(int);
E void bflush(int);
E void bwrite(int, genericptr_t, size_t);
E void bclose(int);
E void def_bclose(int);
#if defined(ZEROCOMP)
E void zerocomp_bclose(int);
#endif
E void savecemetery(int, int, struct cemetery **);
E void savefruitchn(int, int);
E void reset_fruitchn(void);
E void store_plname_in_file(int);
E void store_save_game_stats_in_file(int, int64_t);
E void free_dungeons(void);
E void free_dynamic_data_A(void);
E void free_dynamic_data_B(void);
E void free_dynamic_data_C(void);
E void freedynamicdata(void);
E void store_savefileinfo(int);
E void track_new_save_file(const char*, int64_t);
E void reset_save(void);

/* ### shk.c ### */

E void setpaid(struct monst *);
E int64_t money2mon(struct monst *, int64_t);
E void money2u(struct monst *, int64_t);
E void shkgone(struct monst *);
E void set_residency(struct monst *, boolean);
E void replshk(struct monst *, struct monst *);
E void restshk(struct monst *, boolean);
E char inside_shop(xchar, xchar);
E void u_left_shop(char *, boolean);
E void remote_burglary(xchar, xchar);
E void u_entered_shop(char *);
E void pick_pick(struct obj *);
E boolean same_price(struct obj *, struct obj *);
E void shopper_financial_report(void);
E int inhishop(struct monst *);
E struct monst *shop_keeper(char);
E boolean tended_shop(struct mkroom *);
E boolean is_unpaid(struct obj *);
E void delete_contents(struct obj *);
E void obfree(struct obj *, struct obj *);
E void home_shk(struct monst *, boolean);
E void make_happy_shk(struct monst *, boolean);
E void make_happy_shoppers(boolean);
E void hot_pursuit(struct monst *);
E void make_angry_shk(struct monst *, xchar, xchar);
E struct monst* can_pay_to_shkp(void);
E int dopay(void);
E boolean paybill(int, boolean);
E void finish_paybill(void);
E struct obj *find_oid(unsigned);
E int64_t contained_cost(struct obj *, struct monst *, int64_t, boolean, boolean);
E int64_t contained_gold(struct obj *);
E void picked_container(struct obj *);
E void gem_learned(int);
E void alter_cost(struct obj *, int64_t);
E int64_t unpaid_cost(struct obj *, boolean);
E boolean billable(struct monst **, struct obj *, char, boolean);
E void addtobill(struct obj *, boolean, boolean, boolean);
E void splitbill(struct obj *, struct obj *);
E void subfrombill(struct obj *, struct monst *);
E int64_t stolen_value(struct obj *, xchar, xchar, boolean, boolean);
E void sellobj_state(int);
E void sellobj(struct obj *, xchar, xchar);
E int doinvbill(int);
E struct monst *shkcatch(struct obj *, xchar, xchar);
E void add_damage(xchar, xchar, int64_t);
E int repair_damage(struct monst *, struct damage *, int *,
                            boolean);
E int shk_move(struct monst *);
E void after_shk_move(struct monst *);
E boolean is_fshk(struct monst *);
E void shopdig(int);
E void pay_for_damage(const char *, boolean);
E boolean costly_spot(xchar, xchar);
E struct obj *shop_object(xchar, xchar);
E void price_quote(struct obj *);
E void shk_chat(struct monst *);
E void check_unpaid_usage(struct obj *, boolean);
E void check_unpaid(struct obj *);
E void costly_gold(xchar, xchar, int64_t);
E int64_t get_cost_of_shop_item(struct obj *, int *);
E int oid_price_adjustment(struct obj *, unsigned);
E boolean block_door(xchar, xchar);
E boolean block_entry(xchar, xchar);
E char *shk_your(char *, struct obj *);
E char *Shk_Your(char *, struct obj *);
E int64_t get_cost(struct obj*, struct monst*);
E int shk_chastise_pet(struct monst* mtmp, struct obj*, boolean, boolean);
E int64_t set_cost(struct obj*, struct monst*);
E boolean is_obj_on_shk_bill(struct obj*, struct monst*);
E void add_one_tobill(struct obj*, boolean, struct monst*);
E void reset_shk(void);
E int64_t getprice(struct obj*, boolean);
E void mark_unpaid_container_contents(struct obj*);
E void unmark_unpaid_container_contents(struct obj*);

/* ### shknam.c ### */

E void neweshk(struct monst *);
E void free_eshk(struct monst *);
E void stock_room(int, struct mkroom *, boolean);
E int get_gehennom_undead_spellcaster(int);
E boolean saleable(struct monst *, struct obj *);
E int get_shop_item(int);
E char *Shknam(struct monst *);
E char *shkname(struct monst *);
E char* true_shkname(struct monst*);
E boolean shkname_is_pname(struct monst *);
E boolean is_izchak(struct monst *, boolean);
E const char* shoptypename(int);
E boolean veggy_item(struct obj* obj, int);
E void izchak_talk(struct monst*);
E int64_t get_object_base_value(struct obj*);

/* ### sit.c ### */

E void take_gold(void);
E int dosit(void);
E void rndcurse(void);
E void attrcurse(void);
E void m_attrcurse(struct monst*);

/* ### sounds.c ### */

E void dosounds(void);
E const char *growl_sound(struct monst *);
E void growl(struct monst *);
E void yelp(struct monst *);
E void whimper(struct monst *);
E void beg(struct monst *);
E int doyell(void);
E int dotalk(void);
E int dotalksteed(void);
E int dotalknearby(void);
#ifdef USER_SOUNDS
E int add_sound_mapping(const char *);
E void play_sound_for_message(const char *);
#endif
E int count_sellable_items(struct monst*);
E struct obj* get_first_sellable_item(struct monst*);
E boolean m_sellable_item(struct obj*, struct monst*);
E int64_t get_cost_of_monster_item(struct obj*, struct monst*);
E int64_t m_contained_cost(struct obj*, struct monst*);
E boolean is_shop_item_type(struct obj*, int);
E boolean is_npc_item_identification_type(struct obj*, int);
E int release_item_from_hero_inventory(struct obj*);
E int ask_shk_reconciliation(struct monst*);
E double service_cost_charisma_adjustment(int);
E void genl_chat_message(void);
E void popup_talk_lines(struct monst*, const char**);
E void popup_talk_line(struct monst*, const char*);
E void popup_talk_line_noquotes(struct monst*, const char*);
E void popup_talk_line_ex(struct monst*, const char*, int, int, boolean, boolean);
E void popup_talk_line_with_know_mname(struct monst*, const char*, boolean);
E int service_identify(struct monst*, int64_t);
E void You_ex1_popup(const char*, const char*, int, int, int, uint64_t);
E int dochatmon(struct monst*);
E boolean maybe_refillable_with_oil(struct obj*);


/* ### soundset.c ### */

E void update_game_music(void);
E void play_level_ambient_sounds(void);
E void play_environment_ambient_sounds(void);
E enum floor_surface_types get_floor_surface_type(int, int, boolean);
E void play_movement_sound(struct monst*, enum climbing_types);
E void play_object_floor_sound(struct obj*, enum object_sound_types, boolean);
E void play_object_floor_sound_at_location(struct obj*, enum object_sound_types, int, int, boolean);
E void play_object_container_in_sound(struct obj*, struct obj*);
E enum hit_surface_types get_hit_surface_type(enum hit_surface_source_types, ANY_P*);
E void get_hit_location(enum hit_surface_source_types, ANY_P*, xchar*, xchar*);
E void play_simple_object_sound(struct obj*, enum object_sound_types);
E void play_simple_object_sound_at_location(struct obj*, int, int, enum object_sound_types);
E void set_simple_object_sound_id_and_volume(enum object_soundset_types, enum object_sound_types, enum ghsound_types*, float*);
E void play_simple_container_sound(struct obj*, enum container_sound_types);
E void play_simple_container_sound_at_location(struct obj*, int, int, enum container_sound_types);
E void set_simple_container_sound_id_and_volume(enum object_soundset_types, enum container_sound_types, enum ghsound_types*, float*);
E void play_object_hit_sound(struct obj*, enum hit_surface_source_types, ANY_P*, double, enum hmon_atkmode_types);
E void play_object_hit_sound_at_location(struct obj*, enum hit_surface_source_types, ANY_P*, double, enum hmon_atkmode_types, xchar, xchar);
E void play_simple_player_sound(enum monster_sound_types);
E void play_simple_player_sound_with_flags(enum monster_sound_types, unsigned int);
E void play_player_line_indexed_sound(enum monster_sound_types, int, enum sound_play_groups play_group, enum immediate_sound_types, unsigned int);
E void play_player_ouch_sound(enum monster_ouch_sounds);
E void play_player_flounder_sound(enum monster_flounder_sounds);
E void play_player_pissed_sound(enum monster_pissed_sounds);
E void play_simple_player_voice(enum player_voice_types);
E void set_simple_player_sound_id_and_volume(enum player_soundset_types, enum monster_sound_types, enum ghsound_types*, float*);
E void set_simple_player_voice_id_and_volume(enum player_soundset_types, enum player_voice_types, enum ghsound_types*, float*);
E void play_simple_monster_sound(struct monst*, enum monster_sound_types);
E void play_simple_monster_sound_with_flags(struct monst*, enum monster_sound_types, unsigned int);
E void set_simple_monster_sound_id_and_volume(enum monster_soundset_types, enum monster_sound_types, enum ghsound_types*, float*);
E void play_monster_simple_weapon_sound(struct monst*, int, struct obj*, enum object_sound_types);
E void play_monster_weapon_hit_sound(struct monst*, enum hit_surface_source_types, ANY_P*, int, struct obj*, double, enum hmon_atkmode_types);
E void play_monster_weapon_hit_sound_at_location(struct monst*, enum hit_surface_source_types, ANY_P*, int, struct obj*, double, enum hmon_atkmode_types, xchar, xchar);
E void play_monster_attack_sound(struct monst*, int, enum object_sound_types);
E void play_monster_attack_floor_sound(struct monst*, int, enum object_sound_types);
E void set_simple_location_sound_id_and_volume(enum location_soundset_types, enum location_sound_types, enum ghsound_types*, float*);
E void play_simple_location_sound(xchar, xchar, enum location_sound_types);
E void play_ui_sound(enum ui_sound_types);
E void play_sfx_sound(enum sfx_sound_types);
E void play_sfx_sound_with_flags(enum sfx_sound_types, unsigned int);
E void play_sfx_sound_at_location(enum sfx_sound_types, int, int);
E void play_sfx_sound_at_location_with_minimum_volume(enum sfx_sound_types, int, int, double);
E void play_immediate_instrument_sound(enum object_instrument_soundset_types, enum instrument_sound_types);
E void play_immediate_instrument_sound_at_location(enum object_instrument_soundset_types, enum instrument_sound_types, int, int);
E void play_immediate_ray_sound_at_location(int, enum ray_sound_types, int, int);
E void start_ambient_ray_sound_at_location(int, int, int);
E void update_ambient_ray_sound_to_location(int, int, int);
E void stop_ambient_ray_sound(int);
E void start_occupation_ambient_sound(enum object_soundset_types, enum object_occupation_types);
E void stop_occupation_ambient_sound(enum object_soundset_types, enum object_occupation_types);
E void play_occupation_immediate_sound(enum object_soundset_types, enum object_occupation_types, enum occupation_sound_types);
E void play_occupation_immediate_sound_at_location(enum object_soundset_types, enum object_occupation_types, enum occupation_sound_types, int, int);
E enum player_soundset_types get_player_soundset(void);
E void dosetsoundvolume(void);
E void update_hearing_array(int);
E void update_ambient_sounds(void);
E void update_hearing_array_and_ambient_sounds(void);
E void clear_hearing_array_and_ambient_sounds(void);
E int get_max_hearing_distance(void);
E void update_hearing_array_and_ambient_sounds_if_point_within_hearing_range(int, int);
E void unblock_vision_and_hearing_at_point(int, int);
E void block_vision_and_hearing_at_point(int, int);
E void new_sound_source(xchar, xchar, enum ghsound_types, double, enum soundsource_types, enum soundsource_ambient_subtypes, ANY_P*);
E void del_sound_source(enum soundsource_types, ANY_P*);
E void save_sound_sources(int, int, int);
E void reset_sound_sources(void);
E void restore_sound_sources(int);
E void sound_stats(const char*, char*, int64_t*, size_t*);
E void relink_sound_sources(boolean);
E void sound_sources_sanity_check(void);
E void obj_move_sound_source(struct obj*, struct obj*);
E boolean any_sound_source(void);
E void snuff_sound_source(int, int);
E void obj_split_sound_source(struct obj*, struct obj*);
E void obj_merge_sound_sources(struct obj*, struct obj*);
E void obj_adjust_sound_volume(struct obj*, double);
E void begin_sound(struct obj*, boolean);
E void end_sound(struct obj*, boolean);
E boolean obj_has_sound_source(struct obj*);
E enum ghsound_types obj_ambient_sound(struct obj*);
E double obj_ambient_sound_volume(struct obj*);
E enum ghsound_types get_location_ambient_sound_type(xchar, xchar, double*, enum soundsource_ambient_subtypes*);
E enum ghsound_types get_dungeon_music(int);
E enum ghsound_types get_level_music(struct d_level*);
E enum ghsound_types get_room_music(struct mkroom*);
E enum ghsound_types get_level_ambient_sounds(struct d_level*);
E enum ghsound_types get_room_ambient_sounds(struct mkroom*);
E enum ghsound_types get_environment_ambient_sounds(void);
E enum ghsound_types get_monster_ambient_sound_id(enum monster_soundset_types);
E float get_monster_ambient_sound_volume(enum monster_soundset_types);
E void get_god_indices(int*, int*);
E void play_intro_text(void);
E void stop_all_immediate_sounds(void);
E void stop_all_long_immediate_sounds(void);
E void stop_all_dialogue_of_mon(struct monst*);
E void stop_all_dialogue_of_mon_on_mobile(struct monst*);
E void play_voice_shopkeeper_welcome(struct monst*, int);
E void play_voice_shopkeeper_simple_line(struct monst*, enum shopkeeper_lines);
E void play_voice_shopkeeper_leave_pick_outside(struct monst*, const char*, int, boolean);
E void play_voice_shopkeeper_cad_line(struct monst*, enum shopkeeper_cad_lines, const char*);
E void play_voice_shopkeeper_candelabrum_candles(struct monst*, struct obj*);
E void play_voice_shopkeeper_izchak_talks(struct monst*, int);
E void play_voice_shopkeeper_for_you(struct monst*, int, int, int64_t);
E void play_voice_shopkeeper_pay_before_buying(struct monst*, int64_t, int64_t);
E void play_voice_shopkeeper_how_dare_you_damage(struct monst*, uchar, const char*, boolean);
E void play_voice_shopkeeper_costly_alteration(struct monst*, struct obj*, enum cost_alteration_types);
E void play_voice_monster_cuss(struct monst*, int);
E void play_voice_monster_cuss_with_god_name(struct monst*, int);
E void play_voice_monster_advice(struct monst*, boolean);
E void play_voice_wizard_of_yendor_cuss(struct monst*, int, int);
E void play_voice_wizard_of_yendor_simple_line(struct monst*, enum wizard_of_yendor_simple_lines);
E void play_monster_line_indexed_sound(struct monst*, enum monster_sound_types, int, enum sound_play_groups, enum immediate_sound_types, double, unsigned int);
E void play_monster_msg_indexed_sound(struct monst*, enum monster_sound_types, int, enum sound_play_groups, enum immediate_sound_types, double, unsigned int);
E void play_monster_simply_indexed_sound(struct monst*, enum monster_sound_types, int, enum sound_play_groups, enum immediate_sound_types, double, const char*, unsigned int);
E void play_monster_item_trading_line(struct monst*, enum monster_item_trading_lines);
E void play_monster_standard_dialogue_line(struct monst*, enum monster_standard_dialogue_lines);
E void play_monster_special_dialogue_line_with_flags(struct monst*, int, unsigned int);
E void play_monster_special_dialogue_line(struct monst*, int);
E void play_monster_standard_sound(struct monst*, enum monster_standard_sounds);
E void play_monster_special_sound_with_flags(struct monst*, int, unsigned int);
E void play_monster_special_sound(struct monst*, int);
E void play_monster_ouch_sound(struct monst*, enum monster_ouch_sounds);
E void play_monster_who_sound(struct monst*, enum monster_who_sounds);
E void play_monster_switchable_who_sound(struct monst*, enum monster_who_sounds, enum monster_standard_dialogue_lines);
E void play_monster_happy_sound(struct monst*, enum monster_happy_sounds);
E void play_monster_unhappy_sound(struct monst*, enum monster_unhappy_sounds);
E void play_monster_flounder_sound(struct monst*, enum monster_flounder_sounds);
E void play_monster_pissed_sound(struct monst*, enum monster_pissed_sounds);
E void play_monster_chat_sound(struct monst*, int);
E enum monster_soundset_types get_monster_soundset(struct monst*);
E enum monster_soundset_types get_monster_soundset_with_subtype(struct monst*, int*);
E void play_voice_com_pager(struct monst*, int, boolean);
E void play_voice_quest_pager(struct monst*, int, boolean);
E void play_voice_quest_leader_whoareyou(struct monst*);
E void play_voice_god_simple_line_at(xchar, xchar, enum god_simple_lines);
E void play_voice_god_simple_line_by_align(aligntyp, enum god_simple_lines);
E void play_voice_oracle_major_consultation(struct monst*, int);
E void play_hermit_dialogue_line(struct monst*, enum ghsound_types, uchar, int);


/* ### sys.c ### */

E void sys_early_init(void);
E void sysopt_release(void);
E void sysopt_seduce_set(int);
E void reset_global_variables(void);
E void reset_item_global_variables(void);

/* ### sys/msdos/sound.c ### */

#ifdef MSDOS
E int assign_soundcard(char *);
#endif

/* ### sp_lev.c ### */

E boolean check_room(xchar *, xchar *, xchar *, xchar *, boolean);
E boolean create_room(xchar, xchar, xchar, xchar, xchar,
                              xchar, xchar, xchar, int, int, int, int, short);
E void create_secret_door(struct mkroom *, xchar);
E boolean
dig_corridor(coord *, coord *, boolean, schar, schar);
E void fill_room(struct mkroom *, boolean);
E boolean load_special(const char *);
E xchar selection_getpoint(int, int, struct opvar *);
E struct opvar *selection_opvar(char *);
E void opvar_free_x(struct opvar *);
E void set_selection_floodfillchk(int FDECL((*), (int,int)));
E void selection_floodfill(struct opvar *, int, int, boolean);
E void create_carpet(xchar, xchar, xchar, xchar, int);
E void reset_splev(void);

/* ### spell.c ### */

E void book_cursed(struct obj *);
#ifdef USE_TRAMPOLI
E int learn(void);
#endif
E int study_book(struct obj *);
E void book_disappears(struct obj *);
E void book_substitution(struct obj *, struct obj *);
E void age_spells(void);
E int docast(void);
E int docastquick(void);
E int docastquick_core(boolean*);
E int dospellmanage(void);
E int dospellview(void);
E int dosetquickspell(void);
E int spell_skilltype(int);
E int spelleffects(int, boolean, struct monst*, boolean*);
E int spelldescription(int);
E int spelldescription_core(int, int);
E const char* get_obj_saving_throw_description(struct obj*);
E const char* get_otyp_saving_throw_description(int);
E int setspellhotkey(int);
E int forgetspell(int);
E int spell_skill_base_success_bonus(int);
E double spell_skill_ulevel_success_bonus_per_level(int);
E int spell_skill_ulevel_success_bonus(int);
E double spell_skill_mana_cost_multiplier(int);
E int dospellmanagemenu(void);
E int tport_spell(int);
E void losespells(void);
E int dovspell(void);
E int dosortspell(void);
E void initialspell(struct obj *);
E boolean already_learnt_spell_type(int);
E int domix(void);
E int getspellcooldown(int);
E const char* domatcompname(const struct materialcomponent*);
E int subdirtype2explosiontype(int);
E void addspellintrinsictimeout(int);
E double get_spell_mana_cost(int);
E double get_spellbook_adjusted_mana_cost(int);
E void deduct_mana_cost(double);
E uchar is_obj_component_for(int, struct obj*);
E uchar is_otyp_component_for_spellbook(int, int, uint64_t*, int*);
#if defined (DUMPLOG) || defined (DUMPHTML)
E void dump_spells(void);
#endif
E void sortspells(void);
E void print_spell_level_text(char*, int, boolean, uchar, boolean);
E void reset_spells(void);
E int64_t get_object_spell_casting_penalty(struct obj*);
E const char* spelltypemnemonic(int);


/* ### steal.c ### */

#ifdef USE_TRAMPOLI
E int stealarm(void);
#endif
E int64_t somegold(int64_t);
E void stealgold(struct monst *);
E int add_to_obj_tracking(struct obj*);
E boolean finish_obj_tracking(int);
E boolean remove_worn_item(struct obj*, boolean);
E boolean remove_worn_item_ex(struct obj *, boolean, boolean);
E int steal(struct monst *, char *);
E boolean mpickobj(struct monst *, struct obj *);
E void stealamulet(struct monst *);
E void maybe_absorb_item(struct monst *, struct obj *, int, int);
E void mdrop_obj(struct monst *, struct obj *, boolean, boolean);
E void mdrop_special_objs(struct monst *);
E void release_monster_objects(struct monst *, int, boolean, boolean);
E void mdrop_droppable_objs(struct monst*);
E struct obj *findgold(struct obj *);
E struct obj* findobjecttype(struct obj*, int);

/* ### steed.c ### */

E void rider_cant_reach(void);
E boolean can_saddle(struct monst *);
E int use_saddle(struct obj *);
E void put_saddle_on_mon(struct obj *, struct monst *);
E boolean can_ride(struct monst *);
E int doride(void);
E int doridenearby(void);
E boolean mount_steed(struct monst *);
E void exercise_steed(void);
E void kick_steed(void);
E void dismount_steed(int);
E void place_monster(struct monst *, int, int);
E boolean stucksteed(boolean);

/* ### teleport.c ### */

E boolean goodpos(int, int, struct monst *, uint64_t);
E boolean enexto(coord *, xchar, xchar, struct permonst *);
E boolean enexto_core(coord *, xchar, xchar,
                              struct permonst *, uint64_t);
E void teleds(int, int, boolean, boolean);
E boolean safe_teleds(boolean, boolean);
E void teleds_with_effects(int, int, boolean, boolean);
E boolean safe_teleds_with_effects(boolean, boolean);
E boolean teleport_pet(struct monst *, boolean);
E void tele(void);
E void controlled_teleportation(void);
E void wiztele(void);
E boolean scrolltele(struct obj *, boolean, boolean);
E boolean modronportaltele(struct trap*, struct monst*);
E int dotelecmd(void);
E int dotele(boolean);
E void level_tele(int, int, d_level, uchar);
E void domagicportal(struct trap *);
E void level_teleport_effect_out(int, int);
E void level_teleport_effect_in(int, int);
E void tele_trap(struct trap *);
E void level_tele_trap(struct trap *, unsigned);
E void rloc_to(struct monst *, int, int);
E void rloc_to_with_effects(struct monst*, int, int);
E boolean rloc(struct monst *, boolean);
E boolean rloc_with_effects(struct monst*, boolean);
E boolean rloc2(struct monst*, boolean, boolean);
E boolean tele_restrict(struct monst *);
E void mtele_trap(struct monst *, struct trap *, int);
E int mlevel_tele_trap(struct monst *, struct trap *,
                               boolean, int);
E boolean rloco(struct obj *);
E int random_teleport_level(void);
E boolean u_teleport_mon(struct monst *, boolean);
E void reset_teleport(void);


/* ### tile.c ### */
#ifdef USE_TILES
E void substitute_tiles(d_level *);
#endif


/* ### tiledata.c ###*/
E void init_tiledata(void);
E boolean has_generic_player_action_tile(enum action_tile_types action);
E int process_tiledata(int, const char*, int*, uchar*);
E boolean is_dir_from_base_dir(int, int, boolean*, boolean*);
E boolean is_zap_char_from_base_zap_char(int, int, boolean*, boolean*);
E int get_enlargement_animation(int);


/* ### timeout.c ### */

E void burn_away_slime(void);
E void nh_timeout(void);
E void fall_asleep(int, boolean);
E void attach_egg_hatch_timeout(struct obj *, int64_t);
E void attach_fig_transform_timeout(struct obj *);
E void kill_egg(struct obj *);
E int hatch_egg(ANY_P *, int64_t);
E void learn_egg_type(int);
E void learn_corpse_type(int);
E int burn_object(ANY_P *, int64_t);
E void begin_burn(struct obj *, boolean);
E void end_burn(struct obj *, boolean);
E int unsummon_item(ANY_P*, int64_t);
E void begin_existence(struct obj*);
E int unsummon_monster(ANY_P*, int64_t);
E void begin_summontimer(struct monst*);
E void begin_timestoptimer(int64_t);
E void restart_timestoptimer(int64_t, int64_t);
E int restart_time(ANY_P*, int64_t);
E void do_storms(void);
E boolean start_timer(int64_t, short, short, ANY_P *);
E int64_t stop_timer(short, ANY_P *);
E int64_t peek_timer(short, ANY_P *);
E void run_timers(void);
E int64_t stop_timer_type(short);

E void obj_move_timers(struct obj *, struct obj *);
E void obj_split_timers(struct obj *, struct obj *);
E void obj_stop_timers(struct obj *);
E void obj_remove_timers(struct obj*);
E boolean obj_has_timer(struct obj *, short);

E void mon_move_timers(struct monst*, struct monst*);
E void mon_split_timers(struct monst*, struct monst*);
E void mon_stop_timers(struct monst*);
E void mon_remove_timers(struct monst*);
E boolean mon_has_timer(struct monst*, short);

E void spot_stop_timers(xchar, xchar, short);
E int64_t spot_time_expires(xchar, xchar, short);
E int64_t spot_time_left(xchar, xchar, short);
E boolean obj_is_local(struct obj *);
E void save_timers(int, int, int);
E void restore_timers(int, int, boolean, int64_t);
E void reset_timers(void);
E void timer_stats(const char *, char *, int64_t *, size_t *);
E void relink_timers(boolean);
E int wiz_timeout_queue(void);
E void timer_sanity_check(void);
E int make_sound_object(ANY_P*, int64_t);
E void property_expiry_message(int, boolean);

/* ### topten.c ### */

E void formatkiller(char *, size_t, int, boolean);
E int observable_depth(d_level *);
E void topten(int, time_t);
E void prscore(int, char **);
E struct toptenentry *get_rnd_toptenentry(void);
E struct obj *tt_oname(struct obj *);
E int64_t encodeconduct(void);
E int64_t encodeachieve(void);

/* ### track.c ### */

E void initrack(void);
E void settrack(void);
E coord *gettrack(int, int);
E void reest_track(void);

/* ### trap.c ### */

E int succeed_untrap(int, boolean);
E boolean burnarmor(struct monst *);
E int erode_obj(struct obj *, const char *, int, int);
E boolean grease_protect(struct obj *, const char *, struct monst *);
E struct trap *maketrap(int, int, int, int, uint64_t);
E void fall_through(boolean);
E struct monst *animate_statue(struct obj *, xchar, xchar, int, int *);
E struct monst *activate_statue_trap(struct trap *, xchar, xchar, boolean);
E void set_utrap(uint64_t, uchar);
E void reset_utrap(boolean);
E void dotrap(struct trap *, unsigned short);
E void seetrap(struct trap *);
E void feeltrap(struct trap *);
E int mintrap(struct monst *);
E void instapetrify(const char *);
E void minstapetrify(struct monst *, boolean);
E void start_delayed_petrification(struct monst*, boolean);
E void start_delayed_sliming(struct monst*, boolean);
E void selftouch(const char *);
E void mselftouch(struct monst *, const char *, boolean);
E void float_up(void);
E void fill_pit(int, int);
E int float_down(int64_t, int64_t);
E void climb_pit(void);
E boolean fire_damage(struct obj *, boolean, xchar, xchar);
E int fire_damage_chain(struct obj *, boolean, boolean, xchar, xchar);
E boolean lava_damage(struct obj *, xchar, xchar);
E void acid_damage(struct obj *);
E int water_damage(struct obj *, const char *, boolean);
E void water_damage_chain(struct obj *, boolean);
E boolean drown(void);
E void drain_en(int);
E int dountrap(void);
E void cnv_trap_obj(int, int, struct trap *, boolean, boolean);
E int untrap(boolean);
E int check_all_box_traps(boolean);
E int check_box_trap(struct obj*, boolean);
E boolean openholdingtrap(struct monst *, boolean *);
E boolean closeholdingtrap(struct monst *, boolean *);
E boolean openfallingtrap(struct monst *, boolean, boolean *);
E boolean chest_trap(struct obj *, int, boolean);
E void deltrap(struct trap *);
E boolean delfloortrap(struct trap *);
E struct trap *t_at(int, int);
E void b_trapped(const char *, int, xchar, xchar);
E boolean unconscious(void);
E void blow_up_landmine(struct trap *);
E int launch_obj(short, int, int, int, int, int);
E boolean launch_in_progress(void);
E void force_launch_placement(void);
E boolean uteetering_at_seen_pit(struct trap *);
E boolean lava_effects(void);
E void sink_into_lava(void);
E void sokoban_guilt(void);
E int untrap_probability(int, int, struct monst*);
E int trap_difficulty(int, boolean, struct monst*);
E const char* get_trap_name(int);
E const char* get_trap_explanation(struct trap*);
E int get_shooting_trap_object(struct trap*);
E void reset_traps(void);

/* ### u_init.c ### */

E void u_init(void);
E void u_skills_init(void);
E char* get_role_achievement_description(uchar);
E uint64_t mon_known_spell_schools(struct monst*);
E boolean is_known_spell_school(uint64_t, int);
E int common_player_selection(void);

/* ### uhitm.c ### */

E void erode_armor(struct monst *, int);
E boolean attack_checks(struct monst *, struct obj *);
E void check_caitiff(struct monst *);
E int find_roll_to_hit(struct monst *, uchar, struct obj *,
                               int *, int *);
E boolean attack(struct monst *);
E boolean hmon(struct monst *, struct obj *, int, int, boolean *);
E int damageum(struct monst *, struct attack *, struct obj*, int);
E void missum(struct monst *, struct attack *, boolean);
E int passive(struct monst *, struct obj *, boolean, int,
                      uchar, boolean);
E void passive_obj(struct monst *, struct obj *, struct attack *);
E void stumble_onto_mimic(struct monst *);
E int flash_hits_mon(struct monst *, struct obj *);
E void light_hits_gremlin(struct monst *, int);
E double adjust_damage(int, struct monst*, struct monst*, short, uint64_t);
E int calculate_damage_dealt_to_player(double);
E int deduct_player_hp(double);
E int deduct_monster_hp(struct monst*, double);
E void get_game_difficulty_multipliers(double*, double*);
E void get_game_difficulty_multipliers_by_level(double*, double*, schar);
E void update_u_facing(uchar);
E void update_u_action(enum action_tile_types);
E void update_m_action(struct monst*, enum action_tile_types);
E void update_u_action_core(enum action_tile_types, uint64_t, uint64_t);
E void update_m_action_core(struct monst*, enum action_tile_types, uint64_t, uint64_t);
E void update_u_action_revert(enum action_tile_types);
E void update_m_action_revert(struct monst*, enum action_tile_types);
E void update_u_action_and_wait(enum action_tile_types);
E void update_m_action_and_wait(struct monst*, enum action_tile_types);
E void display_being_hit(struct monst*, int, int, enum hit_tile_types, int, uint64_t);
E void display_u_being_hit(enum hit_tile_types, int, uint64_t);
E void display_m_being_hit(struct monst*, enum hit_tile_types, int, uint64_t, boolean);
E void u_wait_until_action(void);
E void m_wait_until_action(struct monst*, enum action_tile_types);
E void u_wait_until_end(void);
E void m_wait_until_end(struct monst*, enum action_tile_types);
E void remove_monster_and_nearby_waitforu(struct monst*);
E boolean m_slips_free(struct monst*, struct attack*);
E boolean randomize_obj_unpoison(struct obj*);

/* ### unixmain.c ### */

#if defined(UNIX) && !defined(GNH_MOBILE)
#ifdef PORT_HELP
E void port_help(void);
#endif
E void sethanguphandler(void (*)(int));
E boolean authorize_wizard_mode(void);
E boolean check_user_string(char *);
E char *get_login_name(void);
#endif /* UNIX */

/* ### unixtty.c ### */

#if defined(UNIX) || defined(__BEOS__)
E void gettty(void);
E void settty(const char *);
E void setftty(void);
E void intron(void);
E void introff(void);
E void error(const char *, ...) PRINTF_F(1, 2);
#endif /* UNIX || __BEOS__ */

/* ### unixunix.c ### */

#if defined(UNIX)
#if !defined(GNH_MOBILE)
E void getlock(void);
E void regularize(char *);
#if defined(TIMED_DELAY) && !defined(msleep) && defined(SYSV)
E void msleep(unsigned);
#endif
#ifdef SHELL
E int dosh(void);
#endif /* SHELL */
#if defined(SHELL) || defined(DEF_PAGER) || defined(DEF_MAILREADER)
E int child(int);
#endif
#ifdef PANICTRACE
E boolean file_exists(const char *);
#endif
#else
E void regularize(char*);
#endif /* GNH_MOBILE */
#endif /* UNIX */

/* ### unixres.c ### */

#ifdef UNIX
#ifdef GNOME_GRAPHICS
E int hide_privileges(boolean);
#endif
#endif /* UNIX */

/* ### vault.c ### */

E void newegd(struct monst *);
E void free_egd(struct monst *);
E boolean grddead(struct monst *);
E struct monst *findgd(void);
E void vault_summon_gd(void);
E char vault_occupied(char *);
E void uleftvault(struct monst *);
E void invault(void);
E int gd_move(struct monst *);
E void paygd(boolean);
E int64_t hidden_gold(void);
E int64_t magic_gold(void);
E boolean gd_sound(void);
E void vault_gd_watching(unsigned int);
E int64_t carried_gem_value(void);
E int64_t magic_gem_value(void);
E int64_t contained_gem_value(struct obj*);

/* ### version.c ### */

E uint64_t get_version_number(void);
E uint64_t get_version_compatibility(void);
E char *version_string(char *);
E char *getversionstring(char *);
E int doversion(void);
E int doextversion(void);
#ifdef MICRO
E boolean comp_times(int64_t);
#endif
E boolean check_version(struct version_info *, const char *, boolean);
E boolean uptodate(int, const char *);
E void store_version(int);
E uint64_t get_feature_notice_ver(char *);
E uint64_t get_current_feature_ver(void);
E const char *copyright_banner_line(int);
E void early_version_info(boolean);

#ifdef RUNTIME_PORT_ID
E char *get_port_id(char *);
#endif
#ifdef RUNTIME_PASTEBUF_SUPPORT
E void port_insert_pastebuf(char *);
#endif

/* ### video.c ### */

#ifdef MSDOS
E int assign_video(char *);
#ifdef NO_TERMS
E void gr_init(void);
E void gr_finish(void);
#endif
E void tileview(boolean);
#endif
#ifdef VIDEOSHADES
E int assign_videoshades(char *);
E int assign_videocolors(char *);
#endif

/* ### vis_tab.c ### */

#ifdef VISION_TABLES
E void vis_tab_init(void);
#endif

/* ### vision.c ### */

E void vision_init(void);
E int does_block(int, int, struct rm *);
E void vision_reset(void);
E void vision_recalc(int);
E void block_point(int, int);
E void unblock_point(int, int);
E boolean clear_path(int, int, int, int);
E void do_clear_area(int, int, int,
                             void (*)(int, int, genericptr), genericptr_t);
E unsigned howmonseen(struct monst *);
E void reset_vision(void);

#ifdef VMS

/* ### vmsfiles.c ### */

E int vms_link(const char *, const char *);
E int vms_unlink(const char *);
E int vms_creat(const char *, unsigned int);
E int vms_open(const char *, int, unsigned int);
E boolean same_dir(const char *, const char *);
E int c__translate(int);
E char *vms_basename(const char *);

/* ### vmsmail.c ### */

E uint64_t init_broadcast_trapping(void);
E uint64_t enable_broadcast_trapping(void);
E uint64_t disable_broadcast_trapping(void);
#if 0
E struct mail_info *parse_next_broadcast(void);
#endif /*0*/

/* ### vmsmain.c ### */

E int main(int, char **);
#ifdef CHDIR
E void chdirx(const char *, boolean);
#endif /* CHDIR */
E void sethanguphandler(void (*)(int));
E boolean authorize_wizard_mode(void);

/* ### vmsmisc.c ### */

E void vms_abort(void) NORETURN;
E void vms_exit(int) NORETURN;
#ifdef PANICTRACE
E void vms_traceback(int);
#endif

/* ### vmstty.c ### */

E int vms_getchar(void);
E void gettty(void);
E void settty(const char *);
E void shuttty(const char *);
E void setftty(void);
E void intron(void);
E void introff(void);
E void error(const char *, ...) PRINTF_F(1, 2);
#ifdef TIMED_DELAY
E void msleep(unsigned);
#endif

/* ### vmsunix.c ### */

E void getlock(void);
E void regularize(char *);
E int vms_getuid(void);
E boolean file_is_stmlf(int);
E int vms_define(const char *, const char *, int);
E int vms_putenv(const char *);
E char *verify_termcap(void);
#if defined(CHDIR) || defined(SHELL) || defined(SECURE)
E void privoff(void);
E void privon(void);
#endif
#ifdef SHELL
E int dosh(void);
#endif
#if defined(SHELL) || defined(MAIL)
E int vms_doshell(const char *, boolean);
#endif
#if defined(SUSPEND) && !defined(GNH_MOBILE)
E int dosuspend(void);
#endif
#ifdef SELECTSAVED
E int vms_get_saved_games(const char *, char ***);
#endif

#endif /* VMS */

/* ### weapon.c ### */

E int practice_needed_to_advance(int, int);
E const char *weapon_descr(struct obj *);
E const char* weapon_skill_name(struct obj*);
E const char* otyp_weapon_skill_name(int);
E const char* get_skill_name(int);
E const char* get_skill_plural_name(int);
E int m_weapon_range(struct monst*, struct obj*, struct obj*);
E int weapon_range(struct obj*, struct obj*);
E int basehitval(struct obj*, struct monst*, struct monst*, int);
E int weapon_to_hit_value(struct obj *, struct monst *, struct monst*, int);
E int weapon_dmg_value(struct obj *, struct monst *, struct monst*, int);
E int weapon_extra_dmg_value(struct obj*, struct monst*, struct monst*, int);
E int weapon_total_dmg_value(struct obj*, struct monst*, struct monst*, int);
E int get_critical_strike_percentage_chance(struct obj*, struct monst*, struct monst*);
E boolean eligible_for_extra_damage(struct obj*, struct monst*, struct monst*);
E int special_dmgval(struct monst *, struct monst *, int64_t, int64_t *);
E void silver_sears(struct monst *, struct monst *, int64_t);
E struct obj *select_rwep(struct monst *);
E boolean monmightthrowwep(struct obj *);
E struct obj *select_hwep(struct monst *, boolean, xchar, xchar);
E struct obj* select_multiweapon_nth_hwep(struct monst*, int);
E void possibly_unwield(struct monst *, boolean);
E void mwepgone(struct monst *);
E int mon_wield_item(struct monst *, boolean, xchar, xchar);
E int u_strdex_to_hit_bonus(void);
E int u_ranged_strdex_to_hit_bonus(void);
E int u_str_dmg_bonus(void);
E int u_thrown_str_dmg_bonus(void);
E int m_strdex_to_hit_bonus(struct monst*);
E int m_ranged_strdex_to_hit_bonus(struct monst*);
E int m_str_dmg_bonus(struct monst*);
E int m_thrown_str_dmg_bonus(struct monst*);
E int strength_damage_bonus(int);
E double strength_damage_bonus_core(int, boolean);
E int strength_tohit_bonus(int);
E double strength_tohit_bonus_core(int, boolean);
E int dexterity_ac_bonus(int);
E int dexterity_tohit_bonus(int);
E int dexterity_ranged_tohit_bonus(int);
E double constitution_hp_bonus(int);
E void wet_a_towel(struct obj *, int, boolean);
E void dry_a_towel(struct obj *, int, boolean);
E const char* skill_level_name_core(int);
E char *skill_level_name(int, char *, uchar);
E const char *skill_name(int, boolean);
E boolean can_advance(int, boolean);
E void update_can_advance_any_skill(void);
E int doskill(void);
E int doskill_core(void);
E int enhance_weapon_skill(void);
E void unrestrict_weapon_skill(enum p_skills);
E void add_weapon_skill_maximum_by_one(enum p_skills);
E void use_skill(int, int);
E void add_weapon_skill(int);
E void lose_weapon_skill(int);
E enum p_skills weapon_skill_type(struct obj *);
E enum p_skills uwep_skill_type(void);
E int weapon_skill_hit_bonus(struct obj *, int, boolean, boolean, uchar, int, boolean, boolean, boolean);
E int weapon_skill_dmg_bonus(struct obj *, int, boolean, boolean, uchar, int, boolean, boolean, boolean);
E int shield_skill_ac_bonus(int);
E int shield_skill_mc_bonus(int);
E int dodge_skill_ac_bonus(int);
E int wand_skill_hit_bonus(int);
E int digging_skill_speed_bonus(int);
E int whip_skill_weapon_disarm_bonus(int);
E int whip_skill_weapon_disarm_bonus(int);
E int spear_skill_jousting_bonus(int);
E int riding_skill_jousting_bonus(int);
E int riding_skill_dmg_bonus(int);
E int riding_skill_hit_bonus(int);
E int riding_skill_saddling_bonus(int);
E int riding_skill_mount_bonus(int);
E void skill_init(const struct def_skill*, const struct def_skill *);
E int martial_arts_multishot_percentage_chance(int);
E int two_handed_weapon_multishot_percentage_chance(int);
E int get_skill_critical_strike_chance(enum p_skills, boolean, boolean, int, boolean, boolean);
E double get_wand_skill_damage_multiplier(int);
E double get_wand_exceptionality_damage_multiplier(uchar);
E int get_exceptionality_multiplier(uchar);
E void print_weapon_style_string(char*, boolean);
E void print_quivered_weapon_style_string(char*);
E int limited_skill_level(enum p_skills, boolean, boolean);
E int adjusted_limited_skill_level(enum p_skills, boolean, boolean);
E int adjusted_skill_level(enum p_skills);
E int exceptionality_digging_speed_bonus(struct obj*);
E int exceptionality_weapon_disarm_bonus(struct obj*);
E short get_obj_wsdice(struct obj*);
E short get_obj_wsdam(struct obj*);
E short get_obj_wsdmgplus(struct obj*);
E short get_obj_wldice(struct obj*);
E short get_obj_wldam(struct obj*);
E short get_obj_wldmgplus(struct obj*);
E void dump_skills(void);


/* ### were.c ### */

E void were_change(struct monst *);
E int counter_were(int);
E int were_beastie(int);
E void new_were(struct monst *);
E int were_summon(struct permonst *, boolean, int *, char *);
E void you_were(void);
E void you_unwere(boolean);
E void set_ulycn(int);

/* ### wield.c ### */

E void setuwep(struct obj *, int64_t);
E void setuwepcore(struct obj*, int64_t, boolean);
E void setuwepquietly(struct obj*, int64_t);
E void setuqwep(struct obj *);
E void setuswapwep(struct obj *, int64_t);
E void setuqwepquietly(struct obj*);
E void setuswapwepquietly(struct obj*, int64_t);
E int dowield(void);
E int dounwield(void);
E int dowieldprevwep(void);
E int doswaphandedness(void);
E int doswapweapon_right_or_both(void);
E int doswapweapon(void);
E int dosingleswapweapon(int64_t, int64_t);
E int dowieldquiver(void);
E boolean wield_tool(struct obj *, const char *);
E void drop_uswapwep(void);
E int dotwoweapon(void);
E void uwepgone(void);
E void uwep2gone(void);
E void uswapwepgone(void);
E void uswapwep2gone(void);
E void uqwepgone(void);
E void untwoweapon(void);
E int enchant_weapon(struct obj *, struct obj*, int, boolean);
E int welded(struct obj *, struct monst*);
E void weldmsg(struct obj *);
E void setmnotwielded(struct monst *, struct obj *);
E boolean mwelded(struct obj *, struct monst*);
E void update_unweapon(void);
E void update_hand_unweapon(int);
E int wield_weapon(struct obj*);

/* ### windows.c ### */

E void choose_windows(const char *);
#ifdef WINCHAIN
void addto_windowchain(const char *s);
void commit_windowchain(void);
#endif
E boolean genl_can_suspend_no(void);
E boolean genl_can_suspend_yes(void);
E char genl_message_menu(char, int, const char *);
E void genl_preference_update(const char *);
E char *genl_getmsghistory_ex(char**, char**, boolean);
E void genl_putmsghistory_ex(const char *, const char*, const char*, boolean);
#ifdef HANGUPHANDLING
E void nhwindows_hangup(void);
#endif
E void genl_status_init(int);
E void genl_status_finish(void);
E void genl_status_enablefield(int, const char *, const char *, boolean);
E void genl_status_update(int, genericptr_t, int, int, int, uint64_t *);

E void genl_stretch_window(void);
E void genl_set_animation_timer_interval(unsigned int);
E int genl_open_special_view(struct special_view_info);
E void genl_stop_all_sounds(struct stop_all_info);
E void genl_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info);
E void genl_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info);
E void genl_set_effect_ambient_volume(struct effect_ambient_volume_info);
E void genl_play_ghsound_level_ambient(struct ghsound_level_ambient_info);
E void genl_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info);

E void genl_play_ghsound_music(struct ghsound_music_info);
E void genl_play_immediate_ghsound(struct ghsound_immediate_info);
E void genl_adjust_ghsound_general_volumes(void);
E void genl_add_ambient_ghsound(struct soundsource_t*);
E void genl_delete_ambient_ghsound(struct soundsource_t*);
E void genl_set_ambient_ghsound_volume(struct soundsource_t*);
E void genl_clear_context_menu(void);
E void genl_add_context_menu(int, int, int, int, const char*, const char*, int, int);
E void genl_update_status_button(int, int, int, uint64_t);
E void genl_toggle_animation_timer(int, int, int, int, int, int, uint64_t);
E void genl_display_floating_text(int, int, const char*, int, int, int, uint64_t);
E void genl_display_screen_text(const char*, const char*, const char*, int, int, int, uint64_t);
E void genl_display_popup_text(const char*, const char*, int, int, int, int, uint64_t);
E void genl_display_gui_effect(int, int, int, int, int, int, uint64_t);
E void genl_update_cursor(int, int, int);
E int genl_ui_has_input(void);
E void genl_exit_hack(int);

E void write_snapshot_json(time_t, const char*, const char*);
E void dump_open_log(time_t, boolean);
E void dump_close_log(void);
E void dump_forward_putstr(winid, int, const char*, int, int);
#if defined (DUMPLOG) || defined (DUMPHTML)
E void dump_redirect(boolean);
E void dump_start_screendump(void); /* defined in windows.c */
E void dump_end_screendump(void);
E char* dump_fmtstr(const char*, char*);
E void dump_putstr_no_utf8(winid, int, const char*);
E char* print_snapshot_json_filename_to_buffer(char*);
#ifdef DUMPLOG
E char* print_dumplog_filename_to_buffer(char*);
#endif
#ifdef DUMPHTML
E char* print_dumphtml_filename_to_buffer(char*);
E void html_dump_glyph(int, int, int, nhsym, int, uint64_t);
#endif
#endif
#if defined(ANDROID) && defined(DUMPLOG)
E void and_get_dumplog_dir(char*);
#endif
E void itemdesc_redirect(void);
E void reset_windows(void);


/* ### winnt.c ### */
#ifdef WIN32
E void GnollHack_enter_winnt(void);
#endif

/* ### wizard.c ### */

E void amulet(void);
E int mon_has_amulet(struct monst *);
E int mon_has_special(struct monst *);
E int tactics(struct monst *);
E boolean has_aggravatables(struct monst *);
E void aggravate(void);
E void clonewiz(void);
E int pick_nasty(int, int);
E int summon_nasties(struct monst *);
E int summon_level_appropriate_monsters(struct monst*);
E void resurrect(void);
E void intervene(void);
E void wizdead(void);
E boolean cuss(struct monst *);

/* ### worm.c ### */

E int get_wormno(void);
E void initworm(struct monst *, int);
E void worm_move(struct monst *);
E void worm_nomove(struct monst *);
E void wormgone(struct monst *);
E void wormhitu(struct monst *);
E void cutworm(struct monst *, xchar, xchar, boolean);
E void see_wsegs(struct monst *);
E void detect_wsegs(struct monst *, boolean);
E void save_worm(int, int);
E void reset_worm(void);
E void rest_worm(int);
E void place_wsegs(struct monst *, struct monst *);
E void sanity_check_worm(struct monst *);
E void remove_worm(struct monst *);
E void check_and_remove_worm_from_map(struct monst*);
E void place_worm_tail_randomly(struct monst *, xchar, xchar);
E int size_wseg(struct monst *);
E int count_wsegs(struct monst *);
E boolean worm_known(struct monst *);
E boolean worm_cross(int, int, int, int);
E int wseg_at(struct monst *, int, int);
E int get_wseg_dir_at(struct monst*, int, int);
E int get_reverse_prev_wseg_dir_at(struct monst*, int, int);
E boolean is_wseg_head(struct monst*, int, int);
E boolean is_wseg_tailend(struct monst*, int, int);


/* ### worn.c ### */

E void setworn(struct obj *, int64_t);
E boolean setnotworn(struct obj *);
E void setworncore(struct obj*, int64_t, boolean);
E void setwornquietly(struct obj*, int64_t);
E boolean setnotworncore(struct obj*, boolean);
E boolean setnotwornquietly(struct obj*);
E struct obj *wearmask_to_obj(int64_t);
E int64_t wearslot(struct obj *);
//E void mon_adjust_speed(struct monst *, int, struct obj *);
E void update_mon_extrinsics(struct monst*, boolean);
E void update_all_mon_statistics(struct monst *, boolean);
E void update_all_mon_statistics_core(struct monst*, boolean);
E int find_mac(struct monst *);
E void m_dowear(struct monst *, boolean, boolean);
E struct obj *which_armor(struct monst *, int64_t);
E void mon_break_armor(struct monst *, boolean);
E void bypass_obj(struct obj *);
E void clear_bypasses(void);
E void bypass_objlist(struct obj *, boolean);
E struct obj *nxt_unbypassed_obj(struct obj *);
E struct obj *nxt_unbypassed_loot(Loot *, struct obj *);
E int racial_exception(struct monst *, struct obj *);
E void nonadditive_increase_mon_property(struct monst*, int, int);
E boolean nonadditive_increase_mon_property_verbosely(struct monst*, int, int);
E void increase_mon_property(struct monst*, int, int);
E void increase_mon_property_limited(struct monst*, int, int, int);
E int get_mon_property(struct monst*, int);
E void set_mon_property(struct monst*, int, int);
E boolean set_mon_property_verbosely(struct monst*, int, int);
E boolean verbose_wrapper(enum verbose_function_types, struct monst*, int, int, boolean);
E boolean increase_mon_property_verbosely(struct monst*, int, int);
E boolean increase_mon_property_verbosely_limited(struct monst*, int, int, int);
E boolean increase_mon_property_b(struct monst*, int, int, boolean);
E boolean increase_mon_property_b_limited(struct monst*, int, int, int, boolean);
E boolean nonadditive_increase_mon_property_b(struct monst*, int, int, boolean);
E boolean set_mon_property_b(struct monst*, int, int, boolean);
E int count_unworn_items(struct obj*);


/* ### write.c ### */

E int dowrite(struct obj *);
E int ink_cost(struct obj*);
E int otyp_ink_cost(int);

/* ### zap.c ### */

E void learnwand(struct obj *);
E int get_saving_throw_adjustment(struct obj*, struct monst*, struct monst*);
E int get_spell_skill_level_saving_throw_adjustment(int);
E int get_wand_skill_level_saving_throw_adjustment(int);
E int bhitm(struct monst*, struct obj*, struct monst*);
E void probe_monster(struct monst *);
E int probe_object(struct obj*);
E void display_monster_information(struct monst*);
E void print_monster_intrinsics(winid win, struct monst*, struct permonst*);
E void print_monster_wearables(winid win, struct monst*, struct permonst*);
E void print_monster_status(winid win, struct monst*);
E void print_monster_statistics(winid win, struct monst*, struct permonst*);
E void display_monster_inventory(struct monst*, boolean);
E boolean get_obj_location(struct obj *, xchar *, xchar *, int);
E boolean get_mon_location(struct monst *, xchar *, xchar *, int);
E boolean get_region_location(struct nhregion*, xchar*, xchar*, int);
E struct monst *get_container_location(struct obj * obj, int *, int *);
E struct monst *montraits(struct obj *, coord *, boolean, int, int, uint64_t);
E struct monst *revive(struct obj *, boolean, int, boolean);
E int revive_from_inventory(struct monst *);
E void cancel_item(struct obj *, boolean);
E boolean drain_item(struct obj *, boolean);
E struct obj *poly_obj(struct obj *, int);
E boolean obj_resists(struct obj *, int, int);
E boolean obj_shudders(struct obj *);
E void do_osshock(struct obj *);
E int bhito(struct obj *, struct obj *, struct monst*);
E int bhitpile(struct obj *, struct monst*, int (*)(OBJ_P, OBJ_P, MONST_P), int, int, schar, uchar, boolean);
E int bhitt(struct trap*, struct obj*, struct monst*);
E int zappable(struct obj *);
E void zapnodir(struct obj *);
E int dozap(void);
E int dozapquick(void);
E int dozapquick_core(boolean*);
E int dosetquickwand(void);
E int dounsetquickwand(void);
E double zapyourself(struct obj *, boolean);
E void ubreatheu(struct attack *);
E double lightdamage(struct obj *, boolean, int);
E boolean flashburn(int64_t);
E boolean cancel_monst(struct monst *, struct obj *, boolean, boolean, boolean, int, int);
E boolean add_temporary_property(struct monst*, struct obj*, boolean, boolean, boolean, int, int);
E void zapsetup(void);
E void zapwrapup(void);
E void weffects(struct obj *);
E const char *exclam(int force);
E void hit(const char *, struct monst *, const char *, int, const char*);
E void hit_with_hit_tile(const char*, struct monst*, const char*, int, const char*, enum hit_tile_types, boolean);
E void miss(const char *, struct monst *);
E struct monst *bhit(int, int, int, int, enum bhit_call_types,
                             int (*)(MONST_P, OBJ_P, MONST_P),
                             int (*)(OBJ_P, OBJ_P, MONST_P), 
                             int (*)(TRAP_P, OBJ_P, MONST_P), 
                             struct obj **, struct monst*, uchar, boolean);
E struct monst *boomhit(struct obj *, int, int);
E double zhitm(struct monst *, int, struct obj*, struct monst*, int, int, int, struct obj **, uchar*);
E int burn_floor_objects(int, int, boolean, boolean);
E void buzz(int, struct obj*, struct monst*, int, int, int, xchar, xchar, int, int);
E void dobuzz(int, struct obj*, struct monst*, int, int, int, xchar, xchar, int, int, boolean);
E void melt_ice(xchar, xchar, const char *);
E void start_melt_ice_timeout(xchar, xchar, int64_t);
E int melt_ice_away(ANY_P *, int64_t);
E int zap_over_floor(xchar, xchar, int, boolean *, short);
E boolean fracture_rock(struct obj *, boolean);
E boolean pre_break_statue(struct obj*);
E boolean break_statue(struct obj *);
E void destroy_item(int, int);
E int destroy_mitem(struct monst *, int, int);
E void destroy_one_item(struct obj*, int, int, boolean);
E boolean is_obj_protected_by_property(struct obj*, struct monst*, int);
E boolean check_magic_resistance_and_inflict_damage(struct monst *, struct obj *, struct monst*, boolean, int, int, int);
E boolean inflict_spell_damage(struct monst*, struct obj*, struct monst*, int, int, int);
E void makewish(boolean, boolean);
E void summonitem(struct obj*, int);
E void summonblackblade(struct obj*);
E void summonmagearmor(struct obj*);
E struct monst* summoncreature(int, int, const char*, uint64_t, uint64_t); // int, boolean, boolean, boolean, boolean, boolean, boolean, boolean));
E void summondemon(int);
E void summondemogorgon(int);
E void summonbahamut(int);
E void armageddon(void);
E void timestop(int, int);
E boolean check_rider_disintegration(struct monst*, const char*);
E boolean check_rider_death_absorption(struct monst*, const char*);
E void maybe_disintegrate_mon(struct monst*, int, const char*);
E void disintegrate_mon(struct monst*, int, const char*);
E boolean item_prevents_revival(int);
E boolean item_prevents_summoning(int);
E int get_displayed_object_type_from_subdir_type(int);
E int stone_to_flesh_obj(struct obj*);
E int mon_to_zombie(int);
E int mon_to_mummy(int);
E int get_spell_damage(int, uchar, struct monst*, struct monst*);
E int get_spell_skill_level(int, struct monst*, struct monst*);
E int get_obj_spell_duration(struct obj*);
E int get_obj_spell_max_duration(struct obj*);
E int get_otyp_spell_duration(int);
E int get_otyp_spell_max_duration(int);
E int get_maximum_applicable_spell_damage_level(int, struct monst*);
E boolean is_buzztype_breath_weapon(int);
E boolean is_buzztype_eyestalk(int);
E void wish_insurance_check(int);
E boolean get_wand_explosion_damage(struct obj*, int*, int*, int*, short*, boolean);
E double get_wand_skill_explosion_damage_adjustment(int);
E void reset_zap(void);

#endif /* !MAKEDEFS_C && !LEV_LEX_C */

#undef E

#endif /* EXTERN_H */
