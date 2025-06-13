/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    context.h    $NHDT-Date: 1455907260 2016/02/19 18:41:00 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.30 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

/* If you change the context structure make sure you increment EDITLEVEL in */
/* patchlevel.h if needed. */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "animation.h"
#include "dungeon.h"
#include "general.h"
#include "layer.h"
#include "prop.h"
#include "quest.h"

#define CONTEXTVERBSZ 30

/*
 * The context structure houses things that the game tracks
 * or adjusts during the game, to preserve game state or context.
 *
 * The entire structure is saved with the game.
 *
 */

struct dig_info { /* apply.c, hack.c */
    int effort;
    d_level level;
    coord pos;
    int64_t lastdigtime;
    boolean down, chew, warned, quiet;
    int skill;
};

struct tin_info {
    struct obj *tin;
    unsigned o_id; /* o_id of tin in save file */
    int usedtime, reqtime;
};

struct book_info {
    struct obj *book; /* last/current book being xscribed */
    unsigned o_id;    /* o_id of book in save file */
    schar delay;      /* moves left for this spell */
    uchar reading_result;
};

#define READING_RESULT_SUCCESS 0
#define READING_RESULT_FAIL 1
#define READING_RESULT_CONFUSED 2

struct takeoff_info {
    int64_t mask;
    int64_t what;
    int delay;
    boolean cancelled_don;
    char disrobing[CONTEXTVERBSZ + 1];
    uchar command;
};

#define WEAR_OID_BITS 32
struct wear_info {
    int64_t mask;
    unsigned oid[WEAR_OID_BITS]; /* oid of the item intended to be worn */
    int64_t what;
    int delay;
};


struct victual_info {
    struct obj *piece; /* the thing being eaten, or last thing that
                        * was partially eaten, unless that thing was
                        * a tin, which uses the tin structure above,
                        * in which case this should be 0 */
    unsigned o_id;     /* o_id of food object in save file */
    /* doeat() initializes these when piece is valid */
    int usedtime,          /* turns spent eating */
        reqtime;           /* turns required to eat */
    int nmod;              /* coded nutrition per turn */
    int total_nutrition;   /* total nutrition of the piece when start_eating was called */
    uchar canchoke; /* was satiated at beginning */

    /* start_eating() initializes these */
    uchar fullwarn; /* have warned about being full */
    uchar eating;   /* victual currently being eaten */
    uchar doreset;  /* stop eating at end of turn */
};

struct warntype_info {
    uint64_t obj;        /* object warn_of_mon monster type M2 */
    uint64_t polyd;      /* warn_of_mon monster type M2 due to poly */
    struct permonst *species; /* particular species due to poly */
    short speciesidx; /* index of above in mons[] (for save/restore) */
};

struct polearm_info {
    struct monst *hitmon; /* the monster we tried to hit last */
    unsigned m_id;        /* monster id of hitmon, in save file */
};

struct obj_split {
    unsigned parent_oid, /* set: splitobj(),         */
             child_oid;  /* reset: clear_splitobjs() */
};

struct tribute_info {
    uint64_t tributesz;       /* make it possible to skip this in future */
    uchar enabled;        /* Do we have tributes turned on? */
    uchar bookstock; /* Have we stocked the book? */
    uchar Deathnotice;    /* Did Death notice the book? */
};

struct novel_tracking { /* for choosing random passage when reading novel */
    unsigned id;        /* novel oid from previous passage selection */
    int count;          /* number of passage indices available in pasg[] */
    xchar pasg[30];     /* pasg[0..count-1] are passage indices */
    /* tribute file is allowed to have more than 30 passages for a novel;
       if it does, reading will first choose a random subset of 30 of them;
       reading all 30 or switching to a different novel and then back again
       will pick a new subset, independent of previous ones so might not
       contain all--or even any--of the ones left out of the prior subset;
       chatting with Death works the same as reading a novel except that
       sometimes he delivers one of a few hardcoded messages rather than a
       passage from the Death Quotes section of dat/tribute */
};

