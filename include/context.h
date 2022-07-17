/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-04-16 */

/* GnollHack 4.0    context.h    $NHDT-Date: 1455907260 2016/02/19 18:41:00 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.30 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

/* If you change the context structure make sure you increment EDITLEVEL in */
/* patchlevel.h if needed. */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "prop.h"
#include "animation.h"
#include "layer.h"
#include "general.h"
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
    long lastdigtime;
    boolean down, chew, warned, quiet;
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
    long mask;
    long what;
    int delay;
    boolean cancelled_don;
    char disrobing[CONTEXTVERBSZ + 1];
    uchar command;
};

#define WEAR_OID_BITS 32
struct wear_info {
    long mask;
    unsigned oid[WEAR_OID_BITS]; /* oid of the item intended to be worn */
    long what;
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
    Bitfield(canchoke, 1); /* was satiated at beginning */

    /* start_eating() initializes these */
    Bitfield(fullwarn, 1); /* have warned about being full */
    Bitfield(eating, 1);   /* victual currently being eaten */
    Bitfield(doreset, 1);  /* stop eating at end of turn */
};

struct warntype_info {
    unsigned long obj;        /* object warn_of_mon monster type M2 */
    unsigned long polyd;      /* warn_of_mon monster type M2 due to poly */
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
    size_t tributesz;       /* make it possible to skip this in future */
    boolean enabled;        /* Do we have tributes turned on? */
    Bitfield(bookstock, 1); /* Have we stocked the book? */
    Bitfield(Deathnotice,1);    /* Did Death notice the book? */
    /* Markers for other tributes can go here */
    /* 30 free bits */
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
    long next_attrib_check; /* next attribute check */
    long stethoscope_move;
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
    char used_names[BUFSIZ * 32];
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

    unsigned long u_intervals_to_wait_until_action;
    unsigned long u_intervals_to_wait_until_end;
    unsigned long m_intervals_to_wait_until_action;
    unsigned long m_intervals_to_wait_until_end;
    unsigned long zap_aggregate_intervals_to_wait_until_action;
    unsigned long zap_aggregate_intervals_to_wait_until_end;
    unsigned long spef_intervals_to_wait_until_action[MAX_PLAYED_SPECIAL_EFFECTS];
    unsigned long spef_intervals_to_wait_until_end[MAX_PLAYED_SPECIAL_EFFECTS];
    unsigned long expl_intervals_to_wait_until_action;
    unsigned long expl_intervals_to_wait_until_end;

    boolean force_allow_keyboard_commands;
    int makemon_spef_idx;
    unsigned long global_newsym_flags;
    float global_minimum_volume;
    int tether_x;
    int tether_y;
    boolean town_portal_return_level_set;
    d_level town_portal_return_level;
    long last_turn_when_took_damage;
    struct gh_quest_status_data quest_status_info[NUM_QUESTS];

    boolean reviving;
    boolean quit_pressed;

    unsigned long npc_made;
    unsigned int view_pet_mid;
    boolean first_time_cmd;
    boolean starting_prayer_timeout_expired;
    uchar dlords_summoned_via_altar;

    /* Emergency reserved booleans to make non-save-game-breaking changes */
    boolean reserved_bool1;
    boolean reserved_bool2;
    boolean reserved_bool3;
    boolean reserved_bool4;

    /* Emergency reserved variables to make non-save-game-breaking changes */
    char reserved_char1;
    char reserved_char2;
    short reserved_short1;
    short reserved_short2;
    int reserved_int1;
    int reserved_int2;
    unsigned long reserved_ulong1;
    unsigned long reserved_ulong2;

};

extern NEARDATA struct context_info context;

#endif /* CONTEXT_H */
