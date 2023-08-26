/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    hack.h    $NHDT-Date: 1549327459 2019/02/05 00:44:19 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.102 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2017. */
/* GnollHack may be freely redistributed.  See license for details. */


#ifndef CONFIG_H
#include "config.h"
#endif

#if defined (DEBUG) && defined(GNH_CRTDBG)
#if defined (_DEBUG) && defined(WIN32)
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#include <stdlib.h>
#include <crtdbg.h>
#endif
#endif

#ifndef HACK_H
#define HACK_H

#include "lint.h"

#define TELL_LETHAL_STYLE 2
#define TELL 1
#define NOTELL 0
#define ON 1
#define OFF 0

/* Various ranges - BOLT_LIM has been split into these */
#define TELEPATHY_RANGE 8
#define FIND_RANGE 8
#define OPEN_RANGE 8
#define GAZE_RANGE 8
#define M_SHOOT_RANGE 18
#define M_SHOOT_CHANCE_RANGE 8
#define M_GENERIC_RANGED_ATTACK_RANGE 8
#define M_RAY_RANGE 8
#define M_BREATH_WEAPON_RANGE 8
#define M_LINEUP_RANGE 18
#define U_NOT_NEARBY_RANGE 8
#define U_THREAT_RANGE 18
#define NEARBY_CUTOFF_RANGE_CAN_SEE 9
#define NEARBY_CUTOFF_RANGE_CANNOT_SEE 5
#define EXPLOSION_SOUND_RADIUS 18
#define UNDEAD_TURNING_BASE_DISTANCE 6
#define SHOPKEEPER_NEARBY_CUTOFF_DISTANCE 4
#define MAXIMUM_RECOIL_DISTANCE 6

#define MAX_CARR_CAP 2000 /* so that boulders can be heavier */
#define POLEARM_MIN_DISTANCE 4 /* squared distance */
#define POLEARM_NORMAL_MAX_DISTANCE 8
#define POLEARM_EXTENDED_MAX_DISTANCE 13
#define SPEAR_MIN_DISTANCE 1
#define SPEAR_UNSKILLED_MAX_DISTANCE 2
#define SPEAR_BASIC_MAX_DISTANCE 4
#define SPEAR_SKILLED_MAX_DISTANCE 5
#define SPEAR_EXPERT_MAX_DISTANCE 6
#define SPEAR_MASTER_MAX_DISTANCE 7
#define SPEAR_GRAND_MASTER_MAX_DISTANCE 8
#define LANCE_MIN_DISTANCE 4
#define LANCE_UNSKILLED_MAX_DISTANCE 4
#define LANCE_BASIC_MAX_DISTANCE 4
#define LANCE_SKILLED_MAX_DISTANCE 5
#define LANCE_EXPERT_MAX_DISTANCE 6
#define LANCE_MASTER_MAX_DISTANCE 7
#define LANCE_GRAND_MASTER_MAX_DISTANCE 8

#define DUMMY \
    {         \
        0     \
    }




/* symbolic names for capacity levels */
enum encumbrance_types {
    UNENCUMBERED = 0,
    SLT_ENCUMBER = 1, /* Burdened */
    MOD_ENCUMBER = 2, /* Stressed */
    HVY_ENCUMBER = 3, /* Strained */
    EXT_ENCUMBER = 4, /* Overtaxed */
    OVERLOADED   = 5  /* Overloaded */
};

/* types of functions within verbose_wrapper */
enum verbose_function_types {
    VERBOSE_FUNCTION_UPDATE_MON_STATISTICS = 0,
    VERBOSE_FUNCTION_SET_MON_PROPERTY = 1
};


/* Muteness */
#define muteshk(shkp)                       \
    (!mon_can_move(shkp) || (shkp)->data->msound <= MS_ANIMAL)

/* weight increment of heavy iron ball */
#define IRON_BALL_W_INCR 160

/* number of turns it takes for vault guard to show up */
#define VAULT_GUARD_TIME 30

#define SHOP_DOOR_COST 400L /* cost of a destroyed shop door */
#define SHOP_BARS_COST 300L /* cost of iron bars */
#define SHOP_HOLE_COST 200L /* cost of making hole/trapdoor */
#define SHOP_WALL_COST 200L /* cost of destroying a wall */
#define SHOP_WALL_DMG  (10L * ACURRSTR) /* damaging a wall */

/* hunger states - see hu_stat in eat.c */
enum hunger_state_types {
    SATIATED   = 0,
    NOT_HUNGRY = 1,
    HUNGRY     = 2,
    WEAK       = 3,
    FAINTING   = 4,
    FAINTED    = 5,
    STARVED    = 6
};

/* Macros for how a rumor was delivered in outrumor() */
#define BY_ORACLE 0
#define BY_COOKIE 1
#define BY_PAPER 2
#define BY_SPELL 3
#define BY_OTHER 9