struct context_info 
{
    boolean game_started;
    schar game_difficulty;

    unsigned ident;         /* social security number for each monster */
    unsigned no_of_wizards; /* 0, 1 or 2 (wizard and his shadow) */
    unsigned run;           /* 0: h (etc), 1: H (etc), 2: fh (etc) */
                            /* 3: FH, 4: ff+, 5: ff-, 6: FF+, 7: FF- */
                            /* 8: travel */
    unsigned startingpet_mid;
    int current_fruit; /* fruit->fid corresponding to pl_fruit[] */
    int warnlevel;
    int rndencode;          /* randomized escape sequence introducer */
    int64_t next_attrib_check; /* next attribute check */
    int64_t stethoscope_move;
    short stethoscope_movement;
    boolean travel;  /* find way automatically to u.tx,u.ty */
    boolean travel1; /* first travel step */
    uchar travel_mode;
    unsigned tmid; /* target monster id for travel attack */
    unsigned toid; /* target object id for travel pickup */
    boolean forcefight;
    boolean nopick; /* do not pickup objects (as when running) */
    boolean made_amulet;
    boolean made_deserted_shop;
    boolean made_orc_and_a_pie;
    boolean made_orc_and_a_pie2;
    boolean made_witch_king;
    boolean spellpray;    /* is praying initiated by the prayer spell */
    int made_shop_count;
    int made_temple_count;
    int made_armory_box_count;
    char used_names[512 * 32];
    boolean encounter_appeared[256];
    int shop_identify_type;
    int npc_identify_type;
    boolean mon_moving; /* monsters' turn to move */
    boolean mon_talking; /* a monster is talking this turn */
    boolean hungry_message_displayed;
    boolean move;
    boolean mv;
    boolean bypasses;    /* bypass flag is set on at least one fobj */
    boolean botl;        /* partially redo status line */
    boolean botlx;       /* print an entirely new bottom line */
    boolean door_opened; /* set to true if door was opened during test_move */
    boolean time_stopped; /* time has been stopped */
    boolean multishot_target_killed;
    boolean hide_melee_range_warning;
    boolean suit_yielding_ac_bonus;
    boolean suit_yielding_mc_bonus;
    boolean robe_yielding_ac_bonus;
    boolean robe_yielding_mc_bonus;
    int oddideacnt;
    int mummyrot_advancement;
    int bhitcount;
    boolean bootkickmsgdisplayed;
    boolean glovehitmsgdisplayed;
    struct dig_info digging;
    struct victual_info victual;
    struct tin_info tin;
    struct book_info spbook;
    struct takeoff_info takeoff;
    struct wear_info wear;
    struct warntype_info warntype;
    struct polearm_info polearm;
    struct obj_split objsplit; /* track most recently split object stack */
    struct tribute_info tribute;
    struct novel_tracking novel;
    uchar used_fountain_subtype[MAX_FOUNTAIN_SUBTYPES];
    unsigned last_picked_obj_oid;
    short last_picked_obj_show_duration_left;
    boolean click_kick_query;

    boolean u_action_animation_counter_on;
    boolean m_action_animation_counter_on;
    boolean explosion_animation_counter_on;
    boolean zap_animation_counter_on[MAX_PLAYED_ZAP_ANIMATIONS];
    boolean special_effect_animation_counter_on[MAX_PLAYED_SPECIAL_EFFECTS];

    uchar m_action_animation_x;
    uchar m_action_animation_y;
    uchar explosion_animation_x;
    uchar explosion_animation_y;
    uchar zap_animation_x[MAX_PLAYED_ZAP_ANIMATIONS];
    uchar zap_animation_y[MAX_PLAYED_ZAP_ANIMATIONS];
    uchar spef_action_animation_x[MAX_PLAYED_SPECIAL_EFFECTS];
    uchar spef_action_animation_y[MAX_PLAYED_SPECIAL_EFFECTS];
    enum layer_types spef_action_animation_layer[MAX_PLAYED_SPECIAL_EFFECTS];

