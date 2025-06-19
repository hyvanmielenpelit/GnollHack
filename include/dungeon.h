/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    dungeon.h    $NHDT-Date: 1447755969 2015/11/17 10:26:09 $  $NHDT-Branch: master $:$NHDT-Revision: 1.24 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef DUNGEON_H
#define DUNGEON_H

#include "align.h"
#include "config.h"

typedef struct d_flags {     /* dungeon/level type flags */
    uchar tileset;
    uchar has_tileset;
    uchar town;       /* is this a town? (levels only) */
    uchar hellish;    /* is this part of hell? */
    uchar maze_like;  /* is this a maze? */
    uchar rogue_like; /* is this an old-fashioned presentation? */
    uchar align;      /* dungeon alignment. */
    uchar unused;     /* etc... */
} d_flags;

typedef struct d_level { /* basic dungeon level element */
    xchar dnum;          /* dungeon number */
    xchar dlevel;        /* level number */
} d_level;

typedef struct s_level { /* special dungeon level element */
    struct s_level *next;
    d_level dlevel; /* dungeon & level numbers */
    char name[MAX_LVL_NAME_LENGTH]; /* Name displayed whenever relevant */
    char proto[MAX_PROTO_NAME_LENGTH]; /* name of prototype file (eg. "tower") */
    char boneid;    /* character to id level in bones files */
    uchar rndlevs;  /* no. of randomly available similar levels */
    d_flags flags;  /* type flags */
} s_level;

typedef struct stairway { /* basic stairway identifier */
    xchar sx, sy;         /* x / y location of the stair */
    d_level tolev;        /* where does it go */
    char up;              /* what type of stairway (up/down) */
} stairway;

/* level region types */
enum level_region_types {
    LR_DOWNSTAIR = 0,
    LR_UPSTAIR,
    LR_PORTAL,
    LR_BRANCH,
    LR_TELE,
    LR_UPTELE,
    LR_DOWNTELE,
    LR_SPECIAL_MAP_SEEN,
    LR_SPECIAL_MAP_NAME_REVEALED,
    LR_NOTELE,
};

typedef struct dest_area { /* non-stairway level change identifier */
    xchar lx, ly;          /* "lower" left corner (near [0,0]) */
    xchar hx, hy;          /* "upper" right corner (near [COLNO,ROWNO]) */
    xchar nlx, nly;        /* outline of invalid area */
    xchar nhx, nhy;        /* opposite corner of invalid area */
} dest_area;

typedef struct dungeon {   /* basic dungeon identifier */
    char dname[MAX_DGN_NAME_LENGTH];        /* name of the dungeon (eg. "Hell") */
    char proto[MAX_PROTO_NAME_LENGTH];        /* name of prototype file (eg. "tower") */
    char boneid;           /* character to id dungeon in bones files */
    d_flags flags;         /* dungeon flags */
    xchar entry_lev;       /* entry level */
    xchar num_dunlevs;     /* number of levels in this dungeon */
    xchar dunlev_ureached; /* how deep you have been in this dungeon */
    int ledger_start,      /* the starting depth in "real" terms */
        depth_start;       /* the starting depth in "logical" terms */
} dungeon;

/*
 * A branch structure defines the connection between two dungeons.  They
 * will be ordered by the dungeon number/level number of 'end1'.  Ties
 * are resolved by 'end2'.  'Type' uses 'end1' arbitrarily as the primary
 * point.
 */
typedef struct branch {
    struct branch *next; /* next in the branch chain */
    int id;              /* branch identifier */
    int type;            /* type of branch */
    d_level end1;        /* "primary" end point */
    d_level end2;        /* other end point */
    boolean end1_up;     /* does end1 go up? */
} branch;

/* branch types */
#define BR_STAIR 0   /* "Regular" connection, 2 staircases. */
#define BR_NO_END1 1 /* "Regular" connection.  However, no stair from
                        end1 to end2.  There is a stair from end2 to end1. */
#define BR_NO_END2 2 /* "Regular" connection.  However, no stair from
                        end2 to end1.  There is a stair from end1 to end2. */
#define BR_PORTAL 3  /* Connection by magic portals (traps) */

/* A particular dungeon contains num_dunlevs d_levels with dlevel 1..
 * num_dunlevs.  Ledger_start and depth_start are bases that are added
 * to the dlevel of a particular d_level to get the effective ledger_no
 * and depth for that d_level.
 *
 * Ledger_no is a bookkeeping number that gives a unique identifier for a
 * particular d_level (for level.?? files, e.g.).
 *
 * Depth corresponds to the number of floors below the surface.
 */