/* Macros for why you are no longer riding */
enum dismount_types {
    DISMOUNT_GENERIC  = 0,
    DISMOUNT_FELL     = 1,
    DISMOUNT_THROWN   = 2,
    DISMOUNT_POLY     = 3,
    DISMOUNT_ENGULFED = 4,
    DISMOUNT_BONES    = 5,
    DISMOUNT_BYCHOICE = 6
};

/* Special returns from mapglyph() */
#define MG_CORPSE   0x00000001UL
#define MG_INVIS    0x00000002UL
#define MG_DETECT   0x00000004UL
#define MG_PET      0x00000008UL
#define MG_RIDDEN   0x00000010UL
#define MG_STATUE   0x00000020UL
#define MG_OBJPILE  0x00000040UL  /* more than one stack of objects */
#define MG_BW_LAVA  0x00000080UL  /* 'black & white lava': highlight lava if it
                            can't be distringuished from water by color */
#define MG_PEACEFUL 0x00000100UL
#define MG_SADDLED  0x00000200UL
#define MG_FEMALE   0x00000400UL
#define MG_H_FLIP   0x00000800UL /* Flipped horizontally */
#define MG_STONE_INVERSE    0x00001000UL /* ' ' is inversed if it has color */
#define MG_ALT_COLORS       0x00002000UL /* Uses alternative colors */
#define MG_DECORATION       0x00004000UL
#define MG_CARPET           0x00008000UL


/* sellobj_state() states */
#define SELL_NORMAL (0)
#define SELL_DELIBERATE (1)
#define SELL_DONTSELL (2)

/* bitmask flags for corpse_xname();
   PFX_THE takes precedence over ARTICLE, NO_PFX takes precedence over both */
#define CXN_NORMAL 0    /* no special handling */
#define CXN_SINGULAR 1  /* override quantity if greather than 1 */
#define CXN_NO_PFX 2    /* suppress "the" from "the Unique Monst */
#define CXN_PFX_THE 4   /* prefix with "the " (unless pname) */
#define CXN_ARTICLE 8   /* include a/an/the prefix */
#define CXN_NOCORPSE 16 /* suppress " corpse" suffix */

#define KXNFLAGS_NO_ARTICLE 0x01
#define KXNFLAGS_SPELL      0x02  // Not the book, but the spell from it

   /* getpos() return values */
enum getpos_retval {
    LOOK_TRADITIONAL = 0, /* '.' -- ask about "more info?" */
    LOOK_QUICK       = 1, /* ',' -- skip "more info?" */
    LOOK_ONCE        = 2, /* ';' -- skip and stop looping */
    LOOK_VERBOSE     = 3  /* ':' -- show more info w/o asking */
};

/*
 * This is the way the game ends.  If these are rearranged, the arrays
 * in end.c and topten.c will need to be changed.  Some parts of the
 * code assume that PANIC separates the deaths from the non-deaths.
 */
enum game_end_types {
    DIED         =  0,
    CHOKING      =  1,
    POISONING    =  2,
    STARVING     =  3,
    DROWNING     =  4,
    DROWNED      =  5,
    BURNING      =  6,
    DISSOLVED    =  7,
    CRUSHING     =  8,
    STRANGULATION=  9,
    SUFFOCATION  = 10,
    STONING      = 11,
    DISINTEGRATION=12,
    TURNED_SLIME = 13,
    ILLNESS      = 14,
    ROTTED       = 15,
    GENOCIDED    = 16,
    PANICKED     = 17,
    TRICKED      = 18,
    QUIT         = 19,
    ESCAPED      = 20,
    ASCENDED     = 21,
    NUM_GAME_END_TYPES = 22
};

/* game events log */
struct gamelog_line {
    long turn; /* turn when this happened */
    long flags; /* LL_foo flags */
    char* text;
    struct gamelog_line* next;
};

typedef struct strbuf {
    size_t len;
    char * str;
    char   buf[256];
} strbuf_t;

#include "align.h"
#include "dungeon.h"
#include "monsym.h"
#include "mkroom.h"
#include "npc.h"
#include "objclass.h"
#include "youprop.h"
#include "wintype.h"
#include "context.h"
#include "decl.h"
#include "timeout.h"
#include "general.h"


/* attack mode for hmon() */
enum hmon_atkmode_types {
    HMON_MELEE   = 0, /* hand-to-hand */
    HMON_THROWN  = 1, /* normal ranged (or spitting while poly'd) */
    HMON_KICKED  = 2, /* alternate ranged */
    HMON_APPLIED = 3, /* polearm, treated as ranged */
    HMON_DRAGGED = 4, /* attached iron ball, pulled into mon */
    HMON_GOLF    = 5  /* alternate ranged */
};