    uint64_t u_intervals_to_wait_until_action;
    uint64_t u_intervals_to_wait_until_end;
    uint64_t m_intervals_to_wait_until_action;
    uint64_t m_intervals_to_wait_until_end;
    uint64_t zap_aggregate_intervals_to_wait_until_action;
    uint64_t zap_aggregate_intervals_to_wait_until_end;
    uint64_t spef_intervals_to_wait_until_action[MAX_PLAYED_SPECIAL_EFFECTS];
    uint64_t spef_intervals_to_wait_until_end[MAX_PLAYED_SPECIAL_EFFECTS];
    uint64_t expl_intervals_to_wait_until_action;
    uint64_t expl_intervals_to_wait_until_end;

    boolean force_allow_keyboard_commands;
    int makemon_spef_idx;
    uint64_t global_newsym_flags;
    float global_minimum_volume;
    int tether_x;
    int tether_y;
    boolean town_portal_return_level_set;
    d_level town_portal_return_level;
    int64_t last_turn_when_took_damage;

    boolean reviving;
    boolean quit_pressed;

    uint64_t npc_made;
    unsigned int view_pet_mid;
    boolean first_time_cmd;
    boolean starting_prayer_timeout_expired;
    uchar dlords_summoned_via_altar;
    boolean skip_botl;
    boolean amonket_generated;
    uchar town_portal_return_flags;
    uint64_t quest_flags;
    int64_t role_score;

    unsigned quick_zap_wand_oid;
    int quick_cast_spell_no;
    boolean quick_cast_spell_set;
    boolean in_memory_objs;
    boolean save_checkpoint;
    boolean reserved_boolC;
    boolean reserved_boolD;
    boolean reserved_boolE;
    boolean reserved_boolF;
    boolean reserved_boolG;

    /* Emergency reserved variables to make non-save-game-breaking changes */
    int64_t reserved_int64_1;
    int64_t reserved_int64_2;
    int64_t reserved_int64_3;
    int64_t reserved_int64_4;
    
    int reserved_int1;
    int reserved_int2;
    int reserved_int3;
    int reserved_int4;
    short reserved_short1;
    short reserved_short2;
    short reserved_short3;
    short reserved_short4;
    char reserved_char1;
    char reserved_char2;
    char reserved_char3;
    char reserved_char4;

    /* Emergency reserved booleans to make non-save-game-breaking changes */
    boolean reserved_bool1;
    boolean reserved_bool2;
    boolean reserved_bool3;
    boolean reserved_bool4;
    boolean reserved_bool5;
    boolean reserved_bool6;
    boolean reserved_bool7;
    boolean reserved_bool8;
};

extern NEARDATA struct context_info context;

#define QUEST_FLAGS_HEARD_OF_BELL                       0x00000001UL
#define QUEST_FLAGS_HEARD_OF_BOOK                       0x00000002UL
#define QUEST_FLAGS_HEARD_OF_MENORAH                    0x00000004UL
#define QUEST_FLAGS_HEARD_OF_BELL_OWNER                 0x00000008UL
#define QUEST_FLAGS_HEARD_OF_BOOK_OWNER                 0x00000010UL
#define QUEST_FLAGS_HEARD_OF_MENORAH_OWNER              0x00000020UL
#define QUEST_FLAGS_HEARD_OF_VIBRATING_SQUARE           0x00000040UL
#define QUEST_FLAGS_HEARD_OF_AMULET_IN_SANCTUM          0x00000080UL
#define QUEST_FLAGS_HEARD_OF_RITUAL                     0x00000100UL
#define QUEST_FLAGS_HEARD_OF_BELL_OWNER_IS_NEMESIS      0x00000200UL
#define QUEST_FLAGS_HEARD_OF_AMULET_IN_GEHENNOM         0x00000400UL
#define QUEST_FLAGS_HEARD_ORACLE_KNOWS_MORE_DETAILS     0x00000800UL

#endif /* CONTEXT_H */