#define Is_astralevel(x) (on_level(x, &astral_level))
#define Is_earthlevel(x) (on_level(x, &earth_level))
#define Is_waterlevel(x) (on_level(x, &water_level))
#define Is_firelevel(x) (on_level(x, &fire_level))
#define Is_airlevel(x) (on_level(x, &air_level))
#define Is_medusa_level(x) (on_level(x, &medusa_level))
#define Is_oracle_level(x) (on_level(x, &oracle_level))
#define Is_valley(x) (on_level(x, &valley_level))
#define Is_jubilex_level(x) (on_level(x, &jubilex_level))
#define Is_orcus_level(x) (on_level(x, &orcus_level))
#define Is_dispater_level(x) (on_level(x, &dispater_level))
#define Is_baal_level(x) (on_level(x, &baalzebub_level))
#define Is_minotaur_level(x) (on_level(x, &minotaur_level))
#define Is_yeenaghu_level(x) (on_level(x, &yeenaghu_level))
#define Is_tarrasque_level(x) (on_level(x, &tarrasque_level))
#define Is_wiz1_level(x) (on_level(x, &wiz1_level))
#define Is_wiz2_level(x) (on_level(x, &wiz2_level))
#define Is_wiz3_level(x) (on_level(x, &wiz3_level))
#define Is_sanctum(x) (on_level(x, &sanctum_level))
#define Is_portal_level(x) (on_level(x, &portal_level))
#define Is_rogue_level(x) (on_level(x, &rogue_level))
#define Is_really_rogue_level(x) (gnh_has_rogue_level && Is_rogue_level(x))
#define Is_stronghold(x) (on_level(x, &stronghold_level))
#define Is_bigroom(x) (on_level(x, &bigroom_level))
#define Is_qstart(x) (on_level(x, &qstart_level))
#define Is_qlocate(x) (on_level(x, &qlocate_level))
#define Is_nemesis(x) (on_level(x, &nemesis_level))
#define Is_knox(x) (on_level(x, &knox_level))
#define Is_quinton_modron_level(x) (on_level(x, &quinton_modron_level))
#define Is_quarton_modron_level(x) (on_level(x, &quarton_modron_level))
#define Is_tertian_modron_level(x) (on_level(x, &tertian_modron_level))
#define Is_secundus_modron_level(x) (on_level(x, &secundus_modron_level))
#define Is_primus_modron_level(x) (on_level(x, &primus_modron_level))
#define Is_bovine_level(x) (on_level(x, &bovine_level))
#define Is_quantum_tunnel_level(x) (on_level(x, &quantum_tunnel_level))
#define Is_quantum_core_level(x) (on_level(x, &quantum_core_level))
#define Is_treasure_island_level(x) (on_level(x, &treasure_island_level))
#define Is_minetown_level(x) (on_level(x, &minetown_level))
#define Is_mineend_level(x) (on_level(x, &mineend_level))
#define Is_sokoend_level(x) (on_level(x, &sokoend_level))

#define In_sokoban(x) ((x)->dnum == sokoban_dnum)
#define Inhell In_hell(&u.uz) /* now gehennom */
#define In_endgame(x) ((x)->dnum == endgame_dnum)

#define within_bounded_area(X, Y, LX, LY, HX, HY) \
    ((X) >= (LX) && (X) <= (HX) && (Y) >= (LY) && (Y) <= (HY))

/* monster and object migration codes */

#define MIGR_NOWHERE (-1) /* failure flag for down_gate() */
#define MIGR_RANDOM 0
#define MIGR_APPROX_XY 1 /* approximate coordinates */
#define MIGR_EXACT_XY 2  /* specific coordinates */
#define MIGR_STAIRS_UP 3
#define MIGR_STAIRS_DOWN 4
#define MIGR_LADDER_UP 5
#define MIGR_LADDER_DOWN 6
#define MIGR_SSTAIRS 7      /* dungeon branch */
#define MIGR_PORTAL 8       /* magic portal */
#define MIGR_MODRON_PORTAL_UP 9 /* midgrate to modron up portal */
#define MIGR_MODRON_PORTAL_DOWN 10 /* midgrate to modron down portal */
#define MIGR_WITH_HERO 11    /* mon: followers; obj: trap door */
#define MIGR_MAGIC_CHEST 12  /* to the magic chest if found, otherwise random */
#define MIGR_NOBREAK 1024    /* bitmask: don't break on delivery */
#define MIGR_NOSCATTER 2048  /* don't scatter on delivery */
#define MIGR_TO_SPECIES 4096 /* migrating to species as they are made */ 
#define MIGR_LEFTOVERS 8192  /* grab remaining MIGR_TO_SPECIES objects */
/* level information (saved via ledger number) */