/* sortloot() return type; needed before extern.h */
struct sortloot_item {
    struct obj *obj;
    char *str; /* result of loot_xname(obj) in some cases, otherwise null */
    int indx; /* signed int, because sortloot()'s qsort comparison routine
                 assumes (a->indx - b->indx) might yield a negative result */
    xchar orderclass; /* order rather than object class; 0 => not yet init'd */
    xchar subclass; /* subclass for some classes */
    xchar disco; /* discovery status */
};
typedef struct sortloot_item Loot;

#define MATCH_WARN_OF_MON(mon)                                               \
    (Warn_of_mon && ((context.warntype.obj                                   \
                      && (context.warntype.obj & (mon)->data->mflags2))      \
                     || (context.warntype.polyd                              \
                         && (context.warntype.polyd & (mon)->data->mflags2)) \
                     || (context.warntype.species                            \
                         && (context.warntype.species == (mon)->data))))

/*
#define MATCH_WARN_UNDEAD(mon)                                               \
    (Undead_warning && (mon->data->mflags2 & M2_UNDEAD)) 

#define MATCH_WARN_DEMON(mon)                                               \
    (Demon_warning && (mon->data->mflags2 & M2_DEMON)) 

#define MATCH_WARN_ORC(mon)                                               \
    (Orc_warning && (mon->data->mflags2 & M2_ORC)) 
*/

#include "animation.h"
extern int animation_offsets[MAX_ANIMATIONS];
extern int enlargement_offsets[MAX_ENLARGEMENTS];
extern int replacement_offsets[MAX_REPLACEMENTS];

#include "trap.h"
#include "flag.h"
#include "rm.h"
#include "vision.h"
#include "display.h"
#include "engrave.h"
#include "rect.h"
#include "region.h"
#include "soundset.h"
#include "matcomps.h" /* before extern.h */
#include "artifact.h"

/* Symbol offsets */
#define SYM_OFF_P (0)
#define SYM_OFF_O (SYM_OFF_P + MAX_CMAPPED_CHARS)
#define SYM_OFF_M (SYM_OFF_O + MAX_OBJECT_CLASSES)
#define SYM_OFF_W (SYM_OFF_M + MAX_MONSTER_CLASSES)
#define SYM_OFF_X (SYM_OFF_W + WARNCOUNT)
#define SYM_MAX (SYM_OFF_X + MAXOTHER)


/* Tiles */
#include "tiledata.h"
#ifdef USE_TILES
extern int glyph2tile[MAX_GLYPH];
extern uchar glyphtileflags[MAX_GLYPH];
extern short glyph2replacement[MAX_GLYPH];
extern short tile2autodraw[MAX_TILES];
//extern short tile2replacement[MAX_TILES];
extern short tile2animation[MAX_TILES];
extern short tile2enlargement[MAX_TILES];
#endif


#ifdef USE_TRAMPOLI /* this doesn't belong here, but we have little choice */
#undef NDECL
#define NDECL(f) f()
#endif

#include "extern.h"
#include "winprocs.h"
#include "sys.h"

#ifdef USE_TRAMPOLI
#include "wintty.h"
#undef WINTTY_H
#include "trampoli.h"
#undef EXTERN_H
#include "extern.h"
#endif /* USE_TRAMPOLI */

/* flags to control makemon() */
#define NO_MM_FLAGS                     0x00000000UL /* use this rather than plain 0 */
#define MM_NO_MONSTER_INVENTORY         0x00000001UL /* suppress minvent when creating mon */
#define MM_NOWAIT                       0x00000002UL /* don't set STRAT_WAITMASK flags */
#define MM_NOCOUNTBIRTH                 0x00000004UL /* don't increment born counter (for revival) */
#define MM_IGNOREWATER                  0x00000008UL /* ignore water when positioning */
#define MM_ADJACENTOK                   0x00000010UL /* it is acceptable to use adjacent coordinates */
#define MM_ANGRY                        0x00000020UL /* monster is created angry */
#define MM_NONAME                       0x00000040UL /* monster is not christened */
#define MM_WAITFORU                     0x00000080UL /* set WAITFORU strategy for monster */
#define MM_EGD                          0x00000100UL /* add egd structure */
#define MM_EPRI                         0x00000200UL /* add epri structure */
#define MM_ESMI                         0x00000400UL /* add esmi structure */
#define MM_ENPC                         0x00000800UL /* add enpc structure */
#define MM_ESHK                         0x00001000UL /* add eshk structure */
#define MM_EMIN                         0x00002000UL /* add emin structure */
#define MM_EDOG                         0x00004000UL /* add edog structure */
#define MM_ASLEEP                       0x00008000UL /* monsters should be generated asleep */
#define MM_NOGRP                        0x00010000UL /* suppress creation of monster groups */
#define MM_MAX_HP                       0x00020000UL /* maximum hit points */
#define MM_MALE                         0x00040000UL /* always male */
#define MM_FEMALE                       0x00080000UL /* always female */
#define MM_NORMAL_HIT_DICE              0x00100000UL /* use normal hit dice */
#define MM_EMIN_COALIGNED               0x00200000UL /* minion is of the same alignment as player */
#define MM_SET_ORIGIN_COORDINATES       0x00400000UL /* origin coordinates are set to x, y */
#define MM_SADDLED                      0x00800000UL
#define MM_PLAY_SUMMON_ANIMATION        0x01000000UL
#define MM_SUMMON_MONSTER_ANIMATION     0x00000000UL
#define MM_CHAOTIC_SUMMON_ANIMATION     0x02000000UL
#define MM_LAWFUL_SUMMON_ANIMATION      0x04000000UL
#define MM_NEUTRAL_SUMMON_ANIMATION     (MM_CHAOTIC_SUMMON_ANIMATION | MM_LAWFUL_SUMMON_ANIMATION)
#define MM_SPECIAL_SUMMON_ANIMATION     0x08000000UL
#define MM_SUMMON_IN_SMOKE_ANIMATION    (MM_SPECIAL_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION)
#define MM_UNDEAD_SUMMON_ANIMATION      (MM_SPECIAL_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION)
#define MM_ANIMATE_DEAD_ANIMATION       (MM_SPECIAL_SUMMON_ANIMATION | MM_LAWFUL_SUMMON_ANIMATION)
#define MM_SUMMON_NASTY_ANIMATION       (MM_SPECIAL_SUMMON_ANIMATION | MM_LAWFUL_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION)
#define MM_SUMMON_ANIMATION_TYPE_MASK   (MM_SPECIAL_SUMMON_ANIMATION | MM_LAWFUL_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION)

#define MM_PLAY_SUMMON_SOUND            0x10000000UL
#define MM_ANIMATION_WAIT_UNTIL_END     0x20000000UL
#define MM_PEACEFUL                     0x40000000UL
#define MM_ROAMER                       0x80000000UL

#define NO_MM2_FLAGS                    0x00000000UL /* use this rather than plain 0 */
#define MM2_FACING_LEFT                 0x00000001UL
#define MM2_FACING_RIGHT                0x00000002UL
#define MM2_FORCE_RENEGADE              0x00000004UL
#define MM2_FORCE_NONRENEGADE           0x00000008UL
#define MM2_MAYBE_ALLOW_EXTINCT         0x00000010UL
#define MM2_REVIVING                    0x00000020UL
#define MM2_RANDOMIZE_SUBTYPE           0x00000040UL

#define NO_MKCLASS_FLAGS                0x00000000UL /* use this rather than plain 0 */
#define MKCLASS_FLAGS_IGNORE_DIFFICULTY 0x00000001UL /* ignore difficulty restrictions */
#define MKCLASS_FLAGS_SUMMON            0x00000002UL /* is a summoning for mk_gen_ok */
#define MKCLASS_FLAGS_POLYMORPH         0x00000004UL /* is a polymorph for mk_gen_ok */

#define GOODPOS_IGNOREYOU               0x80000000UL

/* flags for make_corpse() and mkcorpstat() */
#define CORPSTAT_NONE 0x00
#define CORPSTAT_INIT 0x01   /* pass init flag to mkcorpstat */
#define CORPSTAT_BURIED 0x02 /* bury the corpse or statue */

/* flags for decide_to_shift() */
#define SHIFT_SEENMSG 0x01 /* put out a message if in sight */
#define SHIFT_MSG 0x02     /* always put out a message */

/* flags for deliver_obj_to_mon */
#define DF_NONE     0x00
#define DF_RANDOM   0x01
#define DF_ALL      0x04

/* special mhpmax value when loading bones monster to flag as extinct or
 * genocided */
#define DEFUNCT_MONSTER (-100)

/* macro form of adjustments of physical damage based on Half_physical_damage.
 * Can be used on-the-fly with the 1st parameter to losehp() if you don't
 * need to retain the dmg value beyond that call scope.
 * Take care to ensure it doesn't get used more than once in other instances.
 */
#define Maybe_Half_Phys(dmg) \
    ((Half_physical_damage) ? (((dmg) + 1) / 2) : (dmg))

#define Maybe_Half_Phys_Magr(dmg, magr) \
    ((Half_physical_damage|| (Half_physical_damage_against_undead_and_demons \
        && (is_undead(magr->data) || is_demon(magr->data) || is_vampshifter(magr)))) \
    ? (((dmg) + 1) / 2) : (dmg))


 /* (mon->mx == 0) implies migrating */
#define mon_is_local_mx(mon) ((mon)->mx > 0)

/* flags for special ggetobj status returns */
#define ALL_FINISHED 0x01 /* called routine already finished the job */