struct linfo {
    unsigned char flags;
#define VISITED 0x01      /* hero has visited this level */
#define FORGOTTEN 0x02    /* hero will forget this level when reached */
#define LFILE_EXISTS 0x04 /* a level file exists for this level */
        /* Note:  VISITED and LFILE_EXISTS are currently almost always
         * set at the same time.  However they _mean_ different things.
         */
#ifdef MFLOPPY
#define FROMPERM 1 /* for ramdisk use */
#define TOPERM 2   /* for ramdisk use */
#define ACTIVE 1
#define SWAPPED 2
    int where;
    int64_t time;
    int64_t size;
#endif /* MFLOPPY */
};

/* types and structures for dungeon map recording
 *
 * It is designed to eliminate the need for an external notes file for some
 * mundane dungeon elements.  "Where was the last altar I passed?" etc...
 * Presumably the character can remember this sort of thing even if, months
 * later in real time picking up an old save game, I can't.
 *
 * To be consistent, one can assume that this map is in the player's mind and
 * has no physical correspondence (eliminating illiteracy/blind/hands/hands
 * free concerns).  Therefore, this map is not exhaustive nor detailed ("some
 * fountains").  This makes it also subject to player conditions (amnesia).
 */

/* Because clearly GnollHack needs more ways to specify alignment */
#define Amask2msa(x) (((x) & AM_MASK) == 4 ? 3 : (x) & AM_MASK)
#define Msa2amask(x) ((x) == 3 ? 4 : (x))
#define MSA_NONE 0 /* unaligned or multiple alignments */
#define MSA_LAWFUL 1
#define MSA_NEUTRAL 2
#define MSA_CHAOTIC 3

/* what the player knows about a single dungeon level */
/* initialized in mklev() */
typedef struct mapseen {
    struct mapseen *next; /* next map in the chain */
    branch *br;           /* knows about branch via taking it in goto_level */
    d_level lev;          /* corresponding dungeon level */
    struct mapseen_feat {
        /* feature knowledge that must be calculated from levl array */
        Bitfield(nfount, 2);
        Bitfield(nsink, 2);
        Bitfield(naltar, 2);
        Bitfield(nthrone, 2);

        Bitfield(ngrave, 2);
        Bitfield(nbrazier, 2);
        Bitfield(nsignpost, 2);
        Bitfield(ntree, 2);

        Bitfield(nmagicportal, 2);
        Bitfield(nvibratingsquare, 2);
        Bitfield(water, 2);
        Bitfield(lava, 2);

        Bitfield(ice, 2);
        /* calculated from rooms array */
        Bitfield(nshop, 2);
        Bitfield(ntemple, 2);
        Bitfield(nsmithy, 2);

        Bitfield(nnpcroom, 2);
        /* altar alignment; MSA_NONE if there is more than one and
           they aren't all the same */
        Bitfield(msalign, 2);
        Bitfield(shoptype, 5);

        unsigned reserved;

        uchar npcroomtype;
        schar fountaintype;
    } feat;
    struct mapseen_flags {
        char special_description[BUFSZ];

        Bitfield(unreachable, 1); /* can't get back to this level */
        Bitfield(forgot, 1);      /* player has forgotten about this level */
        Bitfield(knownbones, 1);  /* player aware of bones */
        Bitfield(oracle, 1);
        Bitfield(sokosolved, 1);
        Bitfield(bigroom, 1);
        Bitfield(castle, 1);
        Bitfield(castletune, 1); /* add tune hint to castle annotation */

        Bitfield(valley, 1);
        Bitfield(msanctum, 1);
        Bitfield(ludios, 1);
        Bitfield(roguelevel, 1);
        Bitfield(special_level, 1);
        Bitfield(special_level_true_nature_known, 1);
        /* quest annotations: quest_summons is for main dungeon level
           with entry portal and is reset once quest has been finished;
           questing is for quest home (level 1) */
        Bitfield(quest_summons, 1); /* heard summons from leader */
        Bitfield(questing, 1); /* quest leader has unlocked quest stairs */
        /* Hints about other special dungeons */
        Bitfield(modron_hint_shown, 1); /* received hint for the modron level */
        Bitfield(yacc_hint_shown, 1); /* received hint for the Hellish Pastures level */
        Bitfield(quantum_hint_shown, 1); /* received hint for the Large Circular Dungeon level */
        Bitfield(lost_world_hint_shown, 1); /* reserved -- received hint for the Lost World level */

        unsigned reserved;
    } flags;
    /* custom naming */
    char *custom;
    uint64_t custom_lth;
    struct mapseen_rooms {
        boolean seen;
        boolean untended;         /* flag for shop without shk */
    } msrooms[(MAXNROFROOMS + 1) * 2]; /* same size as rooms[] */
    /* dead heroes; might not have graves or ghosts */
    struct cemetery *final_resting_place; /* same as level.bonesinfo */
} mapseen;

#endif /* DUNGEON_H */