/* flags to control query_objlist() */
#define BY_NEXTHERE 0x1       /* follow objlist by nexthere field */
#define AUTOSELECT_SINGLE 0x2 /* if only 1 object, don't ask */
#define USE_INVLET 0x4        /* use object's invlet */
#define INVORDER_SORT 0x8     /* sort objects by packorder */
#define SIGNAL_NOMENU 0x10    /* return -1 rather than 0 if none allowed */
#define SIGNAL_ESCAPE 0x20    /* return -2 rather than 0 for ESC */
#define FEEL_COCKATRICE 0x40  /* engage cockatrice checks and react */
#define INCLUDE_HERO 0x80     /* show hero among engulfer's inventory */

/* Flags to control query_category() */
/* BY_NEXTHERE used by query_category() too, so skip 0x01 */
#define UNPAID_TYPES 0x02
#define GOLD_TYPES 0x04
#define WORN_TYPES 0x08
#define ALL_TYPES 0x10
#define BILLED_TYPES 0x20
#define CHOOSE_ALL 0x40
#define BUC_BLESSED 0x80
#define BUC_CURSED 0x100
#define BUC_UNCURSED 0x200
#define BUC_UNKNOWN 0x400
#define BUC_ALLBKNOWN (BUC_BLESSED | BUC_CURSED | BUC_UNCURSED)
#define BUCX_TYPES (BUC_ALLBKNOWN | BUC_UNKNOWN)
#define UNIDENTIFIED_TYPES 0x800
#define UNKNOWN_TYPES 0x1000
#define ALL_TYPES_SELECTED -2

/* Flags to control find_mid() */
#define FM_FMON 0x01    /* search the fmon chain */
#define FM_MIGRATE 0x02 /* search the migrating monster chain */
#define FM_MYDOGS 0x04  /* search mydogs */
#define FM_EVERYWHERE (FM_FMON | FM_MIGRATE | FM_MYDOGS)

/* Flags to control pick_[race,role,gend,align] routines in role.c */
#define PICK_RANDOM 0
#define PICK_RIGID 1

/* Flags to control dotrap() in trap.c */
#define FORCETRAP 0x01     /* triggering not left to chance */
#define NOWEBMSG 0x02      /* suppress stumble into web message */
#define FORCEBUNGLE 0x04   /* adjustments appropriate for bungling */
#define RECURSIVETRAP 0x08 /* trap changed into another type this same turn */
#define TOOKPLUNGE 0x10    /* used '>' to enter pit below you */
#define VIASITTING 0x20    /* #sit while at trap location (affects message) */
#define FAILEDUNTRAP 0x40  /* trap activated by failed untrap attempt */

/* Flags to control test_move in hack.c */
#define DO_MOVE 0   /* really doing the move */
#define TEST_MOVE 1 /* test a normal move (move there next) */
#define TEST_TRAV 2 /* test a future travel location */
#define TEST_TRAP 3 /* check if a future travel loc is a trap */

/* Flags to control adjust_damage */
#define ADFLAGS_NONE            0x00000000UL
#define ADFLAGS_SPELL_DAMAGE    0x00000001UL
#define ADFLAGS_MELEE_DAMAGE    0x00000002UL

/*** some utility macros ***/
#define yn_query(query) yn_function(query, ynchars, 'n', yndescs)
#define ynq(query) yn_function(query, ynqchars, 'q', ynqdescs)
#define ynq2(query) yn_function(query, ynqchars, 'q', ynq2descs)
#define ynaq(query) yn_function(query, ynaqchars, 'y', ynaqdescs)
#define nyaq(query) yn_function(query, ynaqchars, 'n', ynaqdescs)
#define nyNaq(query) yn_function(query, ynNaqchars, 'n', ynNaqdescs)
#define ynNaq(query) yn_function(query, ynNaqchars, 'y', ynNaqdescs)
#define dnq(query) yn_function(query, dnqchars, 'q', dnqdescs)
#define idq(query) yn_function(query, idqchars, 'q', idqdescs)

#define yn_query_ex(a, c, title, query) yn_function_ex(YN_STYLE_GENERAL, a, c, NO_GLYPH, title, query, ynchars, 'n', yndescs, (const char*)0, 0UL)
#define ynq_ex(a, c, title, query) yn_function_ex(YN_STYLE_GENERAL, a, c, NO_GLYPH, title, query, ynqchars, 'q', ynqdescs, (const char*)0, 0UL)
#define yn_function_es(s, a, c, title, query, chars, resp, descs, introline) yn_function_ex(s, a, c, NO_GLYPH, title, query, chars, resp, descs, introline, 0UL)

/* Macros for scatter */
#define VIS_EFFECTS 0x01 /* display visual effects */
#define MAY_HITMON 0x02  /* objects may hit monsters */
#define MAY_HITYOU 0x04  /* objects may hit you */
#define MAY_HIT (MAY_HITMON | MAY_HITYOU)
#define MAY_DESTROY 0x08  /* objects may be destroyed at random */
#define MAY_FRACTURE 0x10 /* boulders & statues may fracture */
#define ADD_CAUGHT_IN_LEAVES_FLAG 0x20 /* if obj stays at x,y, add a speflag SPEFLAGS_CAUGHT_IN_LEAVES */

/* Macros for launching objects */
#define ROLL 0x01          /* the object is rolling */
#define FLING 0x02         /* the object is flying thru the air */
#define LAUNCH_UNSEEN 0x40 /* hero neither caused nor saw it */
#define LAUNCH_KNOWN 0x80  /* the hero caused this by explicit action */

/* enlightenment control flags */
#define BASICENLIGHTENMENT 1 /* show mundane stuff */
#define MAGICENLIGHTENMENT 2 /* show intrinsics and such */
#define ENL_GAMEINPROGRESS 0
#define ENL_GAMEOVERALIVE  1 /* ascension, escape, quit, trickery */
#define ENL_GAMEOVERDEAD   2

/* control flags for sortloot() */
#define SORTLOOT_PACK   0x01
#define SORTLOOT_INVLET 0x02
#define SORTLOOT_LOOT   0x04
#define SORTLOOT_PETRIFY 0x20 /* override filter func for c-trice corpses */

/* flags for xkilled() [note: meaning of first bit used to be reversed,
   1 to give message and 0 to suppress] */
#define XKILL_GIVEMSG   0
#define XKILL_NOMSG     1
#define XKILL_NOCORPSE  2
#define XKILL_NOCONDUCT 4
#define XKILL_DROPDEAD  8

/* pline_flags; mask values for custompline()'s first argument */
/* #define PLINE_ORDINARY 0 */
#define PLINE_NOREPEAT   1
#define OVERRIDE_MSGTYPE 2
#define SUPPRESS_HISTORY 4
#define URGENT_MESSAGE   8
#define STAY_ON_LINE    16

/* Macros for messages referring to hands, eyes, feet, etc... */
enum bodypart_types {
    ARM       =  0,
    EYE       =  1,
    FACE      =  2,
    FINGER    =  3,
    FINGERTIP =  4,
    FOOT      =  5,
    HAND      =  6,
    HANDED    =  7,
    HEAD      =  8,
    LEG       =  9,
    LIGHT_HEADED = 10,
    NECK      = 11,
    SPINE     = 12,
    TOE       = 13,
    HAIR      = 14,
    BLOOD     = 15,
    LUNG      = 16,
    NOSE      = 17,
    STOMACH   = 18,
    WRIST     = 19
};

/* indices for some special tin types */
#define ROTTEN_TIN 0
#define HOMEMADE_TIN 1
#define SPINACH_TIN (-1)
#define RANDOM_TIN (-2)
#define HEALTHY_TIN (-3)

/* these match the categorizations shown by enlightenment */
#define ALGN_SINNED (-4) /* worse than strayed (-1..-3) */
#define ALGN_PIOUS 14    /* better than fervent (9..13) */

/* Some misc definitions */
#define POTION_OCCUPANT_CHANCE(n) (13 + 2 * (n))
#define WAND_BACKFIRE_CHANCE 100
#define BALL_IN_MON (u.uswallow && uball && uball->where == OBJ_FREE)
#define NODIAG(monnum) ((monnum) == PM_GRID_BUG)

/* Flags to control menus */
#define MENUTYPELEN sizeof("traditional ")
#define MENU_TRADITIONAL 0
#define MENU_COMBINATION 1
#define MENU_FULL 2
#define MENU_PARTIAL 3

#define MENU_SELECTED TRUE
#define MENU_UNSELECTED FALSE

/*
 * Option flags
 * Each higher number includes the characteristics of the numbers
 * below it.
 */
/* XXX This should be replaced with a bitmap. */
#define SET_IN_SYS 0   /* system config file option only */
#define SET_IN_FILE 1  /* config file option only */
#define SET_VIA_PROG 2 /* may be set via extern program, not seen in game */
#define DISP_IN_GAME 3 /* may be set via extern program, displayed in game \
                          */
#define SET_IN_GAME 4  /* may be set via extern program or set in the game */
#define SET_IN_WIZGAME 5  /* may be set set in the game if wizmode */
#define SET__IS_VALUE_VALID(s) ((s < SET_IN_SYS) || (s > SET_IN_WIZGAME))

#define FEATURE_NOTICE_VER(major, minor, patch)                    \
    (((unsigned long) major << 24) | ((unsigned long) minor << 16) \
     | ((unsigned long) patch << 8) | ((unsigned long) 0))

#define FEATURE_NOTICE_VER_MAJ (flags.suppress_alert >> 24)
#define FEATURE_NOTICE_VER_MIN \
    (((unsigned long) (0x0000000000FF0000L & flags.suppress_alert)) >> 16)
#define FEATURE_NOTICE_VER_PATCH \
    (((unsigned long) (0x000000000000FF00L & flags.suppress_alert)) >> 8)

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif
#define plur(x) (((x) == 1) ? "" : "s")

#define ARM_AC_BONUS(obj, ptr) \
    ((int)(((obj)->oclass == ARMOR_CLASS || (obj)->oclass == MISCELLANEOUS_CLASS || (objects[(obj)->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED) || has_obj_mythic_defense(obj)) ? \
    (objects[(obj)->otyp].oc_armor_class + get_obj_material_and_exceptionality_ac_bonus(obj) + (!(objects[(obj)->otyp].oc_flags & O1_ENCHANTMENT_DOES_NOT_AFFECT_AC) || has_obj_mythic_defense(obj) ? (!cursed_items_are_positive(ptr) ? (obj)->enchantment : abs((obj)->enchantment)) : 0) \
     - ((objects[(obj)->otyp].oc_flags & O1_EROSION_DOES_NOT_AFFECT_AC) ? 0 : min((int) greatest_erosion(obj), objects[(obj)->otyp].oc_armor_class))) : 0))

#define ARM_MC_BONUS(obj, ptr) \
    ((int)(((obj)->oclass == ARMOR_CLASS || (obj)->oclass == MISCELLANEOUS_CLASS || (objects[(obj)->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED) || has_obj_mythic_defense(obj)) ? \
    (objects[(obj)->otyp].oc_magic_cancellation + get_obj_material_and_exceptionality_mc_bonus(obj) + ((!(objects[(obj)->otyp].oc_flags & O1_ENCHANTMENT_DOES_NOT_AFFECT_MC) || has_obj_mythic_defense(obj)) ? (!cursed_items_are_positive(ptr) ? (obj)->enchantment : abs((obj)->enchantment)) / 3 : 0) \
     - ((objects[(obj)->otyp].oc_flags & O1_EROSION_DOES_NOT_AFFECT_MC) ? 0 : min(greatest_erosion(obj) / 2, objects[(obj)->otyp].oc_magic_cancellation))) : 0))

#define makeknown(x) discover_object((x), TRUE, TRUE)
#define distu(xx, yy) dist2((int)(xx), (int)(yy), (int) u.ux, (int) u.uy)
#define onlineu(xx, yy) online2((int)(xx), (int)(yy), (int) u.ux, (int) u.uy)

#define rn1(x, y) (rn2(x) + (y))

/* negative armor class is randomly weakened to prevent invulnerability */
#define AC_VALUE(AC) ((AC) >= 0 ? (AC) : (AC) >= -21 ? -rnd(-(AC)) : (AC) >= -41 ? -rnd(21 + (-(AC) - 21) / 2) : -rnd(31 + (-(AC) - 41) / 3))

/* Object pile definition */
#define is_objpile(x,y) (!Hallucination && level.objects[(x)][(y)] \
                         && level.objects[(x)][(y)]->nexthere)

#define loc_blocks_flying_and_leviation(x, y) ((IS_ROCK(levl[x][y].typ) || closed_door(x, y) || (Is_waterlevel(&u.uz) && levl[x][y].typ == WATER)))


#if defined(MICRO) && !defined(__DJGPP__)
#define getuid() 1
#define getlogin() ((char *) 0)
#endif /* MICRO */

#if defined(OVERLAY)
#define USE_OVLx
#define STATIC_DCL extern
#define STATIC_OVL
#define STATIC_VAR

#else /* !OVERLAY */

#define STATIC_DCL static
#define STATIC_OVL static
#define STATIC_VAR static

#endif /* OVERLAY */

/* Macro for a few items that are only static if we're not overlaid.... */
#if defined(USE_TRAMPOLI) || defined(USE_OVLx)
#define STATIC_PTR
#else
#define STATIC_PTR static
#endif

/* The function argument to qsort() requires a particular
 * calling convention under WINCE which is not the default
 * in that environment.
 */
#if defined(WIN_CE)
#define CFDECLSPEC __cdecl
#else
#define CFDECLSPEC
#endif

#define DEVTEAM_EMAIL "gnollhack@hyvanmielenpelit.fi"
#define DEVTEAM_URL "http://www.gnollhack.com"

#define ENCOUNTER_ONE_IN_CHANCE 5
#include "encounter.h"

static const char empty_string[] = "";

/* Game play constants */
#define TO_HIT_LEVEL_MULTIPLIER 0.25
#define BOW_TO_HIT_MELEE_PENALTY 40
#define THROWN_WEAPON_TO_HIT_MELEE_PENALTY 40
#define THROWING_WEAPON_TO_HIT_MELEE_PENALTY_WHEN_USED_AS_MELEE_WEAPON 20

#define SHARPNESS_PERCENTAGE_CHANCE 35
#define SHARPNESS_DIE_ROLL_CHANCE (SHARPNESS_PERCENTAGE_CHANCE / 5)
#define SHARPNESS_MAX_HP_PERCENTAGE_DAMAGE 15

#define CORPSE_ROTTING_SPEED 35L            /* Turns per one rotting level */
#define CORPSE_ROTTING_SPEED_VARIATION 10L  /* Turns per one rotting level */
#define ROTTING_THRESHOLD 40L               /* Turns before can rot */
#define BLESSED_ROTTING_THRESHOLD 80L       /* Turns before can rot */


/* Some tile display properties */
#define MIN_SCREEN_SCALE_ADJUSTMENT -0.90
#define MAX_SCREEN_SCALE_ADJUSTMENT 3.0
#define KEYBOARD_SCREEN_SCALE_ADJUSTMENT_STEP 0.25
#define MOUSE_SCREEN_SCALE_ADJUSTMENT_STEP 0.05
#define MIN_PREF_SCREEN_SCALE 10
#define MAX_PREF_SCREEN_SCALE 400
#define SPECIAL_HEIGHT_IN_PIT -32
#define SPECIAL_HEIGHT_LEVITATION 16
#define PIT_BOTTOM_BORDER 2
#define OBJECT_PILE_START_HEIGHT 2
#define OBJECT_PILE_HEIGHT_DIFFERENCE 2
#define OBJECT_CAUGHT_IN_LEAVES_HEIGHT (TILE_Y / 2)
#define OBJECT_PIT_SCALING_FACTOR 0.75

#define ANIMATION_FRAME_INTERVAL 25 /* in milliseconds */
#define DELAY_OUTPUT_INTERVAL_IN_ANIMATION_INTERVALS 2 /* 2 animation intervals, or 50 milliseconds */
#define DEFAULT_MOVE_INTERVAL (ANIMATION_FRAME_INTERVAL * 2) /* in milliseconds */
#define DEFAULT_CRAWL_INTERVAL (ANIMATION_FRAME_INTERVAL * 5) /* in milliseconds */
#define ITEM_PICKUP_DROP_DELAY 75 /* in milliseconds */

#define NO_WALL_END_AUTODRAW(x,y) (!isok(x, y) || glyph_is_specific_cmap_or_its_variation(levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FLOOR], S_unexplored) || (IS_DOORJOIN(levl[x][y].typ) && !IS_TREE(levl[x][y].typ)) || levl[x][y].typ == DOOR || levl[x][y].typ == UNDEFINED_LOCATION /*|| (levl[x][y].seenv & (SV4 | SV5 | SV6)) == 0 */)
//#define NO_WALL_END_AUTODRAW(x,y) no_wall_end_autodraw(x, y)

/* Saved initial object data in o_init.c */
extern struct objdescr saved_obj_descr[NUM_OBJECTS];
extern struct objclass saved_objects[NUM_OBJECTS];

/* Some general definitions for graphics and sounds */
#include "layer.h"
#include "soundset.h"

#define empty_layerinfo(empty_symbol) \
    { base_cmap_to_glyph(empty_symbol),\
        NO_GLYPH,\
        { base_cmap_to_glyph(empty_symbol),NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,\
          NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,\
          NO_GLYPH,NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH \
        }, \
        { base_cmap_to_glyph(empty_symbol),NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,\
          NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,NO_GLYPH,\
          NO_GLYPH,NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH \
        }, \
        0UL, 0U, 0U, (struct obj*)0,   0, 0,   \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0 }, 0, 0, 0UL,   0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0UL, 0, 0, 0, \
        {0, 0, 0}, {0, 0, 0} \
    }

#define nul_layerinfo empty_layerinfo(S_unexplored)

/* convert 1..10 to 0..9; add 10 for second group (spell casting) */
#define ad_to_typ(k) (10 + (int) k - 1)


/* some math */
#define absd(x) (x > 0.0 ? x : -x)

#define Elbereth_word "Elbereth"
#define Gilthoniel_word "Gilthoniel"
#define Morgoth_word "Morgoth"

#if defined(BSD) || defined(ULTRIX)
#define readLenType int
#else /* e.g. SYSV, __TURBOC__ */
#define readLenType unsigned
#endif

#if defined(__BEOS__) || defined(MICRO) || defined(OS2) || defined(ANDROID) || defined(GNH_MOBILE) || defined(WIN32)
extern void FDECL(gnollhack_exit, (int)) NORETURN;
#else
#define gnollhack_exit exit
#endif

#endif /* HACK_H */
